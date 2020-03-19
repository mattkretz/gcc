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
  test_values<V>(
    {limits::quiet_NaN(),
     limits::infinity(),
     -limits::infinity(),
     +0.,
     -0.,
     +1.3,
     -1.3,
     2.1,
     -2.1,
     0.99,
     0.9,
     -0.9,
     -0.99,
     limits::denorm_min(),
     limits::min(),
     limits::max(),
     limits::min() / 3,
     -limits::denorm_min(),
     -limits::min(),
     -limits::max(),
     -limits::min() / 3},
    {10000, -limits::max() / 2, limits::max() / 2},
    [](const V input) {
      for (int exp : {-10000, -100, -10, -1, 0, 1, 10, 100, 10000})
	{
	  const auto totest = ldexp(input, exp);
	  using R = std::remove_const_t<decltype(totest)>;
	  auto&& expected = [&](const auto& v) -> const R {
	    R tmp = {};
	    using std::ldexp;
	    for (std::size_t i = 0; i < R::size(); ++i)
	      {
		tmp[i] = ldexp(v[i], exp);
	      }
	    return tmp;
	  };
	  const R expect1 = expected(input);
	  COMPARE(isnan(totest), isnan(expect1))
	    << "ldexp(" << input << ", " << exp << ") = " << totest
	    << " != " << expect1;
	  FUZZY_COMPARE(ldexp(iif(isnan(expect1), 0, input), exp),
			expected(iif(isnan(expect1), 0, input)))
	    << "\nclean = " << iif(isnan(expect1), 0, input);
	}
    },
    [](const V input) {
      for (int exp : {-10000, -100, -10, -1, 0, 1, 10, 100, 10000})
	{
	  const auto totest = scalbn(input, exp);
	  using R = std::remove_const_t<decltype(totest)>;
	  auto&& expected = [&](const auto& v) -> const R {
	    R tmp = {};
	    using std::scalbn;
	    for (std::size_t i = 0; i < R::size(); ++i)
	      {
		tmp[i] = scalbn(v[i], exp);
	      }
	    return tmp;
	  };
	  const R expect1 = expected(input);
	  COMPARE(isnan(totest), isnan(expect1))
	    << "scalbn(" << input << ", " << exp << ") = " << totest
	    << " != " << expect1;
	  FUZZY_COMPARE(scalbn(iif(isnan(expect1), 0, input), exp),
			expected(iif(isnan(expect1), 0, input)))
	    << "\nclean = " << iif(isnan(expect1), 0, input);
	}
    },
    [](const V input) {
      for (long exp : {-10000, -100, -10, -1, 0, 1, 10, 100, 10000})
	{
	  const auto totest = scalbln(input, exp);
	  using R = std::remove_const_t<decltype(totest)>;
	  auto&& expected = [&](const auto& v) -> const R {
	    R tmp = {};
	    using std::scalbln;
	    for (std::size_t i = 0; i < R::size(); ++i)
	      {
		tmp[i] = scalbln(v[i], exp);
	      }
	    return tmp;
	  };
	  const R expect1 = expected(input);
	  COMPARE(isnan(totest), isnan(expect1))
	    << "scalbln(" << input << ", " << exp << ") = " << totest
	    << " != " << expect1;
	  FUZZY_COMPARE(scalbln(iif(isnan(expect1), 0, input), exp),
			expected(iif(isnan(expect1), 0, input)))
	    << "\nclean = " << iif(isnan(expect1), 0, input);
	}
    },
    [](const V input) {
      V integral = {};
      const V totest = modf(input, &integral);
      auto&& expected = [&](const auto& v) -> std::pair<const V, const V> {
	std::pair<V, V> tmp = {};
	using std::modf;
	for (std::size_t i = 0; i < V::size(); ++i)
	  {
	    typename V::value_type tmp2;
	    tmp.first[i] = modf(v[i], &tmp2);
	    tmp.second[i] = tmp2;
	  }
	return tmp;
      };
      const auto expect1 = expected(input);
      COMPARE(isnan(totest), isnan(expect1.first))
	<< "modf(" << input << ", iptr) = " << totest << " != " << expect1;
      COMPARE(isnan(integral), isnan(expect1.second))
	<< "modf(" << input << ", iptr) = " << totest << " != " << expect1;
      COMPARE(isnan(totest), isnan(integral))
	<< "modf(" << input << ", iptr) = " << totest << " != " << expect1;
      const V clean = iif(isnan(totest), 0, input);
      const auto expect2 = expected(clean);
      COMPARE(modf(clean, &integral), expect2.first) << "\nclean = " << clean;
      COMPARE(integral, expect2.second);
    });
}
