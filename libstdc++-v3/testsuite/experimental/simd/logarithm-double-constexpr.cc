// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/logarithm.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
