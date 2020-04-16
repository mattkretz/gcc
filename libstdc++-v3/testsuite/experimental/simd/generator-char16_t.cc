// { dg-options "-std=c++17" }

#include "tests/generator.h"

int main()
{
  iterate_abis<char16_t>();
  return 0;
}
