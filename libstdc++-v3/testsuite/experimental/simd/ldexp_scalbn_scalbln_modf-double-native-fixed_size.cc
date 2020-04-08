// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=double\" -DTESTFIXEDSIZE -march=native" }
// { dg-do run { target { c++17 && { x86_64-*-* i?86-*-* } } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/ldexp_scalbn_scalbln_modf.h"
