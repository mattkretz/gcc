// { dg-do compile }
// { dg-options "-std=c++23" }

#include <variant>

struct move_only
{
    move_only() = default;
    move_only(move_only&&) = default;
    move_only& operator =(move_only&&) = default;
    move_only(const move_only&) = delete;
    move_only& operator =(const move_only&) = delete;
};

struct A : move_only {};
struct B : move_only {};

std::variant<A, B>
operator?:(bool c, A&& a, B&& b) = default;

const auto val = true ? A{} : B{};
