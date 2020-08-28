// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/where.h"

int main()
{
  iterate_abis<unsigned int>();
  return 0;
}
