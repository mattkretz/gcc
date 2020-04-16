// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
// { dg-do compile }
#define TESTFIXEDSIZE 1
#include "tests/sincos.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
