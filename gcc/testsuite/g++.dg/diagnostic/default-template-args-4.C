// { dg-options "-fpretty-templates" }
// { dg-do compile { target c++11 } }

template <class T> struct X {};
template <class... T> struct Y {};

template <template <class> class Tpl, template <class...> class VTpl,
	  class T0 = float, class U0 = VTpl<Tpl<T0>>, int x0 = 2>
  struct S
  {
    template <class T, class... Ps, class U = int, int x = 1>
      [[deprecated]] static U0
      f(T, Ps&&...);

    template <class T, class... Ps, class U = int, int x = 1>
      [[deprecated]] U0
      g(T, Ps&&...);
  };

void g()
{
  S<X, Y>::f(1, '2', 3, 4.); // { dg-warning "'static U0 S<Tpl, VTpl>::f\\(T, Ps&& ...\\) .with T = int; Ps = {char, int, double}; Tpl = X; VTpl = Y; U0 = Y<X<float> >.'" }
  S<X, Y>().g(1, '2', 3, 4.); // { dg-warning "'U0 S<Tpl, VTpl>::g\\(T, Ps&& ...\\) .with T = int; Ps = {char, int, double}; Tpl = X; VTpl = Y; U0 = Y<X<float> >.'" }
}
