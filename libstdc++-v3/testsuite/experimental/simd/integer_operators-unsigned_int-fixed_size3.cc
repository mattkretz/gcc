// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 3
#include "tests/integer_operators.h"

int main()
{
  iterate_abis<unsigned int>();
  return 0;
}
