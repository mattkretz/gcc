// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 3
#include "tests/hypot3_fma.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
