// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/hypot3_fma.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
