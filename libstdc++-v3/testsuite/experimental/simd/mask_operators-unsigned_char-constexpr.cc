// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_operators.h"

int main()
{
  iterate_abis<unsigned char>();
  return 0;
}
