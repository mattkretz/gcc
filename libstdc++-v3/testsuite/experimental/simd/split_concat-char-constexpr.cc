// { dg-options "-std=gnu++17 -DSTRESSTEST=1" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/split_concat.h"

int main()
{
  iterate_abis<char>();
  return 0;
}
