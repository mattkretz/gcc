// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 2
#include "tests/generator.h"

int main()
{
  iterate_abis<unsigned int>();
  return 0;
}
