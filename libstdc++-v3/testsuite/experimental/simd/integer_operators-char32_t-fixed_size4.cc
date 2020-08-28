// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/integer_operators.h"

int main()
{
  iterate_abis<char32_t>();
  return 0;
}
