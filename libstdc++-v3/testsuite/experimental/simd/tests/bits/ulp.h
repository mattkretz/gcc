/*{
Copyright © 2011-2018 Matthias Kretz <kretz@kde.org>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of contributing organizations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

}*/

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

    template <typename T>
      inline T
      ulp_distance(const T& val_, const T& ref_)
      {
        if constexpr (std::is_floating_point_v<value_type_t<T>>)
          {
            const int fp_exceptions = std::fetestexcept(FE_ALL_EXCEPT);
            T val = val_;
            T ref = ref_;

            T diff = T();

            using std::abs;
            using std::fpclassify;
            using std::frexp;
            using std::isnan;
            using std::isinf;
            using std::ldexp;
            using std::max;
            using std::experimental::where;
            using TT = value_type_t<T>;

            where(ref == 0, val) = abs(val);
            where(ref == 0, diff) = 1;
            where(ref == 0, ref) = std::__norm_min_v<TT>;
            where(isinf(ref) && ref == val, ref)
              = 0; // where(val_ == ref_) = 0 below will fix it up

            where(val == 0, ref) = abs(ref);
            where(val == 0, diff) += 1;
            where(val == 0, val) = std::__norm_min_v<TT>;

            using I = decltype(fpclassify(std::declval<T>()));
            I exp = {};
            frexp(ref, &exp);
            // lower bound for exp must be min_exponent to scale the resulting
            // difference from a denormal correctly
            exp = max(exp, I(std::__min_exponent_v<TT>));
            diff += ldexp(abs(ref - val), std::__digits_v<TT> - exp);
            where(val_ == ref_ || (isnan(val_) && isnan(ref_)), diff) = T();
            std::feclearexcept(FE_ALL_EXCEPT ^ fp_exceptions);
            return diff;
          }
        else
          {
            if (val_ > ref_)
              return val_ - ref_;
            else
              return ref_ - val_;
          }
      }

    template <typename T>
      inline T
      ulp_distance_signed(const T& _val, const T& _ref)
      {
        using std::copysign;
        return copysign(ulp_distance(_val, _ref), _val - _ref);
      }
  } // namespace test
} // namespace vir

#endif // ULP_H
