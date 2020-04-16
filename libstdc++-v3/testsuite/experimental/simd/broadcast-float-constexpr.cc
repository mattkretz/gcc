// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/broadcast.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
