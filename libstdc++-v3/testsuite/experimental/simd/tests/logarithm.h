// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/mathreference.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  vir::test::setFuzzyness<float>(1);
  vir::test::setFuzzyness<double>(1);

  using limits = std::numeric_limits<typename V::value_type>;
  test_values<V>({1,
		  2,
		  4,
		  8,
		  16,
		  32,
		  64,
		  128,
		  256,
		  512,
		  1024,
		  2048,
		  3,
		  5,
		  7,
		  15,
		  17,
		  31,
		  33,
		  63,
		  65,
		  limits::quiet_NaN(),
		  limits::infinity(),
		  -limits::infinity(),
		  +0.,
		  -0.,
		  limits::denorm_min(),
		  limits::min(),
		  limits::max(),
		  limits::min() / 3,
		  -limits::denorm_min(),
		  -limits::min(),
		  -limits::max(),
		  -limits::min() / 3},
		 {10000, -limits::max() / 2, limits::max() / 2},
		 MAKE_TESTER(log), MAKE_TESTER(log10), MAKE_TESTER(log1p),
		 MAKE_TESTER(log2), MAKE_TESTER(logb));
}
