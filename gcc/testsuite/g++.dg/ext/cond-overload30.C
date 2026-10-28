// { dg-do compile }
// { dg-options "-std=c++23" }

// The declared value category of each branch of a defaulted operator?: is
// enforced: an lvalue reference parameter must not reach an rvalue-only
// entry, and a branch that converts as an xvalue must not reach an
// lvalue-only entry.

struct A { A(); A(const A&); };
struct B { B(); B(const B&); };

struct V { V(const V&); V(A&&); V(const B&); };
V operator?:(bool, A&, B&&) = default; // { dg-error "cannot convert" }

struct W { W(const W&); W(A&); W(const B&); };
W operator?:(bool, A&&, B&&) = default; // { dg-error "cannot convert" }

struct X { X(const X&); X(A&); X(const B&); };
X operator?:(bool, A, B&&) = default; // { dg-error "cannot convert" }
