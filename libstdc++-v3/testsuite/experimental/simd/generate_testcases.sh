#!/bin/bash

floattypes=(
"long double"
"double"
"float"
)
alltypes=(
"${floattypes[@]}"
"long long"
"unsigned long long"
"unsigned long"
"long"
"int"
"unsigned int"
"short"
"unsigned short"
"char"
"signed char"
"unsigned char"
"char32_t"
"char16_t"
"wchar_t"
)

archlist=(
arch=native
arch=armv7-a+simd
arch=armv8.1-a+simd
cpu=power7
)

cd ${0%/*}
for testcase in tests/*.h; do
  if grep -q "test only floattypes" "$testcase"; then
    typelist=("${floattypes[@]}")
  else
    typelist=("${alltypes[@]}")
  fi
  testcase=${testcase%.h}
  testcase=${testcase##*/}
  for type in "${typelist[@]}"; do
    for m in "${archlist[@]}"; do
      target=$m
      case "$m" in
        arch=native)
          target="{ x86_64-*-* i?86-*-* }"
	  runtime=sse_runtime
          ;;
        *armv8*)
          target="aarch64-*-*"
          runtime=arm_neon_hw
          ;;
        *+neon*|*+simd*)
          target="armv7*-*-*"
          runtime=arm_neon_hw
          ;;
        cpu=power7)
          target="powerpc64*-*-*"
          runtime=vsx_hw
      esac
      name=${m//arch=/}
      cat > "${testcase}-${type// /_}-${name}.cc" <<EOF
// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=${type}\" -m${m}" }
// { dg-do run { target { c++17 && ${target} } } }
#include "tests/${testcase}.h"
EOF
    cat > "${testcase}-${type// /_}-${name}-fixed_size.cc" <<EOF
// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=${type}\" -DTESTFIXEDSIZE -m${m}" }
// { dg-do run { target { c++17 && ${target} } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/${testcase}.h"
EOF
    done
  done
done
