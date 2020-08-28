// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
// { dg-skip-if "long double with -ffast-math is irrelevant for simd" { *-*-* }  { "-ffast-math" } { "" } }

#define TESTFIXEDSIZE 3
#include "tests/remqo.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
