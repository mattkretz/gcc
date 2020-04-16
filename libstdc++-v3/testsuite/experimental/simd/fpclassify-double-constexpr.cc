// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/fpclassify.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
