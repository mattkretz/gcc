// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }

#define TESTFIXEDSIZE 1
#include "tests/mask_broadcast.h"

int main()
{
  iterate_abis<wchar_t>();
  return 0;
}
