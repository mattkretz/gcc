#include "bits/verify.h"

template <typename V>
void
test()
{
  using T = typename V::value_type;

  // V must store V::size() values of type T giving us the lower bound on the
  // sizeof
  VERIFY(sizeof(V) >= sizeof(T) * V::size());

  // V should not pad more than to the next-power-of-2 of V::size() values of
  // type T giving us the upper bound on the sizeof
  auto n = V::size();
  n = ((n << 1) & ~n) & ~((n >> 1) | (n >> 3));
  while (n & (n - 1))
    {
      n &= n - 1;
    }
  VERIFY(sizeof(V) <= sizeof(T) * n);
}
