// { dg-options "-std=c++17" }
// { dg-do compile }
#include "tests/sincos.h"

int main()
{
  iterate_abis<double>();
  return 0;
}
