// { dg-options "-std=c++17" }

#include "tests/loadstore.h"

int main()
{
  iterate_abis<unsigned char>();
  return 0;
}
