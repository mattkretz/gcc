// { dg-options "-std=c++17" }

#include "tests/mask_loadstore.h"

int main()
{
  iterate_abis<signed char>();
  return 0;
}
