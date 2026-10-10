/* "Generated" code for efib.eln (hand-written as the new compiler would
 * emit it). Space is the AC symbol U (flattened, sorted by id); the
 * injections Fib->Object->Space and builtinInt->eInt are invisible. */
#include "runtime.h"
#include <stdlib.h>
#include <string.h>

enum { S_INT, S_TRUE, S_FALSE, S_U, S_EMPTY, S_FIB, S_COMPUTE, S_UNDEF,
       S_OCCURSFIB, S_GO, S_RESULT, NSYMS };
static const char *fib_fields[] = {"farg", "val"};
static const char *compute_fields[] = {"question", "answer"};
static const SymInfo syms[NSYMS] = {
  RT_BASE_SYMS,
  [S_U] = {"U", 2, K_AC, 0},
  [S_EMPTY] = {"empty", 0, K_FREE, 0},
  [S_FIB] = {"Fib", 2, K_RECORD, fib_fields},
  [S_COMPUTE] = {"Compute", 2, K_RECORD, compute_fields},
  [S_UNDEF] = {"UNDEF", 0, K_FREE, 0},
  [S_OCCURSFIB] = {"occursFib", 2, K_FREE, 0},
  [S_GO] = {"go", 1, K_FREE, 0},
  [S_RESULT] = {"result", 2, K_FREE, 0},
};
static Term *C_empty, *C_UNDEF, *C_0, *C_1, *C_2, *C_3, *C_1000000;

/* ---- constructors / normalising functions ----------------------------- */

/* constructor Fib[farg=@,val=@] */
static Term *fn_fib(Term *farg, Term *val) { return mk2(S_FIB, farg, val); }

/* @ U @ (AC), only labelled rules: flatten + sort */
__attribute__((unused)) static Term *fn_U(Term *a, Term *b) { return ac_build2(S_U, a, b); }

/* [] occursFib(S U Fib[farg=n,val=v],n) => true
   [] occursFib(S,n)                     => false */
static Term *fn_occursFib(Term *s, Term *n) {
  if (s->sym == S_U) {
    uint8_t used[s->arity];
    memset(used, 0, s->arity);
    for (int i = ac_next(s, used, -1); i >= 0; i = ac_next(s, used, i)) {
      Term *e = s->args[i];
      if (e->sym != S_FIB || e->args[0] != n) continue;
      used[i] = 1;
      if (ac_count_rest(s, used) >= 1) { rewrite_step++; return TRUE_T; } /* S unused */
      used[i] = 0;
    }
  }
  rewrite_step++;
  return FALSE_T;
}

/* [] result(S U Fib[farg=n,val=v],n) => v */
static Term *fn_result(Term *s, Term *n) {
  if (s->sym == S_U) {
    uint8_t used[s->arity];
    memset(used, 0, s->arity);
    for (int i = ac_next(s, used, -1); i >= 0; i = ac_next(s, used, i)) {
      Term *e = s->args[i];
      if (e->sym != S_FIB || e->args[0] != n) continue;
      used[i] = 1;
      if (ac_count_rest(s, used) >= 1) { rewrite_step++; return e->args[1]; }
      used[i] = 0;
    }
  }
  return mk2(S_RESULT, s, n);
}

/* ---- labelled rules (CPS) --------------------------------------------- */

/* [rec1] S U Fib[farg=n,val=UNDEF]
       => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-1,val=UNDEF]
          if n > 2  if not(occursFib(S,n-1)) */
static bool rule_rec1(Term *t, K k) {
  if (t->sym != S_U) return false;
  uint8_t used[t->arity];
  memset(used, 0, t->arity);
  for (int i = ac_next(t, used, -1); i >= 0; i = ac_next(t, used, i)) {
    Term *e = t->args[i];
    if (e->sym != S_FIB || e->args[1] != C_UNDEF) continue;
    Term *n = e->args[0];
    if (int_gt(n, C_2) != TRUE_T) continue;
    used[i] = 1;
    Term *s = ac_rest(t, used);
    if (s && bool_not(fn_occursFib(s, int_sub(n, C_1))) == TRUE_T) {
      rewrite_step++;
      Term *items[3] = {s, fn_fib(n, C_UNDEF), fn_fib(int_sub(n, C_1), C_UNDEF)};
      if (kcall(k, ac_build(S_U, items, 3))) return true;
    }
    used[i] = 0;
  }
  return false;
}

/* [rec2] S U Fib[farg=n,val=UNDEF]
       => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-2,val=UNDEF]
          if n > 3  if not(occursFib(S,n-2)) */
static bool rule_rec2(Term *t, K k) {
  if (t->sym != S_U) return false;
  uint8_t used[t->arity];
  memset(used, 0, t->arity);
  for (int i = ac_next(t, used, -1); i >= 0; i = ac_next(t, used, i)) {
    Term *e = t->args[i];
    if (e->sym != S_FIB || e->args[1] != C_UNDEF) continue;
    Term *n = e->args[0];
    if (int_gt(n, C_3) != TRUE_T) continue;
    used[i] = 1;
    Term *s = ac_rest(t, used);
    if (s && bool_not(fn_occursFib(s, int_sub(n, C_2))) == TRUE_T) {
      rewrite_step++;
      Term *items[3] = {s, fn_fib(n, C_UNDEF), fn_fib(int_sub(n, C_2), C_UNDEF)};
      if (kcall(k, ac_build(S_U, items, 3))) return true;
    }
    used[i] = 0;
  }
  return false;
}

/* [compute] S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2] U Fib[farg=n,val=UNDEF]
   => S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2] U Fib[farg=n,val=v1+v2 % 1000000]
      if n1 == n2 + 1  if n == n1 + 1
   (v1, v2 : builtinInt, so UNDEF does not match them; each condition is
   tested as soon as its variables are bound) */
static bool rule_compute(Term *t, K k) {
  if (t->sym != S_U) return false;
  uint8_t used[t->arity];
  memset(used, 0, t->arity);
  for (int i = ac_next(t, used, -1); i >= 0; i = ac_next(t, used, i)) {
    Term *e1 = t->args[i];
    if (e1->sym != S_FIB || !IS_INT(e1->args[1])) continue;
    Term *n1 = e1->args[0], *v1 = e1->args[1];
    used[i] = 1;
    for (int j = ac_next(t, used, -1); j >= 0; j = ac_next(t, used, j)) {
      Term *e2 = t->args[j];
      if (e2->sym != S_FIB || !IS_INT(e2->args[1])) continue;
      Term *n2 = e2->args[0], *v2 = e2->args[1];
      if (int_eq(n1, int_add(n2, C_1)) != TRUE_T) continue;
      used[j] = 1;
      for (int l = ac_next(t, used, -1); l >= 0; l = ac_next(t, used, l)) {
        Term *e3 = t->args[l];
        if (e3->sym != S_FIB || e3->args[1] != C_UNDEF) continue;
        Term *n = e3->args[0];
        if (int_eq(n, int_add(n1, C_1)) != TRUE_T) continue;
        used[l] = 1;
        Term *s = ac_rest(t, used);
        if (s) {
          rewrite_step++;
          Term *items[4] = {s, fn_fib(n1, v1), fn_fib(n2, v2),
                            fn_fib(n, int_add(v1, int_mod(v2, C_1000000)))};
          if (kcall(k, ac_build(S_U, items, 4))) return true;
        }
        used[l] = 0;
      }
      used[j] = 0;
    }
    used[i] = 0;
  }
  return false;
}

/* ---- strategies ------------------------------------------------------- */

typedef struct { Term *r; } one_env;
static bool one_k(void *env, Term *r) { ((one_env *)env)->r = r; return true; }

/* first one(rec1, rec2, compute) */
static bool st_first_one_rec1_rec2_compute(Term *t, K k) {
  one_env e = {NULL};
  K k1 = {one_k, &e};
  if (rule_rec1(t, k1) || rule_rec2(t, k1) || rule_compute(t, k1)) return kcall(k, e.r);
  return false;
}

/* [] loop => repeat*(first one(rec1,rec2,compute)) */
static bool st_loop(Term *t, K k) {
  for (;;) {
    one_env e = {NULL};
    if (!st_first_one_rec1_rec2_compute(t, (K){one_k, &e})) break;
    t = e.r;
  }
  return kcall(k, t);
}

/* [] go(n) => result(S,n)
      where S:=(loop) empty U Fib[farg=0,val=1] U Fib[farg=1,val=1]
                            U Fib[farg=n,val=UNDEF] */
static Term *fn_go(Term *n) {
  Term *items[4] = {C_empty, fn_fib(C_0, C_1), fn_fib(C_1, C_1), fn_fib(n, C_UNDEF)};
  one_env e = {NULL};
  st_loop(ac_build(S_U, items, 4), (K){one_k, &e});
  if (e.r) {
    rewrite_step++;
    return fn_result(e.r, n);
  }
  return mk1(S_GO, n);
}

/* ---- main: start with () query, query = go(N) ------------------------- */
int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: efib N\n"); return 1; }
  rt_init(syms, NSYMS);
  C_empty = mk_const(S_EMPTY);
  C_UNDEF = mk_const(S_UNDEF);
  C_0 = mk_int(0); C_1 = mk_int(1); C_2 = mk_int(2); C_3 = mk_int(3);
  C_1000000 = mk_int(1000000);
  print_result(fn_go(mk_int(atol(argv[1]))));
  print_steps();
  return 0;
}
