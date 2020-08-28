// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_reductions.h"

int main()
{
  iterate_abis<short>();
  return 0;
}
