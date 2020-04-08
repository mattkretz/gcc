// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=unsigned short\" -march=armv7-a+simd" }
// { dg-do run { target { c++17 && armv7*-*-* } } }
#include "tests/mask_broadcast.h"
