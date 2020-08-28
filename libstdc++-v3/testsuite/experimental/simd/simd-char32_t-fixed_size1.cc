// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/simd.h"

int main()
{
  iterate_abis<char32_t>();
  return 0;
}
