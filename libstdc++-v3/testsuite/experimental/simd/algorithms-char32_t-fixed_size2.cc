// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/algorithms.h"

int main()
{
  iterate_abis<char32_t>();
  return 0;
}
