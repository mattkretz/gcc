// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/mask_operator_cvt.h"

int main()
{
  iterate_abis<char32_t>();
  return 0;
}
