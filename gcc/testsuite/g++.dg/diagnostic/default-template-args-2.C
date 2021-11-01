// { dg-options "-fno-pretty-templates" }
// { dg-do compile { target c++11 } }

template <int a = 1>
  [[deprecated]] void f0();

template <int a>
  [[deprecated]] void f1();

template <class a, int b = 1>
  [[deprecated]] void f2();

template <class a, int b = 1>
  [[deprecated]] void f3(a);

template <class a>
  [[deprecated]] void f4(a);

template <>
  [[deprecated]] void f4<int>(int);

template <class a, class b = int>
  [[deprecated]] void f5(a);

template void f5<float>(float); // { dg-error "'void f5<float>\\(float\\)'" }

int main()
{
  f0();            // { dg-warning "'void f0\\(\\)'" }
  f1<1>();         // { dg-warning "'void f1<1>\\(\\)'" }
  f2<int>();       // { dg-warning "'void f2<int>\\(\\)'" }
  f3(1);           // { dg-warning "'void f3\\(int\\)'" }
  f3<float>(1);    // { dg-warning "'void f3<float>\\(float\\)'" }
  f3<float, 2>(1); // { dg-warning "'void f3<float, 2>\\(float\\)'" }
  f4(1.);          // { dg-warning "'void f4\\(double\\)'" }
  f4(1);           // { dg-warning "'void f4<int>\\(int\\)'" }
}
