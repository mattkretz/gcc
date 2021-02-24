// { dg-options "-fdiagnostics-use-aliases -fpretty-templates" }
// { dg-do compile { target c++11 } }

namespace std
{
  template <class T, class A>
    class basic_string {};

  namespace pmr
  {
    struct P {};
    using string [[gnu::diagnose_as]] = basic_string<char, P>;
  }
}

void f(int);

int main()
{
  f(std::pmr::string()); // { dg-error "cannot convert 'std::pmr::string' to 'int'" }
}
