// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/simd.h"

int main()
{
  iterate_abis<short>();
  return 0;
}
