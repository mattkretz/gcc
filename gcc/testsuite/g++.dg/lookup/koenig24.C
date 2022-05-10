// { dg-do compile { target c++11 } }

#include <functional>

template <typename T>
  class Numeric
  {
    T value;
  public:
    constexpr Numeric() = default;
    constexpr Numeric(const Numeric&) = default;

    friend constexpr bool
    operator<(Numeric a, Numeric b)
    { return a.value < b.value; }
  };

auto f(std::reference_wrapper<Numeric<float>> x) {
  return x < x;
}
