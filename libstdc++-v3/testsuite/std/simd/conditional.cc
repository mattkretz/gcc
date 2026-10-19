// { dg-do run { target c++26 } }
// { dg-require-effective-target x86 }
// { dg-additional-options "-msse2 -foverloadable-conditional" }

// [simd.cond] operator?: (P0917): the hidden friends mirror the __select_impl
// friends — k ? t : f must select exactly what simd::select(k, t, f) selects.

#include "test_setup.h"

template <typename V>
  struct Tests
  {
    using T = typename V::value_type;
    using M = typename V::mask_type;

    static constexpr M alternating = M([](int i) { return 1 == (i & 1); });

    ADD_TEST(vec) {
      std::tuple {alternating, M(true), M(false)},
      [](auto& t, M k, M all, M none) {
	auto a = V(T(1));
	auto b = V(T(2));
	t.verify_equal(k ? a : b, simd::select(k, a, b));
	t.verify_equal(all ? a : b, simd::select(all, a, b));
	t.verify_equal(none ? a : b, simd::select(none, a, b));
      }
    };

    ADD_TEST(mask) {
      std::tuple {alternating, M(true), M(false)},
      [](auto& t, M k, M all, M none) {
	auto tr = M(true);
	auto fa = M(false);
	t.verify_equal(k ? tr : fa, simd::select(k, tr, fa));
	t.verify_equal(all ? tr : fa, simd::select(all, tr, fa));
	t.verify_equal(none ? tr : fa, simd::select(none, tr, fa));
      }
    };

    ADD_TEST(hetero) {
      std::tuple {alternating, M(true), M(false)},
      [](auto& t, M k, M all, M none) {
	auto a = V(T(1));
	T s = T(2);
	t.verify_equal(k ? a : s, simd::select(k, a, s));
	t.verify_equal(all ? a : s, simd::select(all, a, s));
	t.verify_equal(none ? a : s, simd::select(none, a, s));
      }
    };
  };

#include "create_tests.h"
