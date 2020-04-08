// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=long long\" -DTESTFIXEDSIZE -march=armv7-a+simd" }
// { dg-do run { target { c++17 && armv7*-*-* } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/split_concat.h"
