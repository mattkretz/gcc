// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  vir::test::setFuzzyness<float>(1);
  vir::test::setFuzzyness<double>(1);

  using limits = std::numeric_limits<typename V::value_type>;
  test_values<V>({limits::quiet_NaN(), limits::infinity(), -limits::infinity(),
		  +0., -0., limits::denorm_min(), limits::min(), limits::max(),
		  limits::min() / 3},
		 {10000, -limits::max() / 2, limits::max() / 2},
		 MAKE_TESTER(acos), MAKE_TESTER(tan), MAKE_TESTER(acosh),
		 MAKE_TESTER(asinh), MAKE_TESTER(atanh), MAKE_TESTER(cosh),
		 MAKE_TESTER(sinh), MAKE_TESTER(tanh));
}
