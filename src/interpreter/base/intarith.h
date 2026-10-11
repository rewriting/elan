// Integer arithmetic on C int, with the results the interpreter always gave
// on two's complement machines (arm64), but defined: + - * and unary minus
// wrap modulo 2^32 (signed overflow was undefined behaviour), x/0 = 0,
// x%0 = x, INT_MIN/-1 = INT_MIN, INT_MIN%-1 = 0 (x/0 trapped on x86).
// Used by the int builtins (rewrite/rtmisc.cc, tests/regression/int_arith_wraps)
// and by the preprocessor's constant expressions (lex/mlstream.cc, ppexsemact;
// tests/regression/preproc_constexp_div_zero). The Go front end
// (src/compiler-go/front/macro/ppex.go) computes the same.
#ifndef ELAN_INTARITH_H
#define ELAN_INTARITH_H

static inline int int_add(int a, int b) { return (int)((unsigned)a + (unsigned)b); }
static inline int int_sub(int a, int b) { return (int)((unsigned)a - (unsigned)b); }
static inline int int_mul(int a, int b) { return (int)((unsigned)a * (unsigned)b); }
static inline int int_neg(int a)        { return (int)(0u - (unsigned)a); }
static inline int int_div(int a, int b)
{
  if (b == 0) return 0;
  if (b == -1) return int_neg(a);
  return a / b;
}
static inline int int_mod(int a, int b)
{
  if (b == 0) return a;
  if (b == -1) return 0;
  return a % b;
}

#endif
