// { dg-options "-std=c++17" }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }

#include "tests/mask_operator_cvt.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
