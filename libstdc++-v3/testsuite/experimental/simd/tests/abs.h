#include "bits/verify.h"
#include "bits/metahelpers.h"
#include <cmath>    // abs & sqrt
#include <cstdlib>  // integer abs
#include "bits/test_values.h"

template <typename V> void test()
{
  if constexpr (std::is_signed_v<typename V::value_type>)
    {
      using std::abs;
      using T = typename V::value_type;
      using L = std::numeric_limits<T>;
      test_values<V>({L::max(), L::lowest(), L::min(), -L::max() / 2, T(), -T(),
		      T(-1), T(-2)},
		     {100, L::lowest(), L::max()}, [](V input) {
		       const V expected(
			 [&](auto i) { return T(std::abs(T(input[i]))); });
		       COMPARE(abs(input), expected) << "input: " << input;
		     });
    }
}
