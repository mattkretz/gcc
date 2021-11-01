// { dg-options "-fpretty-templates" }
// { dg-do compile { target c++11 } }

template <class T>
  struct id
  { using type = T; };

template <class T0, class T1 = int, template <class> class foo = id>
  struct A
  {
    template <class U0 = const T1&>
      [[deprecated]] static void
      f(typename foo<U0>::type);
  };

template <int> struct B;

template <> struct B<1> 
  { using type = int; };

template <class T, int x = 1>
  [[deprecated]] typename B<x>::type
  f0(T);

int main()
{
  A<int>::f(0); // { dg-warning "'static void A<T0>::f\\(typename foo<U0>::type\\) .with U0 = const int&; T0 = int; foo = id; typename foo<U0>::type = const int&.'" }
  f0(0); // { dg-warning "'typename B<x>::type f0\\(T\\) .with T = int; int x = 1; typename B<x>::type = int.'" }
}
