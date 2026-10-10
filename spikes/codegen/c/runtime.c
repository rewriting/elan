/* ELAN codegen spike, C runtime (see runtime.h).
 *
 * Unique table: open addressing, linear probing, one slot per live term.
 *  - GC build (Boehm): the table is malloc'ed (not scanned) and its slots
 *    hold *hidden* pointers registered as disappearing links, so the table
 *    is weak: a term only reachable from the table is collected and its
 *    slot is cleared to 0 by the collector (a tombstone).
 *  - ARENA build: plain pointers, bump allocation, nothing is ever freed.
 */
#include "runtime.h"
#include <stdlib.h>
#include <string.h>

#ifdef ARENA
/* ---- bump arena ----------------------------------------------------- */
#define ARENA_CHUNK (1u << 20)
static char *arena_cur, *arena_end;
static void *rt_alloc(size_t sz, bool atomic) {
  (void)atomic;
  sz = (sz + 15) & ~(size_t)15;
  if (arena_cur + sz > arena_end) {
    size_t n = sz > ARENA_CHUNK ? sz : ARENA_CHUNK;
    arena_cur = malloc(n);
    if (!arena_cur) { perror("malloc"); exit(2); }
    arena_end = arena_cur + n;
  }
  void *p = arena_cur;
  arena_cur += sz;
  return p;
}
#define SLOT_EMPTY ((uintptr_t)0)
#define SLOT_DEAD ((uintptr_t)1) /* never produced in the arena build */
#define ENC(p) ((uintptr_t)(p))
#define DEC(s) ((Term *)(s))
#define REGISTER(slot, p) ((void)0)
#define MOVE_LINK(from, to) ((void)0)
#else
/* ---- Boehm GC ------------------------------------------------------- */
#include <gc/gc.h>
static void *rt_alloc(size_t sz, bool atomic) {
  void *p = atomic ? GC_MALLOC_ATOMIC(sz) : GC_MALLOC(sz);
  if (!p) { fprintf(stderr, "out of memory\n"); exit(2); }
  return p;
}
#ifdef STRONG_TABLE /* experiment: strong table (scanned), nothing collectable */
#define SLOT_EMPTY ((uintptr_t)0)
#define SLOT_DEAD ((uintptr_t)1)
#define ENC(p) ((uintptr_t)(p))
#define DEC(s) ((Term *)(s))
#define REGISTER(slot, p) ((void)0)
#define MOVE_LINK(from, to) ((void)0)
#define TAB_MALLOC(n) GC_MALLOC_UNCOLLECTABLE(n)
#define TAB_FREE(p) GC_FREE(p)
#else
#define SLOT_EMPTY ((uintptr_t)1) /* ~p is never 1 for a GC object */
#define SLOT_DEAD ((uintptr_t)0)  /* written by the collector      */
#define ENC(p) ((uintptr_t)GC_HIDE_POINTER(p))
#define DEC(s) ((Term *)GC_REVEAL_POINTER(s))
#define REGISTER(slot, p) GC_general_register_disappearing_link((void **)(slot), (p))
#define MOVE_LINK(from, to) GC_move_disappearing_link((void **)(from), (void **)(to))
#endif
#endif

unsigned long rewrite_step;
Term *TRUE_T, *FALSE_T;
static const SymInfo *symtab;
static int nsymtab;
static uint32_t next_id = 1;
#ifndef TAB_MALLOC
#define TAB_MALLOC(n) malloc(n)
#define TAB_FREE(p) free(p)
#endif

/* ---- unique table --------------------------------------------------- */
static uintptr_t *tab;
static size_t tab_cap, tab_used; /* used = live + tombstones */

static uintptr_t *tab_new(size_t cap) {
  uintptr_t *t = TAB_MALLOC(cap * sizeof *t);
  if (!t) { perror("malloc"); exit(2); }
  for (size_t i = 0; i < cap; i++) t[i] = SLOT_EMPTY;
  return t;
}

static void tab_rebuild(void) {
  size_t live = 0;
  for (size_t i = 0; i < tab_cap; i++)
    if (tab[i] != SLOT_EMPTY && tab[i] != SLOT_DEAD) live++;
  size_t cap = tab_cap;
  while (live * 4 > cap) cap *= 2;
  uintptr_t *nt = tab_new(cap);
  size_t mask = cap - 1;
  for (size_t i = 0; i < tab_cap; i++) {
    uintptr_t s = tab[i];
    if (s == SLOT_EMPTY || s == SLOT_DEAD) continue;
    size_t j = DEC(s)->hash & mask;
    while (nt[j] != SLOT_EMPTY) j = (j + 1) & mask;
    nt[j] = s;
    MOVE_LINK(&tab[i], &nt[j]);
  }
  TAB_FREE(tab);
  tab = nt;
  tab_cap = cap;
  tab_used = live;
}

static inline uint32_t hmix(uint64_t h, uint64_t v) {
  h = (h ^ v) * 0x9E3779B97F4A7C15ull;
  return (uint32_t)(h ^ (h >> 29));
}

/* hash-cons (sym, arity, args); for ints args[0] holds the value */
static Term *hcons(uint32_t sym, uint32_t arity, Term **args, intptr_t ival) {
  uint64_t h = hmix(sym * 31u + arity, (uint64_t)ival);
  for (uint32_t i = 0; i < arity; i++) h = hmix(h, (uint64_t)(uintptr_t)args[i] >> 4);
  uint32_t hash = (uint32_t)h;
  size_t mask = tab_cap - 1, i = hash & mask, ins = (size_t)-1;
  for (;;) {
    uintptr_t s = tab[i];
    if (s == SLOT_EMPTY) break;
    if (s == SLOT_DEAD) {
      if (ins == (size_t)-1) ins = i;
    } else {
      Term *p = DEC(s);
      if (p->hash == hash && p->sym == sym && p->arity == arity) {
        if (sym == SYM_INT) {
          if (INTVAL(p) == ival) return p;
        } else {
          uint32_t k = 0;
          while (k < arity && p->args[k] == args[k]) k++;
          if (k == arity) return p;
        }
      }
    }
    i = (i + 1) & mask;
  }
  if (ins == (size_t)-1) { ins = i; tab_used++; }
  uint32_t slots = sym == SYM_INT ? 1 : arity;
  Term *t = rt_alloc(sizeof(Term) + slots * sizeof(Term *), arity == 0);
  t->sym = sym;
  t->arity = arity;
  t->hash = hash;
  t->id = next_id++;
  if (sym == SYM_INT) t->args[0] = (Term *)ival;
  else memcpy(t->args, args, arity * sizeof(Term *));
  tab[ins] = ENC(t);
  REGISTER(&tab[ins], t);
  if (tab_used * 2 > tab_cap) tab_rebuild();
  return t;
}

Term *mk_int(intptr_t v) { return hcons(SYM_INT, 0, NULL, v); }
Term *mk_const(uint32_t sym) { return hcons(sym, 0, NULL, 0); }
Term *mk_app(uint32_t sym, uint32_t arity, Term **args) { return hcons(sym, arity, args, 0); }
Term *mk1(uint32_t sym, Term *a) { return hcons(sym, 1, &a, 0); }
Term *mk2(uint32_t sym, Term *a, Term *b) {
  Term *v[2] = {a, b};
  return hcons(sym, 2, v, 0);
}

/* ---- AC canonical form ---------------------------------------------- */
Term *ac_build(uint32_t sym, Term **items, int n) {
  int total = 0;
  for (int i = 0; i < n; i++) total += items[i]->sym == sym ? (int)items[i]->arity : 1;
  if (total == 1) return items[0];
  Term *buf[total];
  int m = 0;
  for (int i = 0; i < n; i++) {
    Term *x = items[i];
    if (x->sym == sym) for (uint32_t j = 0; j < x->arity; j++) buf[m++] = x->args[j];
    else buf[m++] = x;
  }
  /* insertion sort by id (inputs are mostly sorted runs) */
  for (int i = 1; i < m; i++) {
    Term *x = buf[i];
    int j = i - 1;
    while (j >= 0 && buf[j]->id > x->id) { buf[j + 1] = buf[j]; j--; }
    buf[j + 1] = x;
  }
  return hcons(sym, (uint32_t)m, buf, 0);
}

Term *ac_build2(uint32_t sym, Term *a, Term *b) {
  Term *v[2] = {a, b};
  return ac_build(sym, v, 2);
}

/* ---- AC matching helpers -------------------------------------------- */
int ac_next(const Term *t, const uint8_t *used, int prev) {
  for (int i = prev + 1; i < (int)t->arity; i++) {
    if (used[i]) continue;
    int p = i - 1;
    while (p >= 0 && used[p]) p--;
    if (p >= 0 && t->args[p] == t->args[i]) continue; /* same element */
    return i;
  }
  return -1;
}

int ac_count_rest(const Term *t, const uint8_t *used) {
  int n = 0;
  for (uint32_t i = 0; i < t->arity; i++) n += !used[i];
  return n;
}

Term *ac_rest(const Term *t, const uint8_t *used) {
  Term *buf[t->arity];
  int m = 0;
  for (uint32_t i = 0; i < t->arity; i++)
    if (!used[i]) buf[m++] = t->args[i];
  if (m == 0) return NULL;
  if (m == 1) return buf[0];
  return hcons(t->sym, (uint32_t)m, buf, 0); /* already sorted */
}

Term *ac_rest_but(const Term *t, int i) {
  uint8_t used[t->arity];
  memset(used, 0, t->arity);
  used[i] = 1;
  return ac_rest(t, used);
}

/* ---- builtins ------------------------------------------------------- */
Term *bool_and(Term *a, Term *b) { return a == TRUE_T ? b : FALSE_T; }
Term *bool_not(Term *a) { return a == TRUE_T ? FALSE_T : TRUE_T; }

/* ---- init / printing ------------------------------------------------ */
void rt_init(const SymInfo *syms, int nsyms) {
#ifndef ARENA
  GC_INIT();
#endif
  symtab = syms;
  nsymtab = nsyms;
  tab_cap = 1u << 16;
  tab = tab_new(tab_cap);
  TRUE_T = mk_const(SYM_TRUE);
  FALSE_T = mk_const(SYM_FALSE);
}

void print_term(FILE *f, Term *t) {
  const SymInfo *s = &symtab[t->sym];
  switch (s->kind) {
  case K_INT:
    fprintf(f, "%ld", (long)INTVAL(t));
    return;
  case K_INFIX:
    print_term(f, t->args[0]);
    fputs(s->name, f);
    print_term(f, t->args[1]);
    return;
  case K_RECORD:
    fprintf(f, "%s[", s->name);
    for (uint32_t i = 0; i < t->arity; i++) {
      fprintf(f, "%s%s=", i ? "," : "", s->fields[i]);
      print_term(f, t->args[i]);
    }
    fputc(']', f);
    return;
  default:
    fputs(s->name, f);
    if (t->arity) {
      fputc('(', f);
      for (uint32_t i = 0; i < t->arity; i++) {
        if (i) fputc(',', f);
        print_term(f, t->args[i]);
      }
      fputc(')', f);
    }
  }
}

void print_result(Term *t) {
  fputs("result = ", stdout);
  print_term(stdout, t);
  fputc('\n', stdout);
}

void print_steps(void) { printf("rewrite_step = %lu\n", rewrite_step); }
