// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/mask_conversions.h"

int main()
{
  iterate_abis<signed char>();
  return 0;
}
