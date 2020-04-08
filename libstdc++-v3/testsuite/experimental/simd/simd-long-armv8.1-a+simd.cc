// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=long\" -march=armv8.1-a+simd" }
// { dg-do run { target { c++17 && aarch64-*-* } } }
#include "tests/simd.h"
