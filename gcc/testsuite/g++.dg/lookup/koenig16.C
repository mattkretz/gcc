// { dg-do run }

class Incomplete;

template <class T>
  class Wrap {
    T x;

  public:
    template <template <class> class U>
      friend bool operator!(const U<Wrap<T> >&)
      { return true; }
  };

template <class T>
  class Ptr
  {
    T* x;

  public:
    operator bool() const
    { return true; }
  };

void
test01()
{
  Ptr<Wrap<Incomplete> > ptr;
  if (!ptr)
    __builtin_abort();
}

class Incomplete {};

void
test02()
{
  Ptr<Wrap<Incomplete> > ptr;
  if (!ptr)
    __builtin_abort();

  Wrap<Incomplete> foo; // inverts the result of !ptr
  if (!!ptr)
    __builtin_abort();
}

int main()
{
  test01();
  test02();
  return 0;
}

