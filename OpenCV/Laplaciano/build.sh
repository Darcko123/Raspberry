#!/usr/bin/env bash
# Compila el ejemplo fuera del código fuente (en build/) y lo ejecuta.
set -euo pipefail
cd "$(dirname "$0")"
target=$(sed -n 's/^project(\([A-Za-z0-9_]*\).*/\1/p' CMakeLists.txt)
cmake -S . -B build
cmake --build build -j"$(nproc)"
cd build
exec "./$target"
