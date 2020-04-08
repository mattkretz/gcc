// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=int\" -DTESTFIXEDSIZE -march=armv7-a+simd" }
// { dg-do run { target { c++17 && armv7*-*-* } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/simd.h"
