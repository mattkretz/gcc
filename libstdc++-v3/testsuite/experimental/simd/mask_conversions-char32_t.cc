// { dg-options "-std=c++17" }

#include "tests/mask_conversions.h"

int main()
{
  iterate_abis<char32_t>();
  return 0;
}
