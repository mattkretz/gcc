// { dg-do compile { target c++20 } }

#include <concepts>
#include <functional>

template  <typename T>
  struct X
  {
    friend auto operator<=>(const X&, const X&) = default;
  };

static_assert(std::totally_ordered<std::reference_wrapper<X<int>>>);
