#include "bits/verify.h"

template <typename V>
  void
  test()
  {
    using M = typename V::mask_type;
    using namespace std::experimental::parallelism_v2;
    using T = typename V::value_type;
    if constexpr (V::size() / simd_size_v<T> * simd_size_v<T> == V::size())
      {
	M k(true);
	VERIFY(all_of(k)) << k;
	const auto parts = split<simd_mask<T>>(k);
	for (auto k2 : parts)
	  {
	    VERIFY(all_of(k2)) << k2;
	    COMPARE(typeid(k2), typeid(simd_mask<T>));
	  }
      }
  }
