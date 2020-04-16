// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/casts.h"

int main()
{
  iterate_abis<int>();
  return 0;
}
