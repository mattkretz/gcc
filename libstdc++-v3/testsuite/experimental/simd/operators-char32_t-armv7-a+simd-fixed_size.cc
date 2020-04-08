// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=char32_t\" -DTESTFIXEDSIZE -march=armv7-a+simd" }
// { dg-do run { target { c++17 && armv7*-*-* } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/operators.h"
