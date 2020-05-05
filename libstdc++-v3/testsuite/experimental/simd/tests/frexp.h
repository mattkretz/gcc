// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  using int_v = std::experimental::fixed_size_simd<int, V::size()>;
  using limits = std::numeric_limits<typename V::value_type>;
  test_values<V>(
    {
      0, 0.25, 0.5, 1, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 32, 31, -0., -0.25, -0.5, -1,
	-3, -4, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18,
	-19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, -32, -31,
#if __GCC_IEC_559 >= 2
	limits::denorm_min(), -limits::denorm_min(), limits::min() / 2,
	-limits::min() / 2,
#endif
	limits::max(), -limits::max(), limits::max() * 0.123f,
	-limits::max() * 0.123f
    },
    [](const V input) {
      V expectedFraction;
      const int_v expectedExponent([&](auto i) {
	int exp;
	expectedFraction[i] = std::frexp(input[i], &exp);
	return exp;
      });
      int_v exponent = {};
      const V fraction = frexp(input, &exponent);
      COMPARE(fraction, expectedFraction)
	<< ", input = " << input << ", delta: " << fraction - expectedFraction;
      COMPARE(exponent, expectedExponent)
	<< "\ninput: " << input << ", fraction: " << fraction;
    });
#ifdef __STDC_IEC_559__
  test_values<V>(
    // If x is a NaN, a NaN is returned, and the value of *exp is unspecified.
    //
    // If x is positive  infinity  (negative  infinity),  positive  infinity
    // (negative infinity) is returned, and the value of *exp is unspecified.
    // This behavior is only guaranteed with C's Annex F when __STDC_IEC_559__
    // is defined.
    {limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     limits::denorm_min(),
     limits::denorm_min() * 1.72,
     -limits::denorm_min(),
     -limits::denorm_min() * 1.72,
     0.,
     -0.,
     1,
     -1},
    [](const V input) {
      const V expectedFraction([&](auto i) {
	int exp;
	return std::frexp(input[i], &exp);
      });
      int_v exponent = {};
      const V fraction = frexp(input, &exponent);
      COMPARE(isnan(fraction), isnan(expectedFraction))
	<< fraction << ", input = " << input
	<< ", delta: " << fraction - expectedFraction;
      COMPARE(isinf(fraction), isinf(expectedFraction))
	<< fraction << ", input = " << input
	<< ", delta: " << fraction - expectedFraction;
      COMPARE(signbit(fraction), signbit(expectedFraction))
	<< fraction << ", input = " << input
	<< ", delta: " << fraction - expectedFraction;
    });
#endif
}
