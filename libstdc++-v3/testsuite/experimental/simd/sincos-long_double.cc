// { dg-options "-std=c++17" }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }
// { dg-do compile }
#include "tests/sincos.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
