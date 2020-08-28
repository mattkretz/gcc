// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/loadstore.h"

int main()
{
  iterate_abis<char16_t>();
  return 0;
}
