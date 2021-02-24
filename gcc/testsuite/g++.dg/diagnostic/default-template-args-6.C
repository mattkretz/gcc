// { dg-options "-fpretty-templates" }
// { dg-do compile { target c++11 } }

template <class a, class b = int>
  struct c;

template <class T>
  struct c<T, char>
  {
    template <class d, class e = int>
      [[deprecated]] static void g(d);
  };

int main()
{
  c<int, char>::g(1.); // { dg-warning "'static void c<T, char>::g\\(d\\) .with d = double; T = int.'" }
}
