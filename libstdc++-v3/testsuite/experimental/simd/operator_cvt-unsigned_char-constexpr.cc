// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/operator_cvt.h"

int main()
{
  iterate_abis<unsigned char>();
  return 0;
}
