// { dg-options "-fdiagnostics-use-aliases -fno-pretty-templates" }

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

namespace A
{
  inline namespace B __attribute__((diagnose_as("@1")))
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
	  constexpr const char* ref
	    = is_int<U>() ? "void @1::@3::fun:3()"
			  : "void @1::@2<char>::fun:3()";
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
	    = is_int<U>() ? "void @1::@3::fun:4<float>()"
			  : "void @1::@2<char>::fun:4<float>()";
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
		  : "static void @1::@3::@5<float, short int>::fun:5()"
		: is_char<T0>()
		  ? "static void @1::@2<char>::@6::fun:5()"
		  : "static void @1::@2<char>::@5<float, short int>::fun:5()";
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
	      // Note that @7 is ignored with -fno-pretty-templates. After all
	      // diagnose_as on partial specializations is not supported.
	      constexpr const char* ref = is_int<U>()
		  ? "static void @1::@3::@5<float, long int>::fun:6()"
		  : "static void @1::@2<char>::@5<float, long int>::fun:6()";
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
  }
}

void fn_1(int);
void fn_2(A::D);

int main ()
{
  fn_2 (A::D ());
  fn_1 (A::D ()); // { dg-error "cannot convert '@1::@3' to 'int'" }
  fn_1 (A::C<int> ()); // { dg-error "cannot convert '@1::@3' to 'int'" }
  fn_1 (A::C<char> ()); // { dg-error "cannot convert '@1::@2<char>' to 'int'" }
  fn_1 (A::C<float> ()); // { dg-error "cannot convert '@1::@4' to 'int'" }

  A::C<int>().f(0); // { dg-error "no matching function for call to '@1::@3::f\\(int\\)'" }
  // { dg-message "candidate: 'void @1::@3::fun:1\\(\\)'" "" { target *-*-* } ABC_f }

  A::C<char>().f(0); // { dg-error "no matching function for call to '@1::@2<char>::f\\(int\\)'" }
  // { dg-message "candidate: 'void @1::@2<char>::fun:1\\(\\)'" "" { target *-*-* } ABC_f }

  A::C<float>().f(0); // { dg-error "'struct @1::@4' has no member named 'f'" }

  A::C<int>().g(); // { dg-error "no matching function for call to '@1::@3::g\\(\\)'" }
  // { dg-message "candidate: 'template<class T> void @1::@3::fun:2\\(T, int\\)'" "" { target *-*-* } ABC_gT }
  // { dg-message "candidate: 'template<class T> void @1::@3::fun:2.2\\(\\)'" "" { target *-*-* } ABC_g }

  A::C<char>().g(); // { dg-error "no matching function for call to '@1::@2<char>::g\\(\\)'" }
  // { dg-message "candidate: 'template<class T> void @1::@2<char>::fun:2\\(T, char\\)'" "" { target *-*-* } ABC_gT }
  // { dg-message "candidate: 'template<class T> void @1::@2<char>::fun:2.2\\(\\)'" "" { target *-*-* } ABC_g }

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
