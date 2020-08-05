#ifndef VC_TESTS_METAHELPERS_H_
#define VC_TESTS_METAHELPERS_H_

#include <functional>
#include <type_traits>
#include <utility>

namespace vir
{
namespace test
{
// operator_is_substitution_failure {{{1
template <class A, class B, class Op>
constexpr bool operator_is_substitution_failure_impl(float)
{
  return true;
}

template <class A, class B, class Op>
constexpr
    typename std::conditional<true, bool,
                              decltype(Op()(std::declval<A>(), std::declval<B>()))>::type
    operator_is_substitution_failure_impl(int)
{
  return false;
}

template <class... Ts> constexpr bool operator_is_substitution_failure()
{
  return operator_is_substitution_failure_impl<Ts...>(int());
}

// sfinae_is_callable{{{1
#ifdef Vc_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-inline"
#endif
template <class... Args, class F>
constexpr auto sfinae_is_callable_impl(int, F &&f) -> typename std::conditional<
    true, std::true_type, decltype(std::forward<F>(f)(std::declval<Args>()...))>::type;
template <class... Args, class F> constexpr std::false_type sfinae_is_callable_impl(float, const F &);
template <class... Args, class F> constexpr bool sfinae_is_callable(F &&)
{
  return decltype(sfinae_is_callable_impl<Args...>(int(), std::declval<F>()))::value;
}
template <class... Args, class F>
constexpr auto sfinae_is_callable_t(F &&f)
    -> decltype(sfinae_is_callable_impl<Args...>(int(), std::declval<F>()));

#ifdef Vc_CLANG
#pragma clang diagnostic pop
#endif

// traits {{{1
template <class A, class B> constexpr bool has_less_bits()
{
  return std::__digits_v<A> < std::__digits_v<B>;
}

//}}}1
}  // namespace test
}  // namespace vir

// more operator objects {{{1
struct assignment {
    template <class A, class B>
    constexpr decltype(std::declval<A>() = std::declval<B>()) operator()(A &&a,
                                                                         B &&b) const
        noexcept(noexcept(std::forward<A>(a) = std::forward<B>(b)))
    {
        return std::forward<A>(a) = std::forward<B>(b);
    }
};

struct bit_shift_left {
    template <class A, class B>
    constexpr decltype(std::declval<A>() << std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) << std::forward<B>(b)))
    {
        return std::forward<A>(a) << std::forward<B>(b);
    }
};

struct bit_shift_right {
    template <class A, class B>
    constexpr decltype(std::declval<A>() >> std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) >> std::forward<B>(b)))
    {
        return std::forward<A>(a) >> std::forward<B>(b);
    }
};

struct assign_modulus {
    template <class A, class B>
    constexpr decltype(std::declval<A>() %= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) %= std::forward<B>(b)))
    {
        return std::forward<A>(a) %= std::forward<B>(b);
    }
};

struct assign_bit_and {
    template <class A, class B>
    constexpr decltype(std::declval<A>() &= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) &= std::forward<B>(b)))
    {
        return std::forward<A>(a) &= std::forward<B>(b);
    }
};

struct assign_bit_or {
    template <class A, class B>
    constexpr decltype(std::declval<A>() |= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) |= std::forward<B>(b)))
    {
        return std::forward<A>(a) |= std::forward<B>(b);
    }
};

struct assign_bit_xor {
    template <class A, class B>
    constexpr decltype(std::declval<A>() ^= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) ^= std::forward<B>(b)))
    {
        return std::forward<A>(a) ^= std::forward<B>(b);
    }
};

struct assign_bit_shift_left {
    template <class A, class B>
    constexpr decltype(std::declval<A>() <<= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) <<= std::forward<B>(b)))
    {
        return std::forward<A>(a) <<= std::forward<B>(b);
    }
};

struct assign_bit_shift_right {
    template <class A, class B>
    constexpr decltype(std::declval<A>() >>= std::declval<B>()) operator()(A &&a,
                                                                          B &&b) const
        noexcept(noexcept(std::forward<A>(a) >>= std::forward<B>(b)))
    {
        return std::forward<A>(a) >>= std::forward<B>(b);
    }
};

// operator_is_substitution_failure {{{1
template <class A, class B, class Op = std::plus<>>
constexpr bool is_substitution_failure =
    vir::test::operator_is_substitution_failure<A, B, Op>();

// sfinae_is_callable{{{1
using vir::test::sfinae_is_callable;

// traits {{{1
using vir::test::has_less_bits;

//}}}1

#endif  // VC_TESTS_METAHELPERS_H_
// vim: foldmethod=marker
