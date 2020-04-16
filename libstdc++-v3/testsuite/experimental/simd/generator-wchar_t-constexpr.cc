// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#include "tests/generator.h"

int main()
{
  iterate_abis<wchar_t>();
  return 0;
}
