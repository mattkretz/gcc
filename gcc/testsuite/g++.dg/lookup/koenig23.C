// { dg-do compile { target c++11 } }

#include <array>
#include <memory>
#include <vector>

class I;
using T = std::array<I, 3>;
using Ptr = std::unique_ptr<T>;

bool
f (const std::vector<Ptr> &x)
{
  for (const Ptr& ptr : x)
    {
      if (!ptr)
	return false;
    }
  return true;
}
