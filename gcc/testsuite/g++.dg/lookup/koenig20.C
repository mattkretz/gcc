// { dg-do compile { target c++20 } }

#include <concepts>
#include <iterator>
#include <algorithm>
#include <cassert>

class Incomplete;

template <class T>
  struct Holder
  {
    T x;
  };

using T = Holder<Incomplete>*;

static_assert(std::equality_comparable<T>); // OK
bool x = std::indirectly_comparable<T*, T*, std::equal_to<>>; // hard error
bool y = std::sortable<T*>; // hard error

Holder<Incomplete> *a[10] =  {}; // ten null pointers

void test01() {
  assert(std::count(a, a+10, nullptr) == 10); // OK
  assert(std::ranges::count(a, a+10, nullptr) == 10); // hard error
}
