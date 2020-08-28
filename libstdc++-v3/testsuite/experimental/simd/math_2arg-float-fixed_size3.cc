// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 3
#include "tests/math_2arg.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
