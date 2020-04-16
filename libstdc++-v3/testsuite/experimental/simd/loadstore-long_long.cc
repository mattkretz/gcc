// { dg-options "-std=c++17" }

#include "tests/loadstore.h"

int main()
{
  iterate_abis<long long>();
  return 0;
}
