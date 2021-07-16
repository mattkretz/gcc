/* { dg-do run } */

float a = 1.f;
float b = 1.e20f;

__attribute__((optimize("-ffast-math")))
float
fast()
{
  return __builtin_assoc_barrier (a + b) - b;
}

float
normal()
{
  return a + b - b;
}

int main()
{
  if (fast() != normal())
    __builtin_abort();
  return 0;
}
