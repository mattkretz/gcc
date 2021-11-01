// { dg-options "-fpretty-templates" }
// { dg-do compile { target c++11 } }
//
// Tests the case where DECL_TI_TEMPLATE doesn't return a TEMPLATE_DECL for the
// FUNCTION_DECL of foo<U>(int).

template <class T0, class T1 = int, int x = 1> void foo (int);

template <class U, int y = 2>
class Q {
  friend void foo<U> (int = 3); // { dg-error "'void foo<U>\\(int\\)'" }
};
