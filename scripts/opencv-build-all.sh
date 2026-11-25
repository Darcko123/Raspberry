#!/usr/bin/env bash
# Compila (sin ejecutar) todos los ejemplos de OpenCV/. Sale con 1 si alguno falla.
set -uo pipefail
cd "$(dirname "$0")/../OpenCV"

fail=0
for cm in */CMakeLists.txt; do
  dir=${cm%/CMakeLists.txt}
  if cmake -S "$dir" -B "$dir/build" > /dev/null && cmake --build "$dir/build" -j"$(nproc)" > /dev/null; then
    echo "OK     $dir"
  else
    echo "FALLO  $dir"
    fail=1
  fi
done
exit $fail
