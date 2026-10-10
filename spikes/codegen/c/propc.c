/* "Generated" code for propc.eln (hand-written as the new compiler would
 * emit it). and, xor are AC (flattened, sorted by id, duplicates kept).
 * AC rules follow REM's compilation: a non-linear AC pattern gets an
 * implicit extension R (and(x,x,R), xor(x,x,R)); in and(x,t), and(x,f),
 * xor(x,f), and(x,xor(y,z)) the variable x takes the whole rest; y is
 * the first element of the xor and z its rest. A variable bound to a
 * rest is rebuilt through the normalising function before use. */
#include "runtime.h"
#include <stdlib.h>
#include <string.h>

enum { S_INT, S_TRUE, S_FALSE, S_T, S_F, S_AND, S_XOR, S_OR, S_IFF, S_NOT,
       S_IMPLIES, S_A1, S_A18 = S_A1 + 17, S_Q1, S_Q2, S_Q3, NSYMS };
static const SymInfo syms[NSYMS] = {
  RT_BASE_SYMS,
  [S_T] = {"t", 0, K_FREE, 0}, [S_F] = {"f", 0, K_FREE, 0},
  [S_AND] = {"and", 2, K_AC, 0}, [S_XOR] = {"xor", 2, K_AC, 0},
  [S_OR] = {"or", 2, K_FREE, 0}, [S_IFF] = {"iff", 2, K_FREE, 0},
  [S_NOT] = {"not", 1, K_FREE, 0}, [S_IMPLIES] = {"implies", 2, K_FREE, 0},
  [S_A1] = {"a1", 0, K_FREE, 0}, [S_A1 + 1] = {"a2", 0, K_FREE, 0},
  [S_A1 + 2] = {"a3", 0, K_FREE, 0}, [S_A1 + 3] = {"a4", 0, K_FREE, 0},
  [S_A1 + 4] = {"a5", 0, K_FREE, 0}, [S_A1 + 5] = {"a6", 0, K_FREE, 0},
  [S_A1 + 6] = {"a7", 0, K_FREE, 0}, [S_A1 + 7] = {"a8", 0, K_FREE, 0},
  [S_A1 + 8] = {"a9", 0, K_FREE, 0}, [S_A1 + 9] = {"a10", 0, K_FREE, 0},
  [S_A1 + 10] = {"a11", 0, K_FREE, 0}, [S_A1 + 11] = {"a12", 0, K_FREE, 0},
  [S_A1 + 12] = {"a13", 0, K_FREE, 0}, [S_A1 + 13] = {"a14", 0, K_FREE, 0},
  [S_A1 + 14] = {"a15", 0, K_FREE, 0}, [S_A1 + 15] = {"a16", 0, K_FREE, 0},
  [S_A1 + 16] = {"a17", 0, K_FREE, 0}, [S_A18] = {"a18", 0, K_FREE, 0},
  [S_Q1] = {"q1", 0, K_FREE, 0}, [S_Q2] = {"q2", 0, K_FREE, 0},
  [S_Q3] = {"q3", 0, K_FREE, 0},
};
static Term *C_t, *C_f, *C_a1, *C_a2, *C_a3, *C_a4, *C_a5, *C_a6, *C_a7,
    *C_a8, *C_a9, *C_a10, *C_a11;

static Term *and_rules(Term *t);
static Term *xor_rules(Term *t);

/* normalise a variable bound to an AC rest: element -> itself,
   AC node -> through the rules of its symbol */
static Term *renorm_and(Term *x) { return x->sym == S_AND ? and_rules(x) : x; }
static Term *renorm_xor(Term *x) { return x->sym == S_XOR ? xor_rules(x) : x; }

/* and(@,@) (AC): flatten + sort, then the rules */
static Term *fn_and(Term *a, Term *b) {
  Term *t = ac_build2(S_AND, a, b);
  return t->sym == S_AND ? and_rules(t) : t;
}
/* xor(@,@) (AC): flatten + sort, then the rules */
static Term *fn_xor(Term *a, Term *b) {
  Term *t = ac_build2(S_XOR, a, b);
  return t->sym == S_XOR ? xor_rules(t) : t;
}

/* rules of and, on a canonical AC node t (arity >= 2), in source order:
   [] and(x, x) => x                    (as and(x,x,R) => and(x,R))
   [] and(x, t) => x
   [] and(x, f) => f
   [] and(x, xor(y, z)) => xor(and(x, y), and(x, z)) */
static Term *and_rules(Term *t) {
  int n = (int)t->arity;
  for (int i = 0; i + 1 < n; i++)
    if (t->args[i] == t->args[i + 1]) {          /* and(x,x,R) */
      rewrite_step++;
      Term *r = ac_rest_but(t, i);               /* x + R */
      return r->sym == S_AND ? and_rules(r) : r;
    }
  for (int i = 0; i < n; i++)
    if (t->args[i] == C_t) {                     /* and(x,t) */
      rewrite_step++;
      return renorm_and(ac_rest_but(t, i));
    }
  for (int i = 0; i < n; i++)
    if (t->args[i] == C_f) {                     /* and(x,f) */
      rewrite_step++;
      return C_f;
    }
  for (int i = 0; i < n; i++)
    if (t->args[i]->sym == S_XOR) {              /* and(x,xor(y,z)) */
      Term *xo = t->args[i];
      Term *x = renorm_and(ac_rest_but(t, i));
      Term *y = xo->args[0];
      Term *z = renorm_xor(ac_rest_but(xo, 0));
      rewrite_step++;
      return fn_xor(fn_and(x, y), fn_and(x, z));
    }
  return t;
}

/* rules of xor, on a canonical AC node t:
   [] xor(x, x) => f                    (as xor(x,x,R) => xor(R,f))
   [] xor(x, f) => x */
static Term *xor_rules(Term *t) {
  int n = (int)t->arity;
  for (int i = 0; i + 1 < n; i++)
    if (t->args[i] == t->args[i + 1]) {          /* xor(x,x,R) */
      rewrite_step++;
      uint8_t used[n];
      memset(used, 0, n);
      used[i] = used[i + 1] = 1;
      Term *r = ac_rest(t, used);
      return r ? fn_xor(r, C_f) : C_f;
    }
  for (int i = 0; i < n; i++)
    if (t->args[i] == C_f) {                     /* xor(x,f) */
      rewrite_step++;
      return renorm_xor(ac_rest_but(t, i));
    }
  return t;
}

/* [] not(x) => xor(x, t) */
static Term *fn_not(Term *x) { rewrite_step++; return fn_xor(x, C_t); }
/* [] implies(x, y) => not(xor(x, and(x, y))) */
static Term *fn_implies(Term *x, Term *y) { rewrite_step++; return fn_not(fn_xor(x, fn_and(x, y))); }
/* [] or(x, y) => xor(and(x, y), xor(x, y)) */
static Term *fn_or(Term *x, Term *y) { rewrite_step++; return fn_xor(fn_and(x, y), fn_xor(x, y)); }
/* [] iff(x, y) => not(xor(x, y)) */
static Term *fn_iff(Term *x, Term *y) { rewrite_step++; return fn_not(fn_xor(x, y)); }

/* [] q1 => ... (rhs copied from propc.eln) */
static Term *fn_q1(void) {
  rewrite_step++;
  return fn_implies(fn_and(fn_iff(fn_iff(fn_or(C_a1, C_a2), fn_or(fn_not(C_a3), fn_iff(fn_xor(C_a4, C_a5), fn_not(fn_not(fn_not(C_a6)))))), fn_not(fn_and(fn_and(C_a7, C_a8), fn_not(fn_xor(fn_xor(fn_or(C_a9, fn_and(C_a10, C_a11)), C_a2), fn_and(fn_and(C_a11, fn_xor(C_a2, fn_iff(C_a5, C_a5))), fn_xor(fn_xor(C_a7, C_a7), fn_iff(C_a9, C_a4)))))))), fn_implies(fn_iff(fn_iff(fn_or(C_a1, C_a2), fn_or(fn_not(C_a3), fn_iff(fn_xor(C_a4, C_a5), fn_not(fn_not(fn_not(C_a6)))))), fn_not(fn_and(fn_and(C_a7, C_a8), fn_not(fn_xor(fn_xor(fn_or(C_a9, fn_and(C_a10, C_a11)), C_a2), fn_and(fn_and(C_a11, fn_xor(C_a2, fn_iff(C_a5, C_a5))), fn_xor(fn_xor(C_a7, C_a7), fn_iff(C_a9, C_a4)))))))), fn_not(fn_and(fn_implies(fn_and(C_a1, C_a2), fn_not(fn_xor(fn_or(fn_or(fn_xor(fn_implies(fn_and(C_a3, C_a4), fn_implies(C_a5, C_a6)), fn_or(C_a7, C_a8)), fn_xor(fn_iff(C_a9, C_a10), C_a11)), fn_xor(fn_xor(C_a2, C_a2), C_a7)), fn_iff(fn_or(C_a4, C_a9), fn_xor(fn_not(C_a6), C_a6))))), fn_not(fn_iff(fn_not(C_a11), fn_not(C_a9))))))), fn_not(fn_and(fn_implies(fn_and(C_a1, C_a2), fn_not(fn_xor(fn_or(fn_or(fn_xor(fn_implies(fn_and(C_a3, C_a4), fn_implies(C_a5, C_a6)), fn_or(C_a7, C_a8)), fn_xor(fn_iff(C_a9, C_a10), C_a11)), fn_xor(fn_xor(C_a2, C_a2), C_a7)), fn_iff(fn_or(C_a4, C_a9), fn_xor(fn_not(C_a6), C_a6))))), fn_not(fn_iff(fn_not(C_a11), fn_not(C_a9))))));
}

/* [] q2 => ... (rhs copied from propc.eln) */
static Term *fn_q2(void) {
  rewrite_step++;
  return fn_implies(fn_and(fn_not(fn_and(fn_xor(C_a1, fn_xor(fn_or(C_a2, C_a3), C_a4)), fn_xor(fn_iff(fn_xor(fn_not(C_a5), fn_or(fn_xor(fn_iff(C_a6, C_a7), fn_iff(C_a8, C_a9)), fn_and(C_a10, C_a9))), fn_iff(fn_not(fn_not(C_a2)), fn_implies(fn_or(C_a9, C_a6), fn_or(C_a10, C_a5)))), fn_not(fn_or(C_a9, fn_implies(fn_not(C_a8), fn_or(C_a4, C_a9))))))), fn_implies(fn_not(fn_and(fn_xor(C_a1, fn_xor(fn_or(C_a2, C_a3), C_a4)), fn_xor(fn_iff(fn_xor(fn_not(C_a5), fn_or(fn_xor(fn_iff(C_a6, C_a7), fn_iff(C_a8, C_a9)), fn_and(C_a10, C_a9))), fn_iff(fn_not(fn_not(C_a2)), fn_implies(fn_or(C_a9, C_a6), fn_or(C_a10, C_a5)))), fn_not(fn_or(C_a9, fn_implies(fn_not(C_a8), fn_or(C_a4, C_a9))))))), fn_not(fn_implies(fn_implies(fn_and(fn_or(C_a1, fn_xor(fn_xor(C_a2, C_a3), fn_not(C_a4))), fn_not(fn_xor(C_a5, fn_and(C_a6, C_a7)))), fn_implies(fn_xor(fn_implies(C_a8, C_a9), C_a10), fn_xor(fn_and(C_a4, fn_or(C_a4, C_a1)), C_a2))), fn_or(fn_or(fn_xor(fn_or(C_a4, C_a7), C_a2), fn_and(C_a8, C_a1)), fn_not(fn_not(fn_not(C_a6)))))))), fn_not(fn_implies(fn_implies(fn_and(fn_or(C_a1, fn_xor(fn_xor(C_a2, C_a3), fn_not(C_a4))), fn_not(fn_xor(C_a5, fn_and(C_a6, C_a7)))), fn_implies(fn_xor(fn_implies(C_a8, C_a9), C_a10), fn_xor(fn_and(C_a4, fn_or(C_a4, C_a1)), C_a2))), fn_or(fn_or(fn_xor(fn_or(C_a4, C_a7), C_a2), fn_and(C_a8, C_a1)), fn_not(fn_not(fn_not(C_a6)))))));
}

/* [] q3 => ... (rhs copied from propc.eln) */
static Term *fn_q3(void) {
  rewrite_step++;
  return fn_implies(fn_and(fn_not(fn_and(fn_xor(C_a1, fn_xor(fn_or(C_a2, C_a3), C_a4)), fn_xor(fn_iff(fn_xor(fn_not(C_a5),fn_or(fn_xor(fn_iff(C_a6, C_a7), fn_iff(C_a8, C_a9)), fn_and(C_a10, C_a11))), fn_implies(fn_or(C_a4,fn_and(C_a3, fn_iff(C_a1, C_a2))) , fn_not(fn_not(C_a4)))), fn_xor(fn_implies(fn_implies(C_a6, C_a1),fn_not(C_a1)), fn_not(C_a9))))) , fn_implies(fn_not(fn_and(fn_xor(C_a1, fn_xor(fn_or(C_a2, C_a3), C_a4)), fn_xor(fn_iff(fn_xor(fn_not(C_a5), fn_or(fn_xor(fn_iff(C_a6, C_a7), fn_iff(C_a8, C_a9)), fn_and(C_a10, C_a11))), fn_implies(fn_or(C_a4, fn_and(C_a3, fn_iff(C_a1, C_a2))), fn_not(fn_not(C_a4)))), fn_xor(fn_implies(fn_implies(C_a6, C_a1), fn_not(C_a1)), fn_not(C_a9))))), fn_not(fn_implies(fn_implies(fn_and(fn_or(C_a1, fn_xor(fn_xor(C_a2, C_a3), fn_not(C_a4))), fn_not(fn_xor(C_a5, fn_and(C_a6, C_a7)))), fn_implies(fn_xor(fn_implies(C_a8, C_a9), C_a10), fn_xor(fn_and(C_a11, fn_implies(C_a2, C_a8)), C_a8))), fn_not(fn_or(fn_implies(fn_or(C_a5, fn_or(C_a8, fn_and(C_a8, C_a9))), fn_not(C_a2)), fn_not(C_a7))))))), fn_not(fn_implies(fn_implies(fn_and(fn_or(C_a1, fn_xor(fn_xor(C_a2, C_a3), fn_not(C_a4))), fn_not(fn_xor(C_a5, fn_and(C_a6, C_a7)))), fn_implies(fn_xor(fn_implies(C_a8, C_a9), C_a10), fn_xor(fn_and(C_a11, fn_implies(C_a2, C_a8)), C_a8))), fn_not(fn_or(fn_implies(fn_or(C_a5, fn_or(C_a8, fn_and(C_a8, C_a9))), fn_not(C_a2)), fn_not(C_a7))))));
}

/* ---- main: start with () q<N> ----------------------------------------- */
int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: propc 1|2|3\n"); return 1; }
  rt_init(syms, NSYMS);
  C_t = mk_const(S_T);
  C_f = mk_const(S_F);
  Term **as[] = {&C_a1, &C_a2, &C_a3, &C_a4, &C_a5, &C_a6, &C_a7, &C_a8, &C_a9, &C_a10, &C_a11};
  for (int i = 0; i < 11; i++) *as[i] = mk_const(S_A1 + i);
  int q = atoi(argv[1]);
  Term *r = q == 1 ? fn_q1() : q == 2 ? fn_q2() : fn_q3();
  print_result(r);
  print_steps();
  return 0;
}
