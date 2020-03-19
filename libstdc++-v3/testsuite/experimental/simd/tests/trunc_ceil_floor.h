// test only floattypes
#include "bits/test_values.h"
#include "bits/verify.h"

template <typename V>
void
test()
{
  using limits = std::numeric_limits<typename V::value_type>;
  test_values<V>(
    {2.1,
     2.0,
     2.9,
     2.5,
     2.499,
     1.5,
     1.499,
     1.99,
     0.99,
     0.5,
     0.499,
     0.,
     -2.1,
     -2.0,
     -2.9,
     -2.5,
     -2.499,
     -1.5,
     -1.499,
     -1.99,
     -0.99,
     -0.5,
     -0.499,
     -0.,
     3 << 21,
     3 << 22,
     3 << 23,
     -(3 << 21),
     -(3 << 22),
     -(3 << 23),
     limits::infinity(),
     -limits::infinity(),
     limits::denorm_min(),
     limits::max(),
     limits::min(),
     limits::min() * 0.9,
     limits::lowest(),
     -limits::denorm_min(),
     -limits::max(),
     -limits::min(),
     -limits::min() * 0.9,
     -limits::lowest()},
    [](const V input) {
      const V expected([&](auto i) { return std::trunc(input[i]); });
      COMPARE(trunc(input), expected) << input;
    },
    [](const V input) {
      const V expected([&](auto i) { return std::ceil(input[i]); });
      COMPARE(ceil(input), expected) << input;
    },
    [](const V input) {
      const V expected([&](auto i) { return std::floor(input[i]); });
      COMPARE(floor(input), expected) << input;
    });

  test_values<V>(
    {limits::quiet_NaN(), limits::signaling_NaN()},
    [](const V input) {
      const V expected([&](auto i) { return std::trunc(input[i]); });
      COMPARE(isnan(trunc(input)), isnan(expected)) << input;
    },
    [](const V input) {
      const V expected([&](auto i) { return std::ceil(input[i]); });
      COMPARE(isnan(ceil(input)), isnan(expected)) << input;
    },
    [](const V input) {
      const V expected([&](auto i) { return std::floor(input[i]); });
      COMPARE(isnan(floor(input)), isnan(expected)) << input;
    });
}
