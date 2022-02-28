class B0 {};

namespace A
{
    class B : public ::B0 {};
    void f(B*);
}

void f(void*);
void f(B0*);

template<class T>
  class C
  : public A::B {
  };

void g(C<int>* p)
{
  // no instantiation on ADL => candidates ::f(void*) and ::f(B0*)
  // instantiation on overload resolution => ::f(B0*)
  f(p); // { dg-final { scan-assembler-times "_Z1fP2B0" 1 } }

  // C<int> was instantiated on overload resolution, thus ADL finds A::f
  f(p); // { dg-final { scan-assembler-times "_ZN1A1fEPNS_1BE" 1 } }

  // { dg-final { scan-assembler-not "_Z1fPv" } }
}
