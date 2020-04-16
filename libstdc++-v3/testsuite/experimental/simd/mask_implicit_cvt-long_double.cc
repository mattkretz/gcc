// { dg-options "-std=c++17" }

#include "tests/mask_implicit_cvt.h"

int main()
{
  iterate_abis<long double>();
  return 0;
}
