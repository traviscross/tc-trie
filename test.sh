#!/bin/sh
export ASAN_OPTIONS="abort_on_error=1"
export ASAN_OPTIONS="$ASAN_OPTIONS:detect_leaks=1"
#export ASAN_OPTIONS="$ASAN_OPTIONS:log_path=/tmp/asan"
export ASAN_SYMBOLIZER_PATH="$(which llvm-symbolizer-3.5)"
./test
