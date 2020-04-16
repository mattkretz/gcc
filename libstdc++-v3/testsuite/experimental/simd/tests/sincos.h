// test only floattypes
// { dg-additional-files "reference-sincos-sp.dat" }
// { dg-additional-files "reference-sincos-ep.dat" }
#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/mathreference.h"
#include "bits/simd_view.h"
#include "bits/test_values.h"

template <typename V>
void
test()
{
  using std::cos;
  using std::sin;
  using T = typename V::value_type;

  vir::test::setFuzzyness<float>(2);
  vir::test::setFuzzyness<double>(1);

  const auto& testdata = referenceData<function::sincos, T>();
  std::experimental::experimental::simd_view<V>(testdata).for_each(
    [&](const V input, const V expected_sin, const V expected_cos) {
      FUZZY_COMPARE(sin(input), expected_sin) << " input = " << input;
      FUZZY_COMPARE(sin(-input), -expected_sin) << " input = " << input;
      FUZZY_COMPARE(cos(input), expected_cos) << " input = " << input;
      FUZZY_COMPARE(cos(-input), expected_cos) << " input = " << input;
    });
}

// vim: sw=2 sts=2 noet ts=8
