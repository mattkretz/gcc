// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
// { dg-do compile }
#include "tests/sincos.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
