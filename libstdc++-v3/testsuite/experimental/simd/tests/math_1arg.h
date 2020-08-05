// test only floattypes
#include "bits/verify.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  vir::test::setFuzzyness<float>(0);
  vir::test::setFuzzyness<double>(0);

  using T = typename V::value_type;
  constexpr T inf = std::__infinity_v<T>;
  constexpr T nan = std::__quiet_NaN_v<T>;
  constexpr T denorm_min = std::__denorm_min_v<T>;
  constexpr T norm_min = std::__norm_min_v<T>;
  constexpr T max = std::__finite_max_v<T>;
  test_values<V>({+0.,
		  0.5,
		  -0.5,
		  1.5,
		  -1.5,
		  2.5,
		  -2.5,
		  0x1.fffffffffffffp52,
		  -0x1.fffffffffffffp52,
		  0x1.ffffffffffffep52,
		  -0x1.ffffffffffffep52,
		  0x1.ffffffffffffdp52,
		  -0x1.ffffffffffffdp52,
		  0x1.fffffep21,
		  -0x1.fffffep21,
		  0x1.fffffcp21,
		  -0x1.fffffcp21,
		  0x1.fffffep22,
		  -0x1.fffffep22,
		  0x1.fffffcp22,
		  -0x1.fffffcp22,
		  0x1.fffffep23,
		  -0x1.fffffep23,
		  0x1.fffffcp23,
		  -0x1.fffffcp23,
		  0x1.8p23,
		  -0x1.8p23,
		  inf,
		  -inf,
		  -0.,
		  nan,
		  denorm_min,
		  norm_min / 3,
		  norm_min,
		  max},
		 {10000},
		 MAKE_TESTER(sqrt), MAKE_TESTER(erf), MAKE_TESTER(erfc),
		 MAKE_TESTER(tgamma), MAKE_TESTER(lgamma), MAKE_TESTER(ceil),
		 MAKE_TESTER(floor), MAKE_TESTER(trunc), MAKE_TESTER(round),
		 MAKE_TESTER(lround), MAKE_TESTER(llround),
		 MAKE_TESTER(nearbyint), MAKE_TESTER(rint), MAKE_TESTER(lrint),
		 MAKE_TESTER(llrint), MAKE_TESTER(ilogb));
}
