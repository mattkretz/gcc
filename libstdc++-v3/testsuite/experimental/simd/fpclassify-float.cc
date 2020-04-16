// { dg-options "-std=c++17" }

#include "tests/fpclassify.h"

int main()
{
  iterate_abis<float>();
  return 0;
}
