// { dg-do compile }

template <class T>
  struct A
  {
    friend void
    bar(const A&)
    {}
  };

const A<int>& foo();

void
test01()
{
  bar(foo());
}
