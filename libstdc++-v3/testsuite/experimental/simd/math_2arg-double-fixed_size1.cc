// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/math_2arg.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
