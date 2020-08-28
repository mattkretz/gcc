// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/operator_cvt.h"

int main()
{
  iterate_abis<signed char>();
  return 0;
}
