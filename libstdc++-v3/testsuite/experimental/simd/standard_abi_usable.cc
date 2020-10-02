// { dg-options "-std=c++17 -fno-fast-math" }
// { dg-do compile }

#include <experimental/simd>

template <class T>
  void
  is_usable()
  {
    static_assert(std::is_default_constructible_v<std::experimental::simd<T>>);
    static_assert(std::is_destructible_v<std::experimental::simd<T>>);
    static_assert(
      std::is_default_constructible_v<std::experimental::native_simd<T>>);
    static_assert(std::is_destructible_v<std::experimental::native_simd<T>>);

    static_assert(
      std::is_default_constructible_v<std::experimental::simd_mask<T>>);
    static_assert(std::is_destructible_v<std::experimental::simd_mask<T>>);
    static_assert(
      std::is_default_constructible_v<std::experimental::native_simd_mask<T>>);
    static_assert(
      std::is_destructible_v<std::experimental::native_simd_mask<T>>);
  }

int main()
{
  is_usable<char>();
  is_usable<wchar_t>();
  is_usable<char16_t>();
  is_usable<char32_t>();

  is_usable<signed char>();
  is_usable<unsigned char>();
  is_usable<short>();
  is_usable<unsigned short>();
  is_usable<int>();
  is_usable<unsigned int>();
  is_usable<long>();
  is_usable<unsigned long>();
  is_usable<long long>();
  is_usable<unsigned long long>();
  is_usable<float>();
  is_usable<double>();
  is_usable<long double>();
}
