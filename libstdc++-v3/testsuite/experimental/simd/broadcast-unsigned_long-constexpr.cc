// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/broadcast.h"

int main()
{
  iterate_abis<unsigned long>();
  return 0;
}
