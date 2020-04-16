// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/integer_operators.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
