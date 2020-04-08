// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=long double\" -march=native" }
// { dg-do run { target { c++17 && { x86_64-*-* i?86-*-* } } } }
#include "tests/hypot3_fma.h"
