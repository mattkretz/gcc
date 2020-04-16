// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_implicit_cvt.h"

int main()
{
  iterate_abis<unsigned short>();
  return 0;
}
