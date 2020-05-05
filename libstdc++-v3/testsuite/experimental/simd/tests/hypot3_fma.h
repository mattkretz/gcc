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
  vir::test::setFuzzyness<long double>(2); // because of the bad reference

  using limits = std::numeric_limits<T>;
  // 3-arg std::hypot needs to be fixed, this is a better reference:
  auto&& hypot3 = [](T x, T y, T z) -> T {
    x = std::abs(x);
    y = std::abs(y);
    z = std::abs(z);
    if (std::isinf(x) || std::isinf(y) || std::isinf(z))
      {
	return limits::infinity();
      }
    else if (std::isnan(x) || std::isnan(y) || std::isnan(z))
      {
	return limits::quiet_NaN();
      }
    else if (x == y && y == z)
      {
	return x * std::sqrt(T(3));
      }
    else if (z == 0 && y == 0)
      return x;
    else if (x == 0 && z == 0)
      return y;
    else if (x == 0 && y == 0)
      return z;
    else if (x == 0)
      return std::hypot(y, z);
    else if (y == 0)
      return std::hypot(x, z);
    else if (z == 0)
      return std::hypot(x, y);
    else
      {
	long double hi = std::max(std::max(x, y), z);
	long double lo0 = std::min(std::max(x, y), z);
	long double lo1 = std::min(x, y);
	if (std::isinf(x * x + y * y + z * z) || 0 == (lo0 * lo0 + lo1 * lo1))
	  {
	    lo0 /= hi;
	    lo1 /= hi;
	    return std::abs(hi) * std::sqrt(1 + (lo0 * lo0 + lo1 * lo1));
	  }
	else
	  {
	    return std::sqrt(hi * hi + (lo0 * lo0 + lo1 * lo1));
	  }
      }
  };
  test_values_3arg<V>(
    {
#ifdef __STDC_IEC_559__
      limits::quiet_NaN(), limits::infinity(), -limits::infinity(),
      limits::min() / 3, -0., limits::denorm_min(),
#endif
      0., 1., -1., limits::min(), limits::max(), -limits::max()},
    {100000, -limits::max(), limits::max()}, MAKE_TESTER_2(hypot, hypot3));
  COMPARE(hypot(V(limits::max()), V(limits::max()), V()),
	  V(limits::infinity()));
  COMPARE(hypot(V(limits::max()), V(), V(limits::max())),
	  V(limits::infinity()));
  COMPARE(hypot(V(), V(limits::max()), V(limits::max())),
	  V(limits::infinity()));
  COMPARE(hypot(V(limits::min()), V(limits::min()), V(limits::min())),
	  V(limits::min() * std::sqrt(T(3))));
  VERIFY((sfinae_is_callable<V, V, V>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, T, V>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, T>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, V, T>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, V, V>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, V>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, V, T>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<int, int, V>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<int, V, int>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, int>(
    [](auto a, auto b, auto c) -> decltype(hypot(a, b, c)) { return {}; })));

  vir::test::setFuzzyness<float>(0);
  vir::test::setFuzzyness<double>(0);
  test_values_3arg<V>(
    {
#ifdef __STDC_IEC_559__
      limits::quiet_NaN(), limits::infinity(), -limits::infinity(), -0.,
      limits::min() / 3, limits::denorm_min(),
#endif
      0., limits::min(), limits::max()},
    {10000, -limits::max() / 2, limits::max() / 2}, MAKE_TESTER(fma));
  VERIFY((sfinae_is_callable<V, V, V>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, T, V>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, T>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, V, T>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<T, V, V>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, V>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, V, T>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<int, int, V>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<int, V, int>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
  VERIFY((sfinae_is_callable<V, T, int>(
    [](auto a, auto b, auto c) -> decltype(fma(a, b, c)) { return {}; })));
}

// vim: ts=8 noet sw=2 sts=2
