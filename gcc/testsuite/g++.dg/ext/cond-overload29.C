// { dg-do compile }
// { dg-options "-std=c++23" }

// A defaulted operator?: converts each branch with its declared value
// category (std::forward-equivalent), not as an lvalue.  The operands are
// copyable, so only the value category distinguishes the ctor choice.

struct A { A(); A(const A&); };
struct B { B(); B(const B&); };

struct V
{
  V(const V&);
  V(A&&);
  V(B&&);
};

V operator?:(bool, A&&, B&&) = default;

// Taking the address requires the synthesized body, which must convert the
// forwarded parameters, not lvalues.
const auto ptr = &operator?:;

struct W
{
  W(const W&);
  W(A&&);
  W(B&&);
};

// A by-value branch is moved from as well.
W operator?:(bool, A, B) = default;
W (*ptr2) (bool, A, B) = &operator?:;
