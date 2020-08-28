// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/split_concat.h"

int main()
{
  iterate_abis<unsigned int>();
  return 0;
}
