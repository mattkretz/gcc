#include "bits/verify.h"
#include "bits/metahelpers.h"
#include <random>

static std::mt19937 g_mt_gen{0};
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
  if ((1 + V::size()) * V::size() / 2 <= std::numeric_limits<T>::max())
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

  {
    std::conditional_t<std::is_floating_point_v<T>,
		       std::uniform_real_distribution<T>,
		       std::uniform_int_distribution<T>>
      dist(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
    for (int repeat = 0; repeat < 100; ++repeat)
      {
	const V x([&](int) { return dist(g_mt_gen); });
	T acc = x[0];
	for (size_t i = 1; i < V::size(); ++i)
	  acc += x[i];
	FUZZY_COMPARE(reduce(x), acc);
      }
  }
}

// vim: foldmethod=marker
