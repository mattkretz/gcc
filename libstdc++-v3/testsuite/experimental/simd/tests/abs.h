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
      test_values<V>({std::__finite_max_v<T>, std::__norm_min_v<T>,
		      -std::__norm_min_v<T>, std::__finite_min_v<T>,
		      std::__finite_min_v<T> / 2, T(), -T(), T(-1), T(-2)},
		     {1000}, [](V input) {
		       const V expected(
			 [&](auto i) { return T(std::abs(T(input[i]))); });
		       COMPARE(abs(input), expected) << "input: " << input;
		     });
    }
}
