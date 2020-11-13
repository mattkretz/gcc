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

    using T = typename V::value_type;
    test_values<V>(
      {
#ifdef __STDC_IEC_559__
	std::__quiet_NaN_v<T>, std::__infinity_v<T>, -std::__infinity_v<T>, -0.,
	std::__denorm_min_v<T>, std::__norm_min_v<T> / 3,
#endif
	+0., std::__norm_min_v<T>, std::__finite_max_v<T>},
      {10000}, MAKE_TESTER(acos), MAKE_TESTER(tan), MAKE_TESTER(acosh),
      MAKE_TESTER(asinh), MAKE_TESTER(atanh), MAKE_TESTER(cosh),
      MAKE_TESTER(sinh), MAKE_TESTER(tanh));
  }
