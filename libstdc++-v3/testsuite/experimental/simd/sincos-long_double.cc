// { dg-options "-std=c++17" }
// { dg-do compile }
#include "tests/sincos.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
