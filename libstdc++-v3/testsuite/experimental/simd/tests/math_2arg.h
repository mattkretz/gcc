// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  using T = typename V::value_type;
  using limits = std::numeric_limits<T>;

  vir::test::setFuzzyness<float>(1);
  vir::test::setFuzzyness<double>(1);
  vir::test::setFuzzyness<long double>(1);
  test_values_2arg<V>(
    {
#ifdef __STDC_IEC_559__
      limits::quiet_NaN(), limits::infinity(), -limits::infinity(), -0.,
      limits::denorm_min(), limits::min() / 3,
#endif
      +0., limits::min(), limits::max()},
    {100000, -limits::max() / 2, limits::max() / 2}, MAKE_TESTER(hypot));
  COMPARE(hypot(V(limits::max()), V(limits::max())), V(limits::infinity()));
  COMPARE(hypot(V(limits::min()), V(limits::min())),
	  V(limits::min() * std::sqrt(T(2))));
  VERIFY((sfinae_is_callable<V, V>(
    [](auto a, auto b) -> decltype(hypot(a, b)) { return {}; })));
  VERIFY((sfinae_is_callable<typename V::value_type, V>(
    [](auto a, auto b) -> decltype(hypot(a, b)) { return {}; })));
  VERIFY((sfinae_is_callable<V, typename V::value_type>(
    [](auto a, auto b) -> decltype(hypot(a, b)) { return {}; })));

  vir::test::setFuzzyness<float>(0);
  vir::test::setFuzzyness<double>(0);
  vir::test::setFuzzyness<long double>(0);
  test_values_2arg<V>(
    {
#ifdef __STDC_IEC_559__
      limits::quiet_NaN(), limits::infinity(), -limits::infinity(),
      limits::denorm_min(), limits::min() / 3, -0.,
#endif
      +0., limits::min(), limits::max()},
    {10000, -limits::max() / 2, limits::max() / 2}, MAKE_TESTER(pow),
    MAKE_TESTER(fmod), MAKE_TESTER(remainder), MAKE_TESTER_NOFPEXCEPT(copysign),
    MAKE_TESTER(nextafter), // MAKE_TESTER(nexttoward),
    MAKE_TESTER(fdim), MAKE_TESTER(fmax), MAKE_TESTER(fmin),
    MAKE_TESTER_NOFPEXCEPT(isgreater), MAKE_TESTER_NOFPEXCEPT(isgreaterequal),
    MAKE_TESTER_NOFPEXCEPT(isless), MAKE_TESTER_NOFPEXCEPT(islessequal),
    MAKE_TESTER_NOFPEXCEPT(islessgreater), MAKE_TESTER_NOFPEXCEPT(isunordered));
}

// vim: ts=8 et sw=2 sts=2
