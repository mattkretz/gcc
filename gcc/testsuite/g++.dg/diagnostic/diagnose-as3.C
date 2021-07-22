// { dg-options "-fdiagnostics-use-aliases -fpretty-templates" }
// { dg-do compile { target c++11 } }

const char* volatile s;

namespace foo
{
  template <class U>
    struct [[gnu::diagnose_as("Bar'")]] Bar
    {
      template <class T0, class T1>
        struct [[gnu::diagnose_as("A'")]] A
        { constexpr const char* f() { return __PRETTY_FUNCTION__; } };

      template <class T>
        struct A<T, float>
        { constexpr const char* f() { return __PRETTY_FUNCTION__; } };

      template <class T>
        struct [[gnu::diagnose_as("Adouble")]] A<T, double>
        { constexpr const char* f() { return __PRETTY_FUNCTION__; } };

      using Special [[gnu::diagnose_as("SpecialA")]] = A<int, int>;

      static constexpr const char* f() { return __PRETTY_FUNCTION__; }
    };

  template <class P>
    struct
    [[gnu::diagnose_as("BarPtr")]]
    Bar<P*>
    {
      template <class U = int>
      static constexpr const char* f() { return __PRETTY_FUNCTION__; }
    };

  template <>
    struct [[gnu::diagnose_as("SpecialBar")]] Bar<void>
    {
      static constexpr const char* f() { return __PRETTY_FUNCTION__; }
    };

  using barchar [[gnu::diagnose_as("barchar")]] = Bar<char>;
}

namespace A
{
  inline namespace B __attribute__((diagnose_as("@1")))
  {
    template  <typename U>
      struct __attribute__((diagnose_as("@2"))) C
      {
        template <class T>
        __attribute__((diagnose_as("fun:2")))
          constexpr const char* g() { return __PRETTY_FUNCTION__; }

        __attribute__((diagnose_as("fun:3")))
          constexpr const char* h()
          { return __PRETTY_FUNCTION__; }
      };

    typedef C<int> D __attribute__((diagnose_as("@3")));
  }
}

template <class T0, class U0>
struct [[gnu::diagnose_as("X.0")]] X0
{
  template <class T1, class U1>
    struct X1
    {
      template <class T2, class U2>
        struct X2
        {
          template <class T3, class U3>
            [[gnu::diagnose_as("f-1")]]
            static constexpr const char* f() {
              return __PRETTY_FUNCTION__;
            }
        };

      using XX [[gnu::diagnose_as("X2'")]] = X2<int, long>;
    };

  template <class T3, class U3>
    [[gnu::diagnose_as("f-1")]]
    static constexpr const char* f() {
      return __PRETTY_FUNCTION__;
    }

  struct [[gnu::diagnose_as("X.3")]] X3
  {
    template <class T3, class U3>
      [[gnu::diagnose_as("f-1")]]
      static constexpr const char* f() {
        return __PRETTY_FUNCTION__;
      }
  };
};

using X0int [[gnu::diagnose_as("[int|int]")]] = X0<int, int>;

#define TEST(expected, fun)                                                    \
  static_assert(__builtin_strcmp(expected, fun) == 0, "")


void h() {
  TEST("constexpr const char* @1::@3::fun:3()",
      A::C<int>().h());
  TEST("constexpr const char* @1::@3::fun:2() [with T = float]",
      A::C<int>().g<float>());
  TEST("constexpr const char* @1::@2<U>::fun:3() [with U = char]",
      A::C<char>().h());
  TEST("constexpr const char* @1::@2<U>::fun:2() [with T = float; U = char]",
      A::C<char>().g<float>());
  TEST("constexpr const char* foo::barchar::A'<T0, T1>::f() [with T0 = char; T1 = char]",
      (foo::Bar<char>::A<char, char>().f()));
  TEST("constexpr const char* foo::barchar::A'<T, float>::f() [with T = char]",
      (foo::Bar<char>::A<char, float>().f()));
  TEST("constexpr const char* foo::barchar::Adouble<T, double>::f() [with T = int]",
      (foo::Bar<char>::A<int, double>().f()));
  TEST("constexpr const char* foo::barchar::SpecialA::f()",
      (foo::Bar<char>::A<int, int>().f()));
  TEST("constexpr const char* foo::Bar'<U>::A'<T0, T1>::f() [with T0 = char; T1 = char; U = float]",
      (foo::Bar<float>::A<char, char>().f()));
  TEST("constexpr const char* foo::Bar'<U>::A'<T, float>::f() [with T = char; U = float]",
      (foo::Bar<float>::A<char, float>().f()));
  TEST("constexpr const char* foo::Bar'<U>::Adouble<T, double>::f() [with T = char; U = float]",
      (foo::Bar<float>::A<char, double>().f()));
  TEST("constexpr const char* foo::Bar'<U>::SpecialA::f() [with U = float]",
      (foo::Bar<float>::A<int, int>().f()));
  TEST("static constexpr const char* foo::barchar::f()",
      foo::Bar<char>::f());
  TEST("static constexpr const char* foo::BarPtr<P*>::f() [with U = int; P = char]",
      foo::Bar<char*>::f());
  TEST("static constexpr const char* foo::BarPtr<P*>::f() [with U = int; P = float]",
      foo::Bar<float*>::f());
  TEST("static constexpr const char* foo::Bar'<U>::f() [with U = float]",
      foo::Bar<float>::f());
  TEST("static constexpr const char* foo::SpecialBar::f()",
      foo::Bar<void>::f());
  TEST("static constexpr const char* X.0<T0, U0>::X1<T1, U1>::X2'::f-1() [with T3 = long int; U3 = long long int; T1 = short int; U1 = int; T0 = char; U0 = short int]",
      (X0<char, short>::X1<short, int>::X2<int, long>::f<long, long long>()));
  TEST("static constexpr const char* X.0<T0, U0>::X1<T1, U1>::X2<T2, U2>::f-1() [with T3 = long int; U3 = long long int; T2 = long int; U2 = int; T1 = short int; U1 = int; T0 = char; U0 = short int]",
      (X0<char, short>::X1<short, int>::X2<long, int>::f<long, long long>()));
  TEST("static constexpr const char* X.0<T0, U0>::X.3::f-1() [with T3 = long int; U3 = long long int; T0 = char; U0 = short int]",
      (X0<char, short>::X3::f<long, long long>()));
  TEST("static constexpr const char* [int|int]::f-1() [with T3 = long int; U3 = long long int]",
      (X0<int, int>::f<long, long long>()));
  TEST("static constexpr const char* [int|int]::X.3::f-1() [with T3 = long int; U3 = long long int]",
      (X0<int, int>::X3::f<long, long long>()));
}
