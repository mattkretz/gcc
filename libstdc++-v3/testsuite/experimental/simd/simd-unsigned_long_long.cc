// { dg-options "-std=c++17" }

#include "tests/simd.h"

int main()
{
  iterate_abis<unsigned long long>();
  return 0;
}
