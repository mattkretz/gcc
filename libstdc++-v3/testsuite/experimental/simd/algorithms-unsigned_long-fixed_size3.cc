// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 3
#include "tests/algorithms.h"

int main()
{
  iterate_abis<unsigned long>();
  return 0;
}
