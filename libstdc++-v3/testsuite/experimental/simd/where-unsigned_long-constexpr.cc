// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/where.h"

int main()
{
  iterate_abis<unsigned long>();
  return 0;
}
