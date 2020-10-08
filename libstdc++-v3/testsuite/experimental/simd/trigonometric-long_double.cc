// { dg-options "-std=c++17" }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }
// { dg-skip-if "long double on ppc64 is not conforming" { powerpc64*-*-* && longdouble128 } }

#include "tests/trigonometric.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
