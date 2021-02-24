// { dg-options "-fdiagnostics-use-aliases -fpretty-templates" }

#ifdef __cpp_constexpr
template <typename T>
  constexpr bool is_char() { return false; }

template <>
  constexpr bool is_char<char>() { return true; }

template <typename T>
  constexpr bool is_int() { return false; }

template <>
  constexpr bool is_int<int>() { return true; }
#endif

namespace __attribute__((diagnose_as())) foo0 {} // { dg-warning "'diagnose_as' attribute requires a single NTBS argument" }

namespace __attribute__((diagnose_as("foo2"))) foo1 {} // { dg-line foo1 }
namespace __attribute__((diagnose_as("XXXX"))) foo1 {} // { dg-warning "'foo1' redeclared with different 'diagnose_as' attribute value" }
// { dg-message "previous declaration here" "" { target *-*-* } foo1 }
namespace foo1
{
  void f() {
    f(1); // { dg-error "too many arguments to function 'void foo2::f\\(\\)'" }
  }

  class __attribute__((diagnose_as("XX"))) X; // { dg-line XX }
  class __attribute__((diagnose_as("--"))) X { // { dg-warning "'class foo2::XX' redeclared with 'diagnose_as\\(\"--\"\\)' attribute" }
    // { dg-message "previous declaration here" "" { target *-*-* } XX }
    __attribute__((diagnose_as("ff"))) void f();
  };
  class Y : X
  {
    void g() {
      f(); // { dg-error "'void foo2::XX::ff\\(\\)' is private within this context" }
    }
  };

  class __attribute__((diagnose_as())) Z0; // { dg-warning "'diagnose_as' attribute requires a single NTBS argument" }
  class __attribute__((diagnose_as(1))) Z1; // { dg-warning "'diagnose_as' attribute requires a single NTBS argument" }
  class __attribute__((diagnose_as("a", "b"))) Z2; // { dg-warning "'diagnose_as' attribute requires a single NTBS argument" }

  template <typename T> class A0 {};
  typedef A0<char> Achar __attribute__((diagnose_as("Achar")));
  template class A0<int>;
  typedef A0<int> Aint __attribute__((diagnose_as("Aint"))); // OK
  typedef A0<int> Aint2 __attribute__((diagnose_as("Aint2"))); // { dg-warning "'class foo2::Aint' redeclared with 'diagnose_as\\(\"Aint2\"\\)' attribute" }
  typedef A0<int> Aint3 __attribute__((diagnose_as(1))); // { dg-warning "'diagnose_as' attribute requires 0 or 1 NTBS arguments" }

#if __cplusplus >= 201103L
  template <typename T> using foo [[gnu::diagnose_as]] = A0<T>;
  template <typename T> using aRef [[gnu::diagnose_as]] = A0<const T&>; // { dg-warning "'diagnose_as' attribute ignored" "" { target c++11 } }
#endif
}

namespace A
{
  inline namespace __attribute__((diagnose_as("@1"))) B
  {
    template <typename U>
      struct __attribute__((diagnose_as("@2"))) C
      {
	__attribute__((diagnose_as("fun:1")))
	void f() {} // { dg-line ABC_f }

	template <typename T>
	  __attribute__((diagnose_as("fun:2")))
	  void g(T, U) {} // { dg-line ABC_gT }

	template <typename T>
	  __attribute__((diagnose_as("fun:2.2")))
	  void g() {} // { dg-line ABC_g }

	__attribute__((diagnose_as("fun:3")))
	void h()
	{
#ifdef __cpp_constexpr
	  static_assert(__builtin_strcmp(__FUNCTION__, "fun:3") == 0, "");
	  static_assert(__builtin_strcmp(__func__, "fun:3") == 0, "");
	  constexpr const char* ref
	    = is_int<U>() ? "void @1::@3::fun:3()"
			  : "void @1::@2<U>::fun:3() [with U = char]";
	  static_assert(__builtin_strcmp(__PRETTY_FUNCTION__, ref) == 0, "");
#endif
	}

	template <typename T>
	  __attribute__((diagnose_as("fun:4")))
	  void ht()
	{
#ifdef __cpp_constexpr
	  static_assert(__builtin_strcmp(__FUNCTION__, "fun:4") == 0, "");
	  constexpr const char* ref
	    = is_int<U>()
		? "void @1::@3::fun:4<T>() [with T = float]"
		: "void @1::@2<U>::fun:4<T>() [with T = float; U = char]";
	  static_assert(__builtin_strcmp(__PRETTY_FUNCTION__, ref) == 0, "");
#endif
	}

	template <typename T0, typename T1>
	  struct __attribute__((diagnose_as("@5"))) E
	  {
	    __attribute__((diagnose_as("fun:5"))) static void f() {
#ifdef __cpp_constexpr
	      static_assert(__builtin_strcmp(__FUNCTION__, "fun:5") == 0, "");
	      constexpr const char* ref = is_int<U>()
		? is_char<T0>()
		  ? "static void @1::@3::@6::fun:5()"
		  : "static void @1::@3::@5<T0, T1>::fun:5() [with T0 = float; T1 = short int]"
		: is_char<T0>()
		  ? "static void @1::@2<U>::@6::fun:5() [with U = char]"
		  : "static void @1::@2<U>::@5<T0, T1>::fun:5() [with T0 = float; T1 = short int; U = char]";
	      static_assert(__builtin_strcmp(__PRETTY_FUNCTION__, ref) == 0,
			    "");
#endif
	    }
	  };
	typedef E<char, char> F __attribute__((diagnose_as("@6")));

	template <typename T>
	  struct __attribute__((diagnose_as("@7"))) E<T, long>
	  {
	    __attribute__((diagnose_as("fun:6"))) static void f() {
#ifdef __cpp_constexpr
	      static_assert(__builtin_strcmp(__FUNCTION__, "fun:6") == 0, "");
	      constexpr const char* ref = is_int<U>()
		  ? "static void @1::@3::@7<T, long int>::fun:6() [with T = float]"
		  : "static void @1::@2<U>::@7<T, long int>::fun:6() [with T = float; U = char]";
	      static_assert(__builtin_strcmp(__PRETTY_FUNCTION__, ref) == 0,
			    "");
#endif
	    }
	  };
      };

    typedef C<int> D __attribute__((diagnose_as("@3")));

    template <>
      struct __attribute__((diagnose_as("@4"))) C<float>
      {
      };

    typedef C<short> E __attribute__((diagnose_as));
  }
}

namespace frob
{
  namespace ni
  {
    struct kate {};
    namespace cate
    {
      struct me {};
    }
  }
}

namespace frobni __attribute__((diagnose_as("twiggle"))) = frob::ni;
namespace frobnicate __attribute__((diagnose_as)) = frob::ni::cate;
namespace wrong __attribute__((diagnose_as(1))) = frob::ni; // { dg-warning "'diagnose_as' attribute requires 0 or 1 NTBS arguments" }
namespace wrong2 __attribute__((diagnose_as("wrong", 1))) = frob::ni; // { dg-warning "'diagnose_as' attribute requires 0 or 1 NTBS arguments" }

void fn_1(int);
void fn_2(A::D);

int main ()
{
  fn_2 (A::D ());
  fn_1 (A::D ()); // { dg-error "cannot convert '@1::@3' to 'int'" }
  fn_1 (A::C<int> ()); // { dg-error "cannot convert '@1::@3' to 'int'" }
  fn_1 (A::C<char> ()); // { dg-error "cannot convert '@1::@2<char>' to 'int'" }
  fn_1 (A::C<float> ()); // { dg-error "cannot convert '@1::@4' to 'int'" }
  fn_1 (A::C<short> ()); // { dg-error "cannot convert '@1::E' to 'int'" }
  fn_1 (A::E ()); // { dg-error "cannot convert '@1::E' to 'int'" }
  fn_1 (foo1::A0<int> ()); // { dg-error "cannot convert 'foo2::Aint' to 'int'" }
  fn_1 (foo1::Aint2 ()); // { dg-error "cannot convert 'foo2::Aint2' {aka 'foo2::Aint'} to 'int'" }
#if __cplusplus >= 201103L
  fn_1 (foo1::foo<float> ()); // { dg-error "cannot convert 'foo2::foo<float>' to 'int'" "" { target c++11 } }
  fn_1 (foo1::aRef<float> ()); // { dg-error "cannot convert 'foo2::aRef<float>' {aka 'foo2::foo<const float&>'} to 'int'" "" { target c++11 } }
  fn_1 (frob::ni::kate ()); // { dg-error "cannot convert 'twiggle::kate' to 'int'" "" { target c++11 } }
  fn_1 (frob::ni::cate::me ()); // { dg-error "cannot convert 'frobnicate::me' to 'int'" "" { target c++11 } }
#endif

  A::C<int>().f(0); // { dg-error "no matching function for call to '@1::@3::f\\(int\\)'" }
  // { dg-message "candidate: 'void @1::@3::fun:1\\(\\)'" "" { target *-*-* } ABC_f }

  A::C<char>().f(0); // { dg-error "no matching function for call to '@1::@2<char>::f\\(int\\)'" }
  // { dg-message "candidate: 'void @1::@2<U>::fun:1\\(\\) \\\[with U = char\\\]'" "" { target *-*-* } ABC_f }

  A::C<float>().f(0); // { dg-error "'struct @1::@4' has no member named 'f'" }

  A::C<int>().g(); // { dg-error "no matching function for call to '@1::@3::g\\(\\)'" }
  // { dg-message "candidate: 'template<class T> void @1::@3::fun:2\\(T, U\\)'" "" { target *-*-* } ABC_gT }
  // { dg-message "candidate: 'template<class T> void @1::@3::fun:2.2\\(\\)'" "" { target *-*-* } ABC_g }

  A::C<char>().g(); // { dg-error "no matching function for call to '@1::@2<char>::g\\(\\)'" }
  // { dg-message "candidate: 'template<class T> void @1::@2<U>::fun:2\\(T, U\\) \\\[with U = char\\\]'" "" { target *-*-* } ABC_gT }
  // { dg-message "candidate: 'template<class T> void @1::@2<U>::fun:2.2\\(\\) \\\[with U = char\\\]'" "" { target *-*-* } ABC_g }

  A::C<int>().h();
  A::C<char>().h();
  A::C<int>().ht<float>();
  A::C<char>().ht<float>();
  A::C<int>::E<float, short>::f();
  A::C<char>::E<float, short>::f();
  A::C<int>::E<float, long>::f();
  A::C<char>::E<float, long>::f();
  A::C<int>::F::f();
  A::C<char>::F::f();
}
