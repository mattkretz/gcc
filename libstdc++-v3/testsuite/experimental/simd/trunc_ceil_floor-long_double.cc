// { dg-options "-std=c++17" }

#include "tests/trunc_ceil_floor.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
