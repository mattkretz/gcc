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
    if [[ $testcase == sincos ]]; then
      # The sincos test requires reference data to run
      extra='// { dg-do compile }'
    else
      extra=''
    fi
    filename="${testcase}-${type// /_}"

    cat > "${filename}.cc" <<EOF
// { dg-options "-std=c++17" }
${extra}
#include "tests/${testcase}.h"

int main()
{
  iterate_abis<${type}>();
  return 0;
}
EOF
    cat > "${filename}-constexpr.cc" <<EOF
// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
${extra}
#include "tests/${testcase}.h"

int main()
{
  iterate_abis<${type}>();
  return 0;
}
EOF
    cat > "${filename}-fixed_size.cc" <<EOF
// { dg-options "-std=gnu++17" }
// { dg-require-effective-target run_expensive_tests }
${extra}
#define TESTFIXEDSIZE 1
#include "tests/${testcase}.h"

int main()
{
  iterate_abis<${type}>();
  return 0;
}
EOF
  done
done
