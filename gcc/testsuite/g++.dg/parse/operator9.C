// { dg-do compile }
// { dg-options "-std=c++20 -fno-overloadable-conditional" }

struct A {};
struct B {};
int operator?:(bool, A, B);  // { dg-error "prohibits overloading" }
