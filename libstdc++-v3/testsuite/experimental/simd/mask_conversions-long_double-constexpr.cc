// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }
// { dg-skip-if "long double on ppc64 is not conforming" { powerpc64*-*-* && longdouble128 } }

#include "tests/mask_conversions.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
