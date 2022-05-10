// { dg-do compile }

struct A0
{
  friend A0
  operator+(const A0&, const A0&); // { dg-message "candidate:" }
};

template  <typename T>
  struct A1
  {
    friend A1
    operator+(const A1&a, const A1&) { // { dg-message "candidate:" }
      return a;
    }
  };

struct B
{
  operator A0&();
  operator A1<int>&();
  operator int();
};

void test01() {
  B foo;
  foo + foo; // { dg-line line }
  // { dg-error   "ambiguous overload"    "" { target *-*-* } line }
  // { dg-message "candidate: .*built-in" "" { target *-*-* } line }
}
