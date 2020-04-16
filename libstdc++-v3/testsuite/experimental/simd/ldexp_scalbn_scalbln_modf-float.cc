// { dg-options "-std=c++17" }

#include "tests/ldexp_scalbn_scalbln_modf.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
