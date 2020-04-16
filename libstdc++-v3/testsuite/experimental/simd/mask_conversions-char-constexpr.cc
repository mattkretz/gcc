// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_conversions.h"

int main()
{
  iterate_abis<char>();
  return 0;
}
