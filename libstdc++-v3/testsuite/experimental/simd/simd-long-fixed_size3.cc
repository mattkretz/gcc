// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 3
#include "tests/simd.h"

int main()
{
  iterate_abis<long>();
  return 0;
}
