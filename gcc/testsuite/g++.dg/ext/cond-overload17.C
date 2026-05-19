// { dg-do run }
// { dg-options "-std=c++20" }
//
// P2600 scenario: without name lookup for operator?:, ADL for operator?:
// is never performed, so Wrap<Incomplete> is not instantiated and the code
// is well-formed. With -foverloadable-conditional (default), make sure that no
// name lookup / ADL is attempted for a built-in conditional operator, thus
// keeping backwards compatibility.

struct Incomplete;
template <typename T> struct Wrap { T t; };

template <typename Unused>
  struct Testable
  {
    explicit operator bool () const { return true; }
  };

struct A
{
  operator int() const { return 1; }
};

struct B
{
  explicit operator int() const { return 2; }
};

int operator?:(bool, A, B) = default;

int
main ()
{
  Testable<Wrap<Incomplete>> l;
  // If the following ?: triggers lookup -> ADL -> Testable<Incomplete> would
  // make the program ill-formed
  int x = l ? A() : 2;
  if (x != 1)
    __builtin_abort ();

  x = true ? A() : B();
  if (x != 1)
    __builtin_abort ();
  return 0;
}
