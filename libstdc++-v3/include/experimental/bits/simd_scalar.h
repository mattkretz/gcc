// Simd scalar ABI specific implementations -*- C++ -*-

// Copyright (C) 2020 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef _GLIBCXX_EXPERIMENTAL_SIMD_SCALAR_H_
#define _GLIBCXX_EXPERIMENTAL_SIMD_SCALAR_H_
#if __cplusplus >= 201703L

#include <cmath>

_GLIBCXX_SIMD_BEGIN_NAMESPACE

// __promote_preserving_unsigned{{{
// work around crazy semantics of unsigned integers of lower rank than int:
// Before applying an operator the operands are promoted to int. In which case
// over- or underflow is UB, even though the operand types were unsigned.
template <typename _Tp>
_GLIBCXX_SIMD_INTRINSIC constexpr decltype(auto)
__promote_preserving_unsigned(const _Tp& __x)
{
  if constexpr (std::is_signed_v<decltype(+__x)> && std::is_unsigned_v<_Tp>)
    return static_cast<unsigned int>(__x);
  else
    return __x;
}

// }}}

struct _CommonImplScalar;
struct _CommonImplBuiltin;
struct _SimdImplScalar;
struct _MaskImplScalar;
// simd_abi::_Scalar {{{
struct simd_abi::_Scalar
{
  template <typename _Tp> static constexpr size_t size = 1;
  template <typename _Tp> static constexpr size_t _S_full_size = 1;
  template <typename _Tp> static constexpr bool _S_is_partial = false;
  struct _IsValidAbiTag : true_type
  {
  };
  template <typename _Tp> struct _IsValidSizeFor : true_type
  {
  };
  template <typename _Tp> struct _IsValid : __is_vectorizable<_Tp>
  {
  };
  template <typename _Tp>
  static constexpr bool _S_is_valid_v = _IsValid<_Tp>::value;

  _GLIBCXX_SIMD_INTRINSIC static constexpr bool __masked(bool __x)
  {
    return __x;
  }

  using _CommonImpl = _CommonImplScalar;
  using _SimdImpl = _SimdImplScalar;
  using _MaskImpl = _MaskImplScalar;

  template <typename _Tp, bool = _S_is_valid_v<_Tp>>
  struct __traits : _InvalidTraits
  {
  };

  template <typename _Tp> struct __traits<_Tp, true>
  {
    using _IsValid = true_type;
    using _SimdImpl = _SimdImplScalar;
    using _MaskImpl = _MaskImplScalar;
    using _SimdMember = _Tp;
    using _MaskMember = bool;
    static constexpr size_t _S_simd_align = alignof(_SimdMember);
    static constexpr size_t _S_mask_align = alignof(_MaskMember);

    // nothing the user can spell converts to/from simd/simd_mask
    struct _SimdCastType
    {
      _SimdCastType() = delete;
    };
    struct _MaskCastType
    {
      _MaskCastType() = delete;
    };
    struct _SimdBase
    {
    };
    struct _MaskBase
    {
    };
  };
};
// }}}
// _CommonImplScalar {{{
struct _CommonImplScalar
{
  // __store {{{
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static void __store(_Tp __x, void* __addr)
  {
    __builtin_memcpy(__addr, &__x, sizeof(_Tp));
  }

  // }}}
  // __store_bool_array(_BitMask) {{{
  template <size_t _Np, bool _Sanitized>
  _GLIBCXX_SIMD_INTRINSIC static constexpr void
  __store_bool_array(_BitMask<_Np, _Sanitized> __x, bool* __mem)
  {
    __make_dependent_t<decltype(__x), _CommonImplBuiltin>::__store_bool_array(
      __x, __mem);
  }

  // }}}
};

// }}}
// _SimdImplScalar {{{
struct _SimdImplScalar
{
  // member types {{{2
  using abi_type = simd_abi::scalar;
  template <typename _Tp> using _TypeTag = _Tp*;

  // broadcast {{{2
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static constexpr _Tp __broadcast(_Tp __x) noexcept
  {
    return __x;
  }

  // __generator {{{2
  template <typename _Fp, typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static constexpr _Tp __generator(_Fp&& __gen,
							   _TypeTag<_Tp>)
  {
    return __gen(_SizeConstant<0>());
  }

  // __load {{{2
  template <typename _Tp, typename _Up>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __load(const _Up* __mem,
					    _TypeTag<_Tp>) noexcept
  {
    return static_cast<_Tp>(__mem[0]);
  }

  // __masked_load {{{2
  template <typename _Tp, typename _Up>
  static inline _Tp __masked_load(_Tp __merge, bool __k,
				  const _Up* __mem) noexcept
  {
    if (__k)
      __merge = static_cast<_Tp>(__mem[0]);
    return __merge;
  }

  // __store {{{2
  template <typename _Tp, typename _Up>
  static inline void __store(_Tp __v, _Up* __mem, _TypeTag<_Tp>) noexcept
  {
    __mem[0] = static_cast<_Tp>(__v);
  }

  // __masked_store {{{2
  template <typename _Tp, typename _Up>
  static inline void __masked_store(const _Tp __v, _Up* __mem,
				    const bool __k) noexcept
  {
    if (__k)
      __mem[0] = __v;
  }

  // __negate {{{2
  template <typename _Tp>
  static constexpr inline bool __negate(_Tp __x) noexcept
  {
    return !__x;
  }

  // __reduce {{{2
  template <typename _Tp, typename _BinaryOperation>
  static constexpr inline _Tp __reduce(const simd<_Tp, simd_abi::scalar>& __x,
				       _BinaryOperation&)
  {
    return __x._M_data;
  }

  // __min, __max {{{2
  template <typename _Tp>
  static constexpr inline _Tp __min(const _Tp __a, const _Tp __b)
  {
    return std::min(__a, __b);
  }

  template <typename _Tp>
  static constexpr inline _Tp __max(const _Tp __a, const _Tp __b)
  {
    return std::max(__a, __b);
  }

  // __complement {{{2
  template <typename _Tp>
  static constexpr inline _Tp __complement(_Tp __x) noexcept
  {
    return static_cast<_Tp>(~__x);
  }

  // __unary_minus {{{2
  template <typename _Tp>
  static constexpr inline _Tp __unary_minus(_Tp __x) noexcept
  {
    return static_cast<_Tp>(-__x);
  }

  // arithmetic operators {{{2
  template <typename _Tp> static constexpr inline _Tp __plus(_Tp __x, _Tp __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			    + __promote_preserving_unsigned(__y));
  }

  template <typename _Tp> static constexpr inline _Tp __minus(_Tp __x, _Tp __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			    - __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __multiplies(_Tp __x, _Tp __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			    * __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __divides(_Tp __x, _Tp __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			    / __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __modulus(_Tp __x, _Tp __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			    % __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __bit_and(_Tp __x, _Tp __y)
  {
    if constexpr (is_floating_point_v<_Tp>)
      {
	using _Ip = __int_for_sizeof_t<_Tp>;
	return __bit_cast<_Tp>(__bit_cast<_Ip>(__x) & __bit_cast<_Ip>(__y));
      }
    else
      return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			      & __promote_preserving_unsigned(__y));
  }

  template <typename _Tp> static constexpr inline _Tp __bit_or(_Tp __x, _Tp __y)
  {
    if constexpr (is_floating_point_v<_Tp>)
      {
	using _Ip = __int_for_sizeof_t<_Tp>;
	return __bit_cast<_Tp>(__bit_cast<_Ip>(__x) | __bit_cast<_Ip>(__y));
      }
    else
      return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			      | __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __bit_xor(_Tp __x, _Tp __y)
  {
    if constexpr (is_floating_point_v<_Tp>)
      {
	using _Ip = __int_for_sizeof_t<_Tp>;
	return __bit_cast<_Tp>(__bit_cast<_Ip>(__x) ^ __bit_cast<_Ip>(__y));
      }
    else
      return static_cast<_Tp>(__promote_preserving_unsigned(__x)
			      ^ __promote_preserving_unsigned(__y));
  }

  template <typename _Tp>
  static constexpr inline _Tp __bit_shift_left(_Tp __x, int __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x) << __y);
  }

  template <typename _Tp>
  static constexpr inline _Tp __bit_shift_right(_Tp __x, int __y)
  {
    return static_cast<_Tp>(__promote_preserving_unsigned(__x) >> __y);
  }

  // math {{{2
  // frexp, modf and copysign implemented in simd_math.h
  template <typename _Tp> using _ST = _SimdTuple<_Tp, simd_abi::scalar>;

  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __acos(_Tp __x)
  {
    return std::acos(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __asin(_Tp __x)
  {
    return std::asin(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __atan(_Tp __x)
  {
    return std::atan(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __cos(_Tp __x)
  {
    return std::cos(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __sin(_Tp __x)
  {
    return std::sin(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __tan(_Tp __x)
  {
    return std::tan(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __acosh(_Tp __x)
  {
    return std::acosh(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __asinh(_Tp __x)
  {
    return std::asinh(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __atanh(_Tp __x)
  {
    return std::atanh(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __cosh(_Tp __x)
  {
    return std::cosh(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __sinh(_Tp __x)
  {
    return std::sinh(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __tanh(_Tp __x)
  {
    return std::tanh(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __atan2(_Tp __x, _Tp __y)
  {
    return std::atan2(__x, __y);
  }

  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __exp(_Tp __x)
  {
    return std::exp(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __exp2(_Tp __x)
  {
    return std::exp2(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __expm1(_Tp __x)
  {
    return std::expm1(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __log(_Tp __x)
  {
    return std::log(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __log10(_Tp __x)
  {
    return std::log10(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __log1p(_Tp __x)
  {
    return std::log1p(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __log2(_Tp __x)
  {
    return std::log2(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __logb(_Tp __x)
  {
    return std::logb(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _ST<int> __ilogb(_Tp __x)
  {
    return {std::ilogb(__x)};
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __pow(_Tp __x, _Tp __y)
  {
    return std::pow(__x, __y);
  }

  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __abs(_Tp __x)
  {
    return std::abs(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __fabs(_Tp __x)
  {
    return std::fabs(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __sqrt(_Tp __x)
  {
    return std::sqrt(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __cbrt(_Tp __x)
  {
    return std::cbrt(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __erf(_Tp __x)
  {
    return std::erf(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __erfc(_Tp __x)
  {
    return std::erfc(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __lgamma(_Tp __x)
  {
    return std::lgamma(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __tgamma(_Tp __x)
  {
    return std::tgamma(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __trunc(_Tp __x)
  {
    return std::trunc(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __floor(_Tp __x)
  {
    return std::floor(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __ceil(_Tp __x)
  {
    return std::ceil(__x);
  }

  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __nearbyint(_Tp __x)
  {
    return std::nearbyint(__x);
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __rint(_Tp __x)
  {
    return std::rint(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _ST<long> __lrint(_Tp __x)
  {
    return {std::lrint(__x)};
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _ST<long long> __llrint(_Tp __x)
  {
    return {std::llrint(__x)};
  }
  template <typename _Tp> _GLIBCXX_SIMD_INTRINSIC static _Tp __round(_Tp __x)
  {
    return std::round(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _ST<long> __lround(_Tp __x)
  {
    return {std::lround(__x)};
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _ST<long long> __llround(_Tp __x)
  {
    return {std::llround(__x)};
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __ldexp(_Tp __x, _ST<int> __y)
  {
    return std::ldexp(__x, __y.first);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __scalbn(_Tp __x, _ST<int> __y)
  {
    return std::scalbn(__x, __y.first);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __scalbln(_Tp __x, _ST<long> __y)
  {
    return std::scalbln(__x, __y.first);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __fmod(_Tp __x, _Tp __y)
  {
    return std::fmod(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __remainder(_Tp __x, _Tp __y)
  {
    return std::remainder(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __nextafter(_Tp __x, _Tp __y)
  {
    return std::nextafter(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __fdim(_Tp __x, _Tp __y)
  {
    return std::fdim(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __fmax(_Tp __x, _Tp __y)
  {
    return std::fmax(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __fmin(_Tp __x, _Tp __y)
  {
    return std::fmin(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __fma(_Tp __x, _Tp __y, _Tp __z)
  {
    return std::fma(__x, __y, __z);
  }

  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __remquo(_Tp __x, _Tp __y, _ST<int>* __z)
  {
    return std::remquo(__x, __y, &__z->first);
  }
  template <typename _Tp>
  [[deprecated]] _GLIBCXX_SIMD_INTRINSIC static _Tp __remquo(_Tp __x, _Tp __y,
							     int* __z)
  {
    return std::remquo(__x, __y, __z);
  }

  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static _ST<int> __fpclassify(_Tp __x)
  {
    return {std::fpclassify(__x)};
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isfinite(_Tp __x)
  {
    return std::isfinite(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isinf(_Tp __x)
  {
    return std::isinf(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isnan(_Tp __x)
  {
    return std::isnan(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isnormal(_Tp __x)
  {
    return std::isnormal(__x);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __signbit(_Tp __x)
  {
    return std::signbit(__x);
  }

  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isgreater(_Tp __x, _Tp __y)
  {
    return std::isgreater(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isgreaterequal(_Tp __x, _Tp __y)
  {
    return std::isgreaterequal(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isless(_Tp __x, _Tp __y)
  {
    return std::isless(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __islessequal(_Tp __x, _Tp __y)
  {
    return std::islessequal(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __islessgreater(_Tp __x, _Tp __y)
  {
    return std::islessgreater(__x, __y);
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __isunordered(_Tp __x, _Tp __y)
  {
    return std::isunordered(__x, __y);
  }

  // __increment & __decrement{{{2
  template <typename _Tp> constexpr static inline void __increment(_Tp& __x)
  {
    ++__x;
  }
  template <typename _Tp> constexpr static inline void __decrement(_Tp& __x)
  {
    --__x;
  }

  // compares {{{2
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __equal_to(_Tp __x, _Tp __y)
  {
    return __x == __y;
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __not_equal_to(_Tp __x, _Tp __y)
  {
    return __x != __y;
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __less(_Tp __x, _Tp __y)
  {
    return __x < __y;
  }
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __less_equal(_Tp __x, _Tp __y)
  {
    return __x <= __y;
  }

  // smart_reference access {{{2
  template <typename _Tp, typename _Up>
  constexpr static void __set(_Tp& __v, [[maybe_unused]] int __i,
			      _Up&& __x) noexcept
  {
    _GLIBCXX_DEBUG_ASSERT(__i == 0);
    __v = static_cast<_Up&&>(__x);
  }

  // __masked_assign {{{2
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static void
  __masked_assign(bool __k, _Tp& __lhs, _Tp __rhs)
  {
    if (__k)
      __lhs = __rhs;
  }

  // __masked_cassign {{{2
  template <typename _Op, typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static void
  __masked_cassign(const bool __k, _Tp& __lhs, const _Tp __rhs, _Op __op)
  {
    if (__k)
      __lhs = __op(_SimdImplScalar{}, __lhs, __rhs);
  }

  // __masked_unary {{{2
  template <template <typename> class _Op, typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static _Tp __masked_unary(const bool __k,
							      const _Tp __v)
  {
    return static_cast<_Tp>(__k ? _Op<_Tp>{}(__v) : __v);
  }

  // }}}2
};

// }}}
// _MaskImplScalar {{{
struct _MaskImplScalar
{
  // member types {{{
  template <typename _Tp> using _TypeTag = _Tp*;

  // }}}
  // __broadcast {{{
  template <typename>
  _GLIBCXX_SIMD_INTRINSIC static constexpr bool __broadcast(bool __x)
  {
    return __x;
  }

  // }}}
  // __load {{{
  template <typename>
  _GLIBCXX_SIMD_INTRINSIC static constexpr bool __load(const bool* __mem)
  {
    return __mem[0];
  }

  // }}}
  // __to_bits {{{
  _GLIBCXX_SIMD_INTRINSIC static constexpr _SanitizedBitMask<1>
  __to_bits(bool __x)
  {
    return __x;
  }

  // }}}
  // __convert {{{
  template <typename _Tp, bool _Sanitized>
  _GLIBCXX_SIMD_INTRINSIC static constexpr bool
  __convert(_BitMask<1, _Sanitized> __x)
  {
    return __x[0];
  }

  template <typename _Tp, typename _Up, typename _UAbi>
  _GLIBCXX_SIMD_INTRINSIC static constexpr bool
  __convert(simd_mask<_Up, _UAbi> __x)
  {
    return __x[0];
  }

  // }}}
  // __from_bitmask {{{2
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool
  __from_bitmask(_SanitizedBitMask<1> __bits, _TypeTag<_Tp>) noexcept
  {
    return __bits[0];
  }

  // __masked_load {{{2
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool
  __masked_load(bool __merge, bool __mask, const bool* __mem) noexcept
  {
    if (__mask)
      __merge = __mem[0];
    return __merge;
  }

  // __store {{{2
  _GLIBCXX_SIMD_INTRINSIC static void __store(bool __v, bool* __mem) noexcept
  {
    __mem[0] = __v;
  }

  // __masked_store {{{2
  _GLIBCXX_SIMD_INTRINSIC static void
  __masked_store(const bool __v, bool* __mem, const bool __k) noexcept
  {
    if (__k)
      __mem[0] = __v;
  }

  // logical and bitwise operators {{{2
  static constexpr bool __logical_and(bool __x, bool __y) { return __x && __y; }
  static constexpr bool __logical_or(bool __x, bool __y) { return __x || __y; }
  static constexpr bool __bit_not(bool __x) { return !__x; }
  static constexpr bool __bit_and(bool __x, bool __y) { return __x && __y; }
  static constexpr bool __bit_or(bool __x, bool __y) { return __x || __y; }
  static constexpr bool __bit_xor(bool __x, bool __y) { return __x != __y; }

  // smart_reference access {{{2
  constexpr static void __set(bool& __k, [[maybe_unused]] int __i,
			      bool __x) noexcept
  {
    _GLIBCXX_DEBUG_ASSERT(__i == 0);
    __k = __x;
  }

  // __masked_assign {{{2
  _GLIBCXX_SIMD_INTRINSIC static void __masked_assign(bool __k, bool& __lhs,
						      bool __rhs)
  {
    if (__k)
      __lhs = __rhs;
  }

  // }}}2
  // __all_of {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool
  __all_of(simd_mask<_Tp, _Abi> __k)
  {
    return __k._M_data;
  }

  // }}}
  // __any_of {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool
  __any_of(simd_mask<_Tp, _Abi> __k)
  {
    return __k._M_data;
  }

  // }}}
  // __none_of {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool
  __none_of(simd_mask<_Tp, _Abi> __k)
  {
    return !__k._M_data;
  }

  // }}}
  // __some_of {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static bool __some_of(simd_mask<_Tp, _Abi>)
  {
    return false;
  }

  // }}}
  // __popcount {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static int
  __popcount(simd_mask<_Tp, _Abi> __k)
  {
    return __k._M_data;
  }

  // }}}
  // __find_first_set {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static int
    __find_first_set(simd_mask<_Tp, _Abi>)
  {
    return 0;
  }

  // }}}
  // __find_last_set {{{
  template <typename _Tp, typename _Abi>
  _GLIBCXX_SIMD_INTRINSIC constexpr static int
    __find_last_set(simd_mask<_Tp, _Abi>)
  {
    return 0;
  }

  // }}}
};

// }}}

_GLIBCXX_SIMD_END_NAMESPACE
#endif // __cplusplus >= 201703L
#endif // _GLIBCXX_EXPERIMENTAL_SIMD_SCALAR_H_

// vim: foldmethod=marker sw=2 noet ts=8 sts=2 tw=80
