// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 4
#include "tests/ldexp_scalbn_scalbln_modf.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
