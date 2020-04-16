// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_broadcast.h"

int main()
{
  iterate_abis<short>();
  return 0;
}
