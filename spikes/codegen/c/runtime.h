/* ELAN codegen spike, C runtime: hash-consed terms, AC canonical form,
 * AC matching helpers, success continuations, printing.
 * Build with -DARENA for the bump-arena variant, otherwise Boehm GC. */
#ifndef ELAN_RUNTIME_H
#define ELAN_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* ---- symbols ---------------------------------------------------------- */
enum sym_kind { K_FREE, K_AC, K_INT, K_INFIX, K_RECORD };

typedef struct {
  const char *name;   /* printing name (infix operator for K_INFIX)      */
  int arity;          /* fixed arity (free symbols)                      */
  int kind;
  const char **fields;/* K_RECORD: field names, printed Name[f=..,g=..]  */
} SymInfo;

/* the program registers its symbol table; codes 0..2 are reserved and the
 * table must start with RT_BASE_SYMS (builtin int, true, false). */
#define SYM_TRUE 1
#define SYM_FALSE 2
#define RT_BASE_SYMS {"<int>",0,K_INT,0}, {"true",0,K_FREE,0}, {"false",0,K_FREE,0}
void rt_init(const SymInfo *syms, int nsyms);

/* ---- terms ------------------------------------------------------------ */
typedef struct Term Term;
struct Term {
  uint32_t sym;
  uint32_t arity;   /* number of arguments (AC: n >= 2, sorted by id)   */
  uint32_t hash;
  uint32_t id;      /* unique id, creation order: total order for AC    */
  Term *args[];     /* builtin int: value stored in args[0]             */
};

#define SYM_INT 0
#define INTVAL(t) ((intptr_t)(t)->args[0])
#define IS_INT(t) ((t)->sym == SYM_INT)

Term *mk_int(intptr_t v);
Term *mk_const(uint32_t sym);                         /* arity 0 */
Term *mk_app(uint32_t sym, uint32_t arity, Term **args);
Term *mk1(uint32_t sym, Term *a);
Term *mk2(uint32_t sym, Term *a, Term *b);

/* AC canonical form: flatten items whose top symbol is sym, sort by id
 * (duplicates kept). n_total >= 2 gives an AC node, 1 gives the item. */
Term *ac_build(uint32_t sym, Term **items, int n);
Term *ac_build2(uint32_t sym, Term *a, Term *b);

/* ---- AC matching helpers ----------------------------------------------
 * A match state over an AC node t is a `used` byte array (one per
 * argument position). ac_next enumerates the distinct elements of the
 * multiset t minus used, in order: returns the next index > prev (or -1). */
int ac_next(const Term *t, const uint8_t *used, int prev);
/* number of unused positions */
int ac_count_rest(const Term *t, const uint8_t *used);
/* the rest S = t minus used: NULL if empty (pattern variable cannot match
 * the empty multiset: no unit), the element if one, else an AC node.   */
Term *ac_rest(const Term *t, const uint8_t *used);
/* rest with one position removed (convenience: single-element patterns) */
Term *ac_rest_but(const Term *t, int i);

/* ---- builtins --------------------------------------------------------- */
extern Term *TRUE_T, *FALSE_T;   /* bool constants (registered by rt) */
static inline Term *mk_bool(bool b) { return b ? TRUE_T : FALSE_T; }
Term *bool_and(Term *a, Term *b);
/* builtinInt operations (the int module): strict, on hash-consed ints */
static inline Term *int_add(Term *a, Term *b) { return mk_int(INTVAL(a) + INTVAL(b)); }
static inline Term *int_sub(Term *a, Term *b) { return mk_int(INTVAL(a) - INTVAL(b)); }
static inline Term *int_mod(Term *a, Term *b) { return mk_int(INTVAL(a) % INTVAL(b)); }
static inline Term *int_gt(Term *a, Term *b) { return mk_bool(INTVAL(a) > INTVAL(b)); }
static inline Term *int_eq(Term *a, Term *b) { return mk_bool(a == b); }
static inline Term *int_neq(Term *a, Term *b) { return mk_bool(a != b); }
Term *bool_not(Term *a);

/* ---- success continuations -------------------------------------------- */
typedef struct {
  bool (*fn)(void *env, Term *t);   /* returns true = stop */
  void *env;
} K;
static inline bool kcall(K k, Term *t) { return k.fn(k.env, t); }

/* ---- misc ------------------------------------------------------------- */
extern unsigned long rewrite_step;
void print_term(FILE *f, Term *t);
void print_result(Term *t);       /* "result = <t>" */
void print_steps(void);           /* "rewrite_step = <n>" */

#endif
