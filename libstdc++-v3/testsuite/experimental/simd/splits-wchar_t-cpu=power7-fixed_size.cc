// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=wchar_t\" -DTESTFIXEDSIZE -mcpu=power7" }
// { dg-do run { target { c++17 && powerpc64*-*-* } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/splits.h"
