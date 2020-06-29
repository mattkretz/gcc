// test only floattypes
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  vir::test::setFuzzyness<float>(0);
  vir::test::setFuzzyness<double>(0);

  using limits = std::numeric_limits<typename V::value_type>;
  test_values_2arg<V>(
    {
#ifdef __STDC_IEC_559__
      limits::quiet_NaN(), limits::infinity(), -limits::infinity(),
      limits::denorm_min(), limits::min() / 3, -0.,
#endif
      +0., limits::min(), limits::max()},
    {10000}, [](V a, V b) {
      if constexpr (!limits::is_iec559)
	where(b == 0, b) = 1;
      using IV = std::experimental::fixed_size_simd<int, V::size()>;
      IV quo = {};
      const V totest = remquo(a, b, &quo);
      auto&& expected
	= [&](const auto& v, const auto& w) -> std::pair<const V, const IV> {
	std::pair<V, IV> tmp = {};
	using std::remquo;
	for (std::size_t i = 0; i < V::size(); ++i)
	  {
	    int tmp2;
	    tmp.first[i] = remquo(v[i], w[i], &tmp2);
	    tmp.second[i] = tmp2;
	  }
	return tmp;
      };
      const auto expect1 = expected(a, b);
      COMPARE(isnan(totest), isnan(expect1.first))
	<< "remquo(" << a << ", " << b << ", quo) = " << totest
	<< " != " << expect1.first;
      const V clean_a = iif(isnan(totest), 0, a);
      const V clean_b = iif(isnan(totest), 1, b);
      const auto expect2 = expected(clean_a, clean_b);
      COMPARE(remquo(clean_a, clean_b, &quo), expect2.first)
	<< "\nclean_a/b = " << clean_a << ", " << clean_b;
      COMPARE(quo, expect2.second);
    });
}
