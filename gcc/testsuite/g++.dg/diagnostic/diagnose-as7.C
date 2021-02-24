// { dg-options "-fdiagnostics-use-aliases -fpretty-templates" }
// { dg-do compile { target c++11 } }

namespace A
{
  inline namespace B
  {
    template <typename U>
      struct __attribute__((diagnose_as("@2"))) C
      {
	template <typename T>
	  [[gnu::diagnose_as("fun:1")]] void f() {} // { dg-line f }

	template <typename T>
	  [[gnu::diagnose_as("fun:2")]] void g(T) {} // { dg-line g }

	template <typename T>
	  [[deprecated, gnu::diagnose_as("fun:3")]] void h(T) {}

	template <typename T, typename R = void>
	  [[deprecated, gnu::diagnose_as("fun:4")]] R f0(T) { return R(); }
      };

    using D [[gnu::diagnose_as]] = C<int>;
  }
}

namespace AB [[gnu::diagnose_as]] = A::B;

int main()
{
  A::C<int>().f<float>(1); // { dg-error "no matching function for call to 'AB::D::f<float>\\(int\\)'" }
  // { dg-message "candidate: 'template<class T> void AB::D::fun:1\\(\\)'" "" { target *-*-* } f }

  A::C<int>().g<int>(1, 1); // { dg-error "no matching function for call to 'AB::D::g<int>\\(int, int\\)'" }
  // { dg-message "candidate: 'template<class T> void AB::D::fun:2\\(T\\)'" "" { target *-*-* } g }

  A::C<int>().h(1); // { dg-warning "'void AB::D::fun:3\\(T\\) \\\[with T = int\\\]' is deprecated" }

  A::C<int>().f0(1); // { dg-warning "'R AB::D::fun:4\\(T\\) \\\[with T = int; R = void\\\]' is deprecated" }
  A::C<int>().f0<float>(1); // { dg-warning "'R AB::D::fun:4<T>\\(T\\) \\\[with T = float; R = void\\\]' is deprecated" }
  A::C<int>().f0<float, int>(1); // { dg-warning "'R AB::D::fun:4<T, R>\\(T\\) \\\[with T = float; R = int\\\]' is deprecated" }
}
