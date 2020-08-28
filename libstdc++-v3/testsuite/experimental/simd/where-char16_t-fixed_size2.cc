// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/where.h"

int main()
{
  iterate_abis<char16_t>();
  return 0;
}
