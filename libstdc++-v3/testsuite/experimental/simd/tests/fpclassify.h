// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"
#include <cfenv>

template <typename F>
auto
verify_no_fp_exceptions(F&& fun)
{
  std::feclearexcept(FE_ALL_EXCEPT);
  auto r = fun();
  COMPARE(std::fetestexcept(FE_ALL_EXCEPT), 0);
  return r;
}

#define NOFPEXCEPT(...) verify_no_fp_exceptions([&]() { return __VA_ARGS__; })

template <typename V>
void
test()
{
  using limits = std::numeric_limits<typename V::value_type>;
  test_values<V>(
    {
      0., 1., -1.,
#if __GCC_IEC_559 >= 2
	-0., limits::infinity(), -limits::infinity(), limits::denorm_min(),
	-limits::denorm_min(), limits::quiet_NaN(),
#ifdef __SUPPORT_SNAN__
	limits::signaling_NaN(),
#endif
#endif
	limits::max(), -limits::max(), limits::min(), limits::min() * 0.9,
	-limits::min(), -limits::min() * 0.9
    },
    [](const V input) {
      using intv = std::experimental::fixed_size_simd<int, V::size()>;
      COMPARE(NOFPEXCEPT(isfinite(input)),
	      !V([&](auto i) { return std::isfinite(input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(isinf(input)),
	      !V([&](auto i) { return std::isinf(input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(isnan(input)),
	      !V([&](auto i) { return std::isnan(input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(isnormal(input)),
	      !V([&](auto i) { return std::isnormal(input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(signbit(input)),
	      !V([&](auto i) { return std::signbit(input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(isunordered(input, V())),
	      !V([&](auto i) { return std::isunordered(input[i], 0) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(isunordered(V(), input)),
	      !V([&](auto i) { return std::isunordered(0, input[i]) ? 0 : 1; }))
	<< input;
      COMPARE(NOFPEXCEPT(fpclassify(input)),
	      intv([&](auto i) { return std::fpclassify(input[i]); }))
	<< input;
    });
}
