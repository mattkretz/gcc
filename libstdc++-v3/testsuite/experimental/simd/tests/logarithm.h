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

  using T = typename V::value_type;
  constexpr T nan = std::__quiet_NaN_v<T>;
  constexpr T inf = std::__infinity_v<T>;
  constexpr T denorm_min = std::__denorm_min_v<T>;
  constexpr T norm_min = std::__norm_min_v<T>;
  constexpr T min = std::__finite_min_v<T>;
  constexpr T max = std::__finite_max_v<T>;
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
#ifdef __STDC_IEC_559__
		  nan,
		  inf,
		  -inf,
		  denorm_min,
		  -denorm_min,
		  norm_min / 3,
		  -norm_min / 3,
		  -T(),
		  -norm_min,
		  min,
		  T(),
#endif
		  norm_min,
		  max},
		 {10000,
#ifdef __STDC_IEC_559__
		  min / 2,
#else
		  norm_min,
#endif
		  max / 2},
		 MAKE_TESTER(log), MAKE_TESTER(log10), MAKE_TESTER(log1p),
		 MAKE_TESTER(log2), MAKE_TESTER(logb));
}
