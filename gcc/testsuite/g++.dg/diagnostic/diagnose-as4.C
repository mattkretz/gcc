// { dg-options "-fdiagnostics-use-aliases -fno-pretty-templates" }
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
struct [[gnu::diagnose_as("XX0")]] X0
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
    [[gnu::diagnose_as("f-2")]]
    static constexpr const char* f() {
      return __PRETTY_FUNCTION__;
    }

  struct [[gnu::diagnose_as("XX3")]] X3
  {
    template <class T3, class U3>
      [[gnu::diagnose_as("f-3")]]
      static constexpr const char* f() { // { dg-line X0_X3_f }
        return __PRETTY_FUNCTION__;
      }
  };
};

using X0int [[gnu::diagnose_as("X0intint")]] = X0<int, int>;

#define TEST(expected, fun)                                                    \
  static_assert(__builtin_strcmp(expected, fun) == 0, "")


void h() {
  TEST("constexpr const char* @1::@3::fun:3()",
      A::C<int>().h());
  TEST("constexpr const char* @1::@3::fun:2<float>()",
      A::C<int>().g<float>());
  TEST("constexpr const char* @1::@2<char>::fun:3()",
      A::C<char>().h());
  TEST("constexpr const char* @1::@2<char>::fun:2<float>()",
      A::C<char>().g<float>());
  TEST("constexpr const char* foo::barchar::A'<char, char>::f()",
      (foo::Bar<char>::A<char, char>().f()));
  TEST("constexpr const char* foo::barchar::A'<char, float>::f()",
      (foo::Bar<char>::A<char, float>().f()));
  TEST("constexpr const char* foo::barchar::A'<int, double>::f()",
      (foo::Bar<char>::A<int, double>().f()));
  TEST("constexpr const char* foo::barchar::SpecialA::f()",
      (foo::Bar<char>::A<int, int>().f()));
  TEST("constexpr const char* foo::Bar'<float>::A'<char, char>::f()",
      (foo::Bar<float>::A<char, char>().f()));
  TEST("constexpr const char* foo::Bar'<float>::A'<char, float>::f()",
      (foo::Bar<float>::A<char, float>().f()));
  TEST("constexpr const char* foo::Bar'<float>::A'<char, double>::f()",
      (foo::Bar<float>::A<char, double>().f()));
  TEST("constexpr const char* foo::Bar'<float>::SpecialA::f()",
      (foo::Bar<float>::A<int, int>().f()));
  TEST("static constexpr const char* foo::barchar::f()",
      foo::Bar<char>::f());
  TEST("static constexpr const char* foo::Bar'<char*>::f<int>()",
      foo::Bar<char*>::f());
  TEST("static constexpr const char* foo::Bar'<float*>::f<int>()",
      foo::Bar<float*>::f());
  TEST("static constexpr const char* foo::Bar'<float>::f()",
      foo::Bar<float>::f());
  TEST("static constexpr const char* foo::SpecialBar::f()",
      foo::Bar<void>::f());
  TEST("static constexpr const char* XX0<char, short int>::X1<short int, int>::X2'::f-1<long int, long long int>()",
      (X0<char, short>::X1<short, int>::X2<int, long>::f<long, long long>()));
  TEST("static constexpr const char* XX0<char, short int>::X1<short int, int>::X2<long int, int>::f-1<long int, long long int>()",
      (X0<char, short>::X1<short, int>::X2<long, int>::f<long, long long>()));
  TEST("static constexpr const char* XX0<char, short int>::XX3::f-3<long int, long long int>()",
      (X0<char, short>::X3::f<long, long long>()));
  TEST("static constexpr const char* X0intint::f-2<long int, long long int>()",
      (X0<int, int>::f<long, long long>()));
  TEST("static constexpr const char* X0intint::XX3::f-3<long int, long long int>()",
      (X0<int, int>::X3::f<long, long long>()));

  X0<char, short>::X3::f<long, long long>(1); // { dg-error "no matching function for call to 'XX0<char, short int>::XX3::f<long int, long long int>\\(int\\)" }
  // { dg-message "candidate: 'static constexpr const char\\* XX0<char, short int>::XX3::f-3<long int, long long int>\\(\\)'" "" { target *-*-* } X0_X3_f }

  X0<int, int>::X3::f<long, long long>(1); // { dg-error "no matching function for call to 'X0intint::XX3::f<long int, long long int>\\(int\\)" }
  // { dg-message "candidate: 'static constexpr const char\\* X0intint::XX3::f-3<long int, long long int>\\(\\)'" "" { target *-*-* } X0_X3_f }
}
