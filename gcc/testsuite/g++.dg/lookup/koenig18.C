// { dg-do compile }

class Incomplete;

template <class T>
  class Wrap {
    T x;
  };

void
f(Wrap<Incomplete>* p)
{}

void
test01()
{
  Wrap<Incomplete>* p = 0;
  f(p);
}
