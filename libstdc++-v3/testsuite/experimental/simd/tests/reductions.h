#include "bits/verify.h"
#include "bits/metahelpers.h"
#include "bits/test_values.h"
#include <random>

template <typename V>
void
test()
{
  using T = typename V::value_type;
  COMPARE(reduce(V(1)), T(V::size()));
  {
    V x = 1;
    COMPARE(reduce(x, std::multiplies<>()), T(1));
    x[0] = 2;
    COMPARE(reduce(x, std::multiplies<>()), T(2));
    if constexpr (V::size() > 1)
      {
	x[V::size() - 1] = 3;
	COMPARE(reduce(x, std::multiplies<>()), T(6));
      }
  }
  COMPARE(reduce(V([](int i) { return i & 1; })), T(V::size() / 2));
  COMPARE(reduce(V([](int i) { return i % 3; })),
	  T(3 * (V::size() / 3)   // 0+1+2 for every complete 3 elements in V
	    + (V::size() % 3) / 2 // 0->0, 1->0, 2->1 adjustment
	    ));
  if ((1 + V::size()) * V::size() / 2 <= std::__finite_max_v<T>)
    {
      COMPARE(reduce(V([](int i) { return i + 1; })),
	      T((1 + V::size()) * V::size() / 2));
    }

  {
    const V y = 2;
    COMPARE(reduce(y), T(2 * V::size()));
    COMPARE(reduce(where(y > 2, y)), T(0));
    COMPARE(reduce(where(y == 2, y)), T(2 * V::size()));
  }

  {
    const V z([](T i) { return i + 1; });
    COMPARE(std::experimental::reduce(z,
				      [](auto a, auto b) {
					using std::min;
					return min(a, b);
				      }),
	    T(1))
      << "z: " << z;
    COMPARE(std::experimental::reduce(z,
				      [](auto a, auto b) {
					using std::max;
					return max(a, b);
				      }),
	    T(V::size()))
      << "z: " << z;
    COMPARE(std::experimental::reduce(where(z > 1, z), 117,
				      [](auto a, auto b) {
					using std::min;
					return min(a, b);
				      }),
	    T(V::size() == 1 ? 117 : 2))
      << "z: " << z;
  }

  test_values<V>({}, {1000}, [](V x) {
    // avoid over-/underflow on signed integers:
    if constexpr (std::is_signed_v<T> && std::is_integral_v<T>)
      x /= int(V::size());
    // The error in the following could be huge if catastrophic
    // cancellation occurs. (e.g. `a-a+b+b` vs. `a+b+b-a`).
    // Avoid catastrophic cancellation for floating point:
    if constexpr (std::is_floating_point_v<T>)
      x = abs(x);
    T acc = x[0];
    for (size_t i = 1; i < V::size(); ++i)
      acc += x[i];
    ULP_COMPARE(reduce(x), acc, V::size() / 2).on_failure("x = ", x);
  });
}

// vim: foldmethod=marker
