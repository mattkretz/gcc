// { dg-options "-std=c++17" }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }
// { dg-skip-if "long double on ppc64 is not conforming" { powerpc64*-*-* && longdouble128 } }
// { dg-do compile }
#include "tests/sincos.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
