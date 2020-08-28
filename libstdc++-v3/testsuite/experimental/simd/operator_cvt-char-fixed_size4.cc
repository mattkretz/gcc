// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/operator_cvt.h"

int main()
{
  iterate_abis<char>();
  return 0;
}
