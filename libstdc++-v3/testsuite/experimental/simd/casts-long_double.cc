// { dg-options "-std=c++17" }

#include "tests/casts.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
