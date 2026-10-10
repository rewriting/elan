/* "Generated" code for nqueens.eln (hand-written as the new compiler would
 * emit it): one function per defined symbol (innermost normalisation),
 * one per labelled rule and one per strategy, in CPS. */
#include "runtime.h"
#include <stdlib.h>

enum { S_INT, S_TRUE, S_FALSE, S_A, S_NIL, S_CONS, S_GENLIST, S_QUEENS2,
       S_QUEENS1, S_NOATTACK, S_Q2I, NSYMS };
static const SymInfo syms[NSYMS] = {
  RT_BASE_SYMS,
  [S_A] = {"a", 0, K_FREE, 0},
  [S_NIL] = {"nil", 0, K_FREE, 0},
  [S_CONS] = {".", 2, K_INFIX, 0},
  [S_GENLIST] = {"generate_list", 1, K_FREE, 0},
  [S_QUEENS2] = {"queens", 2, K_FREE, 0},
  [S_QUEENS1] = {"queens", 1, K_FREE, 0},
  [S_NOATTACK] = {"noattack", 3, K_FREE, 0},
  [S_Q2I] = {"q2i", 1, K_FREE, 0},
};
static Term *C_nil, *C_0, *C_1;

/* ---- normalising functions (unlabelled rules) ------------------------ */

/* constructor  @.@ : (int list) list */
static Term *fn_cons(Term *x, Term *l) { return mk2(S_CONS, x, l); }

/* [] q2i(n) => n */
static Term *fn_q2i(Term *n) {
  rewrite_step++;
  return n;
}

/* [] generate_list(0) => nil
   [] generate_list(n) => n . generate_list(n-1) */
static Term *fn_generate_list(Term *n) {
  if (n == C_0) { rewrite_step++; return C_nil; }
  rewrite_step++;
  return fn_cons(n, fn_generate_list(int_sub(n, C_1)));
}

/* queens(@,@): only labelled rules -> constructor */
static Term *fn_queens2(Term *n, Term *size) { return mk2(S_QUEENS2, n, size); }

/* [] queens(n) => queens(n,n) */
static Term *fn_queens1(Term *n) {
  rewrite_step++;
  return fn_queens2(n, n);
}

/* [] noattack(diff,d,nil) => true
   [] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff
                              and noattack(diff+1,d,l) */
static Term *fn_noattack(Term *diff, Term *d, Term *l) {
  if (l == C_nil) { rewrite_step++; return TRUE_T; }
  if (l->sym == S_CONS) {
    Term *p = l->args[0], *l2 = l->args[1];
    Term *b1 = int_neq(d, p);
    Term *b2 = int_neq(int_sub(d, p), diff);
    Term *b3 = bool_and(b1, b2);
    Term *b4 = int_neq(int_sub(p, d), diff);
    Term *b5 = bool_and(b3, b4);
    Term *b6 = fn_noattack(int_add(diff, C_1), d, l2);
    rewrite_step++;
    return bool_and(b5, b6);
  }
  Term *v[3] = {diff, d, l};
  return mk_app(S_NOATTACK, 3, v);
}

/* ---- labelled rules and strategies (CPS) ----------------------------- */

/* [range_rule] x => x-1 if x > 1 */
static bool rule_range_rule(Term *x, K k) {
  if (int_gt(x, C_1) != TRUE_T) return false;
  rewrite_step++;
  return kcall(k, int_sub(x, C_1));
}

/* dc(range_rule): results of the first alternative that has one */
typedef struct { K k; bool got; } dc_env;
static bool dc_k(void *env, Term *r) {
  dc_env *e = env;
  e->got = true;
  return kcall(e->k, r);
}
static bool st_dc_range_rule(Term *t, K k) {
  dc_env e = {k, false};
  bool stop = rule_range_rule(t, (K){dc_k, &e});
  if (stop || e.got) return stop;
  return false; /* no alternative left */
}

/* range => iterate*(dc(range_rule)) */
static bool st_range(Term *t, K k);
static bool range_iter_k(void *env, Term *r) { return st_range(r, *(K *)env); }
static bool st_range(Term *t, K k) {
  if (kcall(k, t)) return true;
  return st_dc_range_rule(t, (K){range_iter_k, &k});
}

/* [queens_0] queens(0,size) => nil */
static bool rule_queens_0(Term *t, K k) {
  if (t->sym != S_QUEENS2 || t->args[0] != C_0) return false;
  rewrite_step++;
  return kcall(k, C_nil);
}

/* [queens_n] queens(n,size) => x . ql
     if n>0
     where ql:=(queens_strat) queens(n-1,size)
     where xx:=(range) size
     where x:=() q2i(xx)
     if noattack(1,x,ql) */
static bool st_queens_strat(Term *t, K k);
typedef struct { Term *n, *size; K k; } queens_n_env1;
typedef struct { queens_n_env1 *up; Term *ql; } queens_n_env2;
static bool queens_n_k2(void *env, Term *xx) { /* after: where xx:=(range) size */
  queens_n_env2 *e = env;
  Term *x = fn_q2i(xx);
  if (fn_noattack(C_1, x, e->ql) != TRUE_T) return false;
  rewrite_step++;
  return kcall(e->up->k, fn_cons(x, e->ql));
}
static bool queens_n_k1(void *env, Term *ql) { /* after: where ql:=(queens_strat) ... */
  queens_n_env1 *e = env;
  queens_n_env2 e2 = {e, ql};
  return st_range(e->size, (K){queens_n_k2, &e2});
}
static bool rule_queens_n(Term *t, K k) {
  if (t->sym != S_QUEENS2) return false;
  Term *n = t->args[0], *size = t->args[1];
  if (int_gt(n, C_0) != TRUE_T) return false;
  queens_n_env1 e = {n, size, k};
  return st_queens_strat(fn_queens2(int_sub(n, C_1), size), (K){queens_n_k1, &e});
}

/* queens_strat => dk(queens_0, queens_n) */
static bool st_queens_strat(Term *t, K k) {
  if (rule_queens_0(t, k)) return true;
  return rule_queens_n(t, k);
}

/* det_queens_strat => first one(queens_0, queens_n) */
typedef struct { Term *r; } one_env;
static bool one_k(void *env, Term *r) { ((one_env *)env)->r = r; return true; }
__attribute__((unused)) static bool st_det_queens_strat(Term *t, K k) {
  one_env e = {NULL};
  if (!rule_queens_0(t, (K){one_k, &e})) rule_queens_n(t, (K){one_k, &e});
  return e.r ? kcall(k, e.r) : false;
}

/* ---- main: start with (queens_strat) queens(query) -------------------- */
static bool print_k(void *env, Term *r) {
  (void)env;
  print_result(r);
  return false; /* all solutions */
}

int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: nqueens N\n"); return 1; }
  rt_init(syms, NSYMS);
  C_nil = mk_const(S_NIL);
  C_0 = mk_int(0);
  C_1 = mk_int(1);
  (void)fn_generate_list;
  Term *q = fn_queens1(mk_int(atol(argv[1])));
  st_queens_strat(q, (K){print_k, NULL});
  print_steps();
  return 0;
}
