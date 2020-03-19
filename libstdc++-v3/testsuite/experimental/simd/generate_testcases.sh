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
sse sse2 sse3 ssse3 sse4
avx avx2
arch=knl
arch=skylake-avx512
arch=pentium3
arch=armv7-a+simd
arch=armv7ve+simd
arch=armv8-a+simd
arch=armv8.1-a+simd
arch=armv8.2-a+simd
arch=armv8.3-a+simd
arch=armv8.4-a+simd
arch=armv8-r+simd
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
      runtime=${m}_runtime
      case "$m" in
        *knl)
          target=avx512f
          runtime=avx512f_runtime
          ;;
        *skylake-avx512)
          target='{ avx512bw && { avx512vl && avx512dq } }'
          runtime='{ avx512bw_runtime && { avx512vl_runtime && avx512dq_runtime } }'
          ;;
        *pentium3)
          target="{ia32 && sse}"
          runtime=sse_runtime
          ;;
        *armv8*)
          target=aarch64_little_endian
          runtime=arm_neon_hw
          ;;
        *+neon*|*+simd*)
          target=arm_neon
          runtime=arm_neon_hw
          ;;
      esac
      cat > "${testcase}-${type// /_}-${m// /_}.cc" <<EOF
// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=${type}\" -m${m}" }
// { dg-do run { target { c++17 && ${target} } } }
// { dg-xfail-run-if "expected SIGILL" { ! ${runtime} } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/${testcase}.h"
EOF
    done
    cat > "${testcase}-${type// /_}-native.cc" <<EOF
// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=${type}\" -march=native" }
// { dg-do run { target { c++17 && sse } } }
#include "tests/${testcase}.h"
EOF
    cat > "${testcase}-${type// /_}-native-fixed_size.cc" <<EOF
// { dg-options "-O2 -std=gnu++17 \"-DTESTTYPE=${type}\" -DTESTFIXEDSIZE -march=native" }
// { dg-do run { target { c++17 && sse } } }
// { dg-require-effective-target run_expensive_tests }
#include "tests/${testcase}.h"
EOF
  done
done
