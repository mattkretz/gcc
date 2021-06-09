// Copyright (C) 2020-2021 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef ULP_H
#define ULP_H

#include <cmath>
#include <experimental/simd>
#include <type_traits>
#include <cfenv>

namespace vir {
  namespace test {
    template <typename T, typename R = typename T::value_type>
      R
      value_type_impl(int);

    template <typename T>
      T
      value_type_impl(float);

    template <typename T>
      using value_type_t = decltype(value_type_impl<T>(int()));

    template <typename T0, typename T1>
      inline T0
      ulp_distance(T0 val0, const T1& ref1)
      {
        if constexpr (std::is_floating_point_v<value_type_t<T0>>)
          {
	    namespace stdx = std::experimental;
	    using namespace stdx::__float_bitwise_operators;
	    using std::isnan;
            const int fp_exceptions = std::fetestexcept(FE_ALL_EXCEPT);
	    using T = value_type_t<T0>;
	    constexpr int mant_bits = std::__digits_v<T> - 1;
	    constexpr T signexp_mask = -std::__infinity_v<T>;
	    auto&& cast = [](auto x) {
	      using Other = std::conditional_t<std::is_same_v<decltype(x), T0>, T1, T0>;
	      if constexpr (stdx::is_simd_v<decltype(x)>)
		return stdx::static_simd_cast<Other>(x);
	      else
		return static_cast<Other>(x);
	    };
	    T0 ref0 = cast(ref1);
	    T1 val1 = cast(val0);
	    T1 eps1 = std::__epsilon_v<T>;
	    if constexpr (stdx::is_simd_v<T0>)
	      eps1 *= ref1 & T0(signexp_mask);
	    else
	      {
		using I = stdx::__int_for_sizeof_t<value_type_t<T1>>;
		eps1 *= __builtin_bit_cast(T0, __builtin_bit_cast(I, ref1)
						 & __builtin_bit_cast(I, signexp_mask));
	      }
	    const auto subnormal = abs(ref1) < std::__norm_min_v<T>;
	    where(subnormal, eps1) = std::__denorm_min_v<T>;
	    T0 ulp = cast((ref1 - val1) / eps1);
	    where(val0 == ref0 || (isnan(val0) && isnan(ref0)), ulp) = 0;
            std::feclearexcept(FE_ALL_EXCEPT ^ fp_exceptions);
            return ulp;
          }
        else
          {
            if (val0 > ref1)
              return val0 - ref1;
            else
              return ref1 - val0;
          }
      }

    template <typename T0, typename T1>
      inline T0
      ulp_distance_signed(const T0& val, const T1& ref)
      { return ulp_distance(val, ref); }
  } // namespace test
} // namespace vir

#endif // ULP_H
