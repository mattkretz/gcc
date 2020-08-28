// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
// { dg-do compile }
#define TESTFIXEDSIZE 4
#include "tests/sincos.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
