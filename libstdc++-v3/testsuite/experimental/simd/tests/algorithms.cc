#include "bits/verify.h"
#include "bits/metahelpers.h"

template <typename V>
  void
  test()
  {
    using T = typename V::value_type;
    V a{[](auto i) -> T { return i & 1u; }};
    V b{[](auto i) -> T { return (i + 1u) & 1u; }};
    COMPARE(min(a, b), V{0});
    COMPARE(max(a, b), V{1});
  }
