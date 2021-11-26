// { dg-options "-fpretty-templates" }
// { dg-do compile { target c++11 } }

template <int a = 1>
  [[deprecated]] void f0(); // { dg-message "'template<int a> void f0\\(\\)'" }

template <int a>
  [[deprecated]] void f1(); // { dg-message "'template<int a> void f1\\(\\)'" }

template <class a, int b = 1>
  [[deprecated]] void f2(); // { dg-message "'template<class a, int b> void f2\\(\\)'" }

template <class a, int b = 1>
  [[deprecated]] void f3(a); // { dg-message "'template<class a, int b> void f3\\(a\\)'" }

template <class a>
  [[deprecated]] void f4(a); // { dg-message "'template<class a> void f4\\(a\\)'" }

template <>
  [[deprecated]] void f4<int>(int);

template <>
  [[deprecated]] void f4(float);

template <class a, class b = int>
  [[deprecated]] void f5(a);

template void f5<float>(float); // { dg-error "'void f5<a>\\(a\\) .with a = float.'" }

template <class a, class b = int>
  struct c
  {
    template <class d, class e = int>
      [[deprecated]] static void f(d);
  };

int main()
{
  f0();            // { dg-warning "'void f0\\(\\)'" }
  f1<1>();         // { dg-warning "'void f1<a>\\(\\) .with int a = 1.'" }
  f2<int>();       // { dg-warning "'void f2<a>\\(\\) .with a = int.'" }
  f3(1);           // { dg-warning "'void f3\\(a\\) .with a = int.'" }
  f3<float>(1);    // { dg-warning "'void f3<a>\\(a\\) .with a = float.'" }
  f3<float, 2>(1); // { dg-warning "'void f3<a, b>\\(a\\) .with a = float; int b = 2.'" }
  f4(1.);          // { dg-warning "'void f4\\(a\\) .with a = double.'" }
  f4(1);           // { dg-warning "'void f4<a>\\(a\\) .with a = int.'" }
  f4(1.f);         // { dg-warning "'void f4\\(a\\) .with a = float.'" }

  f0(0); // { dg-error "" }
  f1(0); // { dg-error "" }
  f2(0); // { dg-error "" }
  f3();  // { dg-error "" }
  f4();  // { dg-error "" }

  c<int>::f(1.);    // { dg-warning "'static void c<a>::f\\(d\\) .with d = double; a = int.'" }
  c<int>::f<int>(1);    // { dg-warning "'static void c<a>::f<d>\\(d\\) .with d = int; a = int.'" }
  c<int>::f<float, int>(1.f);    // { dg-warning "'static void c<a>::f<d, e>\\(d\\) .with d = float; e = int; a = int.'" }
  c<float, int>::f(1.);    // { dg-warning "'static void c<a, b>::f\\(d\\) .with d = double; a = float; b = int.'" }
  c<float, int>::f<int>(1);    // { dg-warning "'static void c<a, b>::f<d>\\(d\\) .with d = int; a = float; b = int.'" }
  c<float, int>::f<float, int>(1.f);    // { dg-warning "'static void c<a, b>::f<d, e>\\(d\\) .with d = float; e = int; a = float; b = int.'" }
}
