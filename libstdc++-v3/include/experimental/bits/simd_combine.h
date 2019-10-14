// Simd _Combine ABI specific implementations -*- C++ -*-

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

#ifndef _GLIBCXX_EXPERIMENTAL_SIMD_COMBINE_H_
#define _GLIBCXX_EXPERIMENTAL_SIMD_COMBINE_H_

#if __cplusplus >= 201703L

_GLIBCXX_SIMD_BEGIN_NAMESPACE

template <int _N, typename _Abi>
struct _SimdImplCombine
{
  // member types {{{
  using abi_type = simd_abi::_Combine<_N, _Abi>;
  template <typename _Tp> using _TypeTag = _Tp *;
  using _PartImpl = typename _Abi::_SimdImpl;
  template <typename _Tp>
  using _SimdMember =
    std::array<typename _Abi::template __traits<_Tp>::_SimdMember, _N>;

  // }}}
  // broadcast {{{
  template <typename _Tp>
  _GLIBCXX_SIMD_INTRINSIC static constexpr _SimdMember<_Tp>
    __broadcast(_Tp __x) noexcept
  {
    return __generate_from_n_evaluations<_N, _SimdMember<_Tp>>(
      [&](int) constexpr { return _PartImpl::__broadcast(__x); });
  }

  // }}}
  // load {{{
//X   template <typename _Tp, typename _Up, typename _F>
//X   _GLIBCXX_SIMD_INTRINSIC static constexpr _Tp
//X     __load(const _U* __mem, _F, _TypeTag<_Tp> __x) noexcept
//X   {
//X     return __generate_from_n_evaluations<_N, _Tp>(
//X       [&](auto __i) constexpr { return _PartImpl::__load(__mem + __i * __part_size<_Tp>, _F{}, __x); });
//X   }
//X 
  // }}}
};
template <int _N, typename _Abi>
struct _MaskImplCombine
{
};

_GLIBCXX_SIMD_END_NAMESPACE
#endif  // __cplusplus >= 201703L
#endif  // _GLIBCXX_EXPERIMENTAL_SIMD_COMBINE_H_
// vim: foldmethod=marker sw=2 noet ts=8 sts=2 tw=80
