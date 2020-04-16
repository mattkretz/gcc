// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/hypot3_fma.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
