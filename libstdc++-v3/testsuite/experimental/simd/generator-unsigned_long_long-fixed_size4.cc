// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/generator.h"

int main()
{
  iterate_abis<unsigned long long>();
  return 0;
}
