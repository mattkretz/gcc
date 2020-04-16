// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/where.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
