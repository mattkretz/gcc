// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=long\" -DTESTFIXEDSIZE -march=armv8.1-a+simd" }
// { dg-do run { target { c++17 && aarch64-*-* } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/mask_conversions.h"
