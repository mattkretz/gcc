// { dg-options "-std=c++17" }

#include "tests/hypot3_fma.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
