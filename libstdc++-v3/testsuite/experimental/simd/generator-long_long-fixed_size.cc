// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/generator.h"

int main()
{
  iterate_abis<long long>();
  return 0;
}
