// { dg-options "-fdiagnostics-use-aliases -fpretty-templates" }
// { dg-do compile { target c++11 } }

template <class T, class U>
  struct A
  {
    struct X {};
    using Y [[gnu::diagnose_as]] = X;
  };

template <>
  struct [[gnu::diagnose_as("A!")]] A<int, int>
  {
    struct X {};
    using Y [[gnu::diagnose_as]] = X;
  };

void f(int);

template <class T, class U>
  using B [[gnu::diagnose_as]] = A<T, U>;

// this is not a real alias template
template <class T>
  using C [[gnu::diagnose_as]] = A<int, int>; // { dg-warning "'diagnose_as' attribute ignored" }

// Would the following request A<float, int> to be diagnosed as D<int>?
// What about `= A<float, const T>', etc? This is not worth the implementation
// complexity.
template <class T>
  using D [[gnu::diagnose_as]] = A<float, T>; // { dg-warning "'diagnose_as' attribute ignored" }

template <class T, class U>
  struct E {};
template <class T, class U>
  using F [[gnu::diagnose_as("EF")]] = E<U, T>; // { dg-warning "'diagnose_as' attribute ignored" }

int main()
{
  f(A<char, int>()); // { dg-error "cannot convert 'B<char, int>' to 'int'" }
  f(A<char, int>::X()); // { dg-error "cannot convert 'B<char, int>::Y' to 'int'" }
  f(B<int, int>::X()); // { dg-error "cannot convert 'A!::Y' to 'int'" }
  f(C<float>::X()); // { dg-error "cannot convert 'A!::Y' to 'int'" }
  f(F<char, int>()); // { dg-error "cannot convert 'F<char, int>' {aka 'E<int, char>'} to 'int'" }
}
