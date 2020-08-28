// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/mask_conversions.h"

int main()
{
  iterate_abis<char16_t>();
  return 0;
}
