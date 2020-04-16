// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/splits.h"

int main()
{
  iterate_abis<long>();
  return 0;
}
