// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/trigonometric.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
