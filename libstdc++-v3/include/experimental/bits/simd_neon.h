// Simd NEON specific implementations -*- C++ -*-

// Copyright (C) 2019 Free Software Foundation, Inc.
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

#ifndef _GLIBCXX_EXPERIMENTAL_SIMD_NEON_H_
#define _GLIBCXX_EXPERIMENTAL_SIMD_NEON_H_

#if __cplusplus >= 201703L

#if !_GLIBCXX_SIMD_HAVE_NEON
#error "simd_neon.h may only be included when NEON on ARM is available"
#endif

_GLIBCXX_SIMD_BEGIN_NAMESPACE

// _SimdImplNeon {{{
template <typename _Abi>
struct _SimdImplNeon : _SimdImplBuiltin<_Abi>
{
  using _Base = _SimdImplBuiltin<_Abi>;
  template <typename _Tp>
  static constexpr size_t _S_max_store_size = 16;

  // __masked_load {{{
  template <typename _Tp, size_t _N, typename _Up, typename _Fp>
  static inline _SimdWrapper<_Tp, _N>
    __masked_load(_SimdWrapper<_Tp, _N> __merge,
		  _SimdWrapper<_Tp, _N> __k,
		  const _Up*            __mem,
		  _Fp) noexcept
  {
    __execute_n_times<_N>([&](auto __i) {
      if (__k[__i] != 0)
	__merge.__set(__i, static_cast<_Tp>(__mem[__i]));
    });
    return __merge;
  }

  // }}}
  // __masked_store_nocvt {{{
  template <typename _Tp, std::size_t _N, typename _F>
  _GLIBCXX_SIMD_INTRINSIC static void __masked_store_nocvt(
    _SimdWrapper<_Tp, _N> __v, _Tp* __mem, _F, _SimdWrapper<_Tp, _N> __k)
  {
    __execute_n_times<_N>([&](auto __i) {
      if (__k[__i] != 0)
	__mem[__i] = __v[__i];
    });
  }

  // }}}
  // __reduce {{{
  template <typename _Tp, class _BinaryOperation>
  _GLIBCXX_SIMD_INTRINSIC static _Tp
    __reduce(simd<_Tp, _Abi> __x, _BinaryOperation&& __binary_op)
  {
    constexpr size_t _N = __x.size();
    if constexpr (sizeof(__x) == 16 && _N >= 4 && !_Abi::_S_is_partial)
      {
	const auto __halves = split<simd<_Tp, simd_abi::_Neon<8>>>(__x);
	const auto __y      = __binary_op(__halves[0], __halves[1]);
	return _SimdImplNeon<simd_abi::_Neon<8>>::__reduce(
	  __y, forward<_BinaryOperation>(__binary_op));
      }
    else if constexpr (_N == 8)
      {
	__x = __binary_op(
	  __x, _Base::template __make_simd<_Tp, _N>(
		 __vector_permute<1, 0, 3, 2, 5, 4, 7, 6>(__x._M_data)));
	__x = __binary_op(
	  __x, _Base::template __make_simd<_Tp, _N>(
		 __vector_permute<3, 2, 1, 0, 7, 6, 5, 4>(__x._M_data)));
	__x = __binary_op(
	  __x, _Base::template __make_simd<_Tp, _N>(
		 __vector_permute<7, 6, 5, 4, 3, 2, 1, 0>(__x._M_data)));
	return __x[0];
      }
    else if constexpr (_N == 4)
      {
	__x = __binary_op(__x, _Base::template __make_simd<_Tp, _N>(
				 __vector_permute<1, 0, 3, 2>(__x._M_data)));
	__x = __binary_op(__x, _Base::template __make_simd<_Tp, _N>(
				 __vector_permute<3, 2, 1, 0>(__x._M_data)));
	return __x[0];
      }
    else if constexpr (_N == 2)
      {
	__x = __binary_op(__x, _Base::template __make_simd<_Tp, _N>(
				 __vector_permute<1, 0>(__x._M_data)));
	return __x[0];
      }
    else
      return _Base::__reduce(__x, forward<_BinaryOperation>(__binary_op));
  }

  // }}}
  // math {{{
  // __sqrt {{{
  template <typename _Tp, typename _TVT = _VectorTraits<_Tp>>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __sqrt(_Tp __x)
  {
    if constexpr (__have_neon_a64)
      {
	const auto __intrin = __to_intrin(__x);
	if constexpr (_TVT::template __is<float, 2>)
	  return vsqrt_f32(__intrin);
	else if constexpr (_TVT::template __is<float, 4>)
	  return vsqrtq_f32(__intrin);
	else if constexpr (_TVT::template __is<double, 1>)
	  return vsqrt_f64(__intrin);
	else if constexpr (_TVT::template __is<double, 2>)
	  return vsqrtq_f64(__intrin);
	else
	  __assert_unreachable<_Tp>();
      }
    else
      return _Base::__sqrt(__x);
  } // }}}
  // __trunc {{{
  template <typename _Tp, typename _TVT = _VectorTraits<_Tp>>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __trunc(_Tp __x)
  {
    if constexpr (__have_neon_a32)
      {
	const auto __intrin = __to_intrin(__x);
	if constexpr (_TVT::template __is<float, 2>)
	  return vrnd_f32(__intrin);
	else if constexpr (_TVT::template __is<float, 4>)
	  return vrndq_f32(__intrin);
	else if constexpr (_TVT::template __is<double, 1>)
	  return vrnd_f64(__intrin);
	else if constexpr (_TVT::template __is<double, 2>)
	  return vrndq_f64(__intrin);
	else
	  __assert_unreachable<_Tp>();
      }
    else
      return _Base::__trunc(__x);
  } // }}}
  // __floor {{{
  template <typename _Tp, typename _TVT = _VectorTraits<_Tp>>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __floor(_Tp __x)
  {
    if constexpr (__have_neon_a32)
      {
	const auto __intrin = __to_intrin(__x);
	if constexpr (_TVT::template __is<float, 2>)
	  return vrndm_f32(__intrin);
	else if constexpr (_TVT::template __is<float, 4>)
	  return vrndmq_f32(__intrin);
	else if constexpr (_TVT::template __is<double, 1>)
	  return vrndm_f64(__intrin);
	else if constexpr (_TVT::template __is<double, 2>)
	  return vrndmq_f64(__intrin);
	else
	  __assert_unreachable<_Tp>();
      }
    else
      return _Base::__floor(__x);
  } // }}}
  // __ceil {{{
  template <typename _Tp, typename _TVT = _VectorTraits<_Tp>>
  _GLIBCXX_SIMD_INTRINSIC static _Tp __ceil(_Tp __x)
  {
    if constexpr (__have_neon_a32)
      {
	const auto __intrin = __to_intrin(__x);
	if constexpr (_TVT::template __is<float, 2>)
	  return vrndp_f32(__intrin);
	else if constexpr (_TVT::template __is<float, 4>)
	  return vrndpq_f32(__intrin);
	else if constexpr (_TVT::template __is<double, 1>)
	  return vrndp_f64(__intrin);
	else if constexpr (_TVT::template __is<double, 2>)
	  return vrndpq_f64(__intrin);
	else
	  __assert_unreachable<_Tp>();
      }
    else
      return _Base::__ceil(__x);
  } //}}}
  //}}}
}; // }}}
// _MaskImplNeonMixin {{{
struct _MaskImplNeonMixin {
  using _Base = _MaskImplBuiltinMixin;

  template <typename _Tp, size_t _N>
  _GLIBCXX_SIMD_INTRINSIC static constexpr auto
    __to_bits(_SimdWrapper<_Tp, _N> __x)
  {
    using _I = __int_for_sizeof_t<_Tp>;
    if constexpr (sizeof(__x) == 16)
      {
	auto                            __asint = __vector_bitcast<_I>(__x);
#ifdef __aarch64__
	[[maybe_unused]] constexpr auto __zero  = decltype(__asint)();
#else
	[[maybe_unused]] constexpr auto __zero = decltype(__lo64(__asint))();
#endif
	if constexpr (sizeof(_Tp) == 1)
	  {
	    __asint &=
	      __make_vector<_I>(0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1,
				0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80);
#ifdef __aarch64__
	    return __vector_bitcast<_UShort>(vpaddq_s8(
	      vpaddq_s8(vpaddq_s8(__asint, __zero), __zero), __zero))[0];
#else
	    return __vector_bitcast<_UShort>(vpadd_s8(
	      vpadd_s8(vpadd_s8(__lo64(__asint), __hi64(__asint)), __zero),
	      __zero))[0];
#endif
	  }
	else if constexpr (sizeof(_Tp) == 2)
	  {
	    __asint &=
	      __make_vector<_I>(0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80);
#ifdef __aarch64__
	    return vpaddq_s16(vpaddq_s16(vpaddq_s16(__asint, __zero), __zero),
			      __zero)[0];
#else
	    return vpadd_s16(
	      vpadd_s16(vpadd_s16(__lo64(__asint), __hi64(__asint)), __zero),
	      __zero)[0];
#endif
	  }
	else if constexpr (sizeof(_Tp) == 4)
	  {
	    __asint &= __make_vector<_I>(0x1, 0x2, 0x4, 0x8);
#ifdef __aarch64__
	    return vpaddq_s32(vpaddq_s32(__asint, __zero), __zero)[0];
#else
	    return vpadd_s32(vpadd_s32(__lo64(__asint), __hi64(__asint)),
			     __zero)[0];
#endif
	  }
	else if constexpr (sizeof(_Tp) == 8)
	  {
	    return (__asint[0] & 1) | (__asint[1] & 2);
	  }
	else
	  __assert_unreachable<_Tp>();
      }
    else if constexpr (sizeof(__x) == 8)
      {
	auto                            __asint = __vector_bitcast<_I>(__x);
	[[maybe_unused]] constexpr auto __zero  = decltype(__asint)();
	if constexpr (sizeof(_Tp) == 1)
	  {
	    __asint &=
	      __make_vector<_I>(0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80);
	    return vpadd_s8(vpadd_s8(vpadd_s8(__asint, __zero), __zero),
			    __zero)[0];
	  }
	else if constexpr (sizeof(_Tp) == 2)
	  {
	    __asint &= __make_vector<_I>(0x1, 0x2, 0x4, 0x8);
	    return vpadd_s16(vpadd_s16(__asint, __zero), __zero)[0];
	  }
	else if constexpr (sizeof(_Tp) == 4)
	  {
	    __asint &= __make_vector<_I>(0x1, 0x2);
	    return vpadd_s32(__asint, __zero)[0];
	  }
	else
	  __assert_unreachable<_Tp>();
      }
    else
      return _Base::__to_bits(__x);
  }
};

// }}}
// _MaskImplNeon {{{
template <typename _Abi>
struct _MaskImplNeon : _MaskImplNeonMixin, _MaskImplBuiltin<_Abi>
{
  using _MaskImplNeonMixin::__to_bits;
}; // }}}

_GLIBCXX_SIMD_END_NAMESPACE
#endif  // __cplusplus >= 201703L
#endif  // _GLIBCXX_EXPERIMENTAL_SIMD_NEON_H_
// vim: foldmethod=marker sw=2 noet ts=8 sts=2 tw=80
