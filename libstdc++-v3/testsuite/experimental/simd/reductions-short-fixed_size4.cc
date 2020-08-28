// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/reductions.h"

int main()
{
  iterate_abis<short>();
  return 0;
}
