#!/usr/bin/env bash
# Comprueba los ejemplos de OpenCV/ y los compila (sin ejecutarlos). Sale con 1 si algo falla:
#  - regla de nombres: carpeta = <carpeta>.cpp = project() del CMakeLists.txt
#  - cabecera Doxygen: cada <carpeta>.cpp empieza con '@file <carpeta>.cpp'
#  - encabezados ## y ### de cada README.md = los de _PLANTILLA/README.md, en el mismo orden
#  - enlaces relativos rotos en los .md de OpenCV/
#  - compilación de cada ejemplo, sin avisos (los avisos cuentan como error)
set -uo pipefail
cd "$(dirname "$0")/../OpenCV"

fail=0
for cm in */CMakeLists.txt; do
  dir=${cm%/CMakeLists.txt}
  name=$(sed -n 's/^project(\([A-Za-z0-9_]*\).*/\1/p' "$cm")
  if [ "$name" != "$dir" ] || [ ! -f "$dir/$dir.cpp" ]; then
    echo "NOMBRE $dir (project: '$name', se espera $dir/$dir.cpp)"
    fail=1
  fi
  if ! grep -q "@file $dir.cpp" "$dir/$dir.cpp" 2>/dev/null; then
    echo "CABECERA $dir (falta '@file $dir.cpp' en $dir/$dir.cpp)"
    fail=1
  fi
  if [ "$dir" != _PLANTILLA ] &&
     ! diff <(grep -E '^###? ' _PLANTILLA/README.md) <(grep -E '^###? ' "$dir/README.md" 2>/dev/null) > /dev/null; then
    echo "README $dir (encabezados distintos de los de _PLANTILLA/README.md)"
    fail=1
  fi
done

while IFS= read -r md; do
  # Destinos de [texto](destino) que no son URL ni ancla, sin el #ancla
  while IFS= read -r link; do
    if [ ! -e "$(dirname "$md")/$link" ]; then
      echo "ENLACE $md -> $link"
      fail=1
    fi
  done < <(grep -oE '\]\([^)]+\)' "$md" | sed -E 's/^\]\(//; s/\)$//; s/#.*//' | grep -vE '^(https?:|mailto:|$)')
done < <(find . -name '*.md' -not -path '*/build/*')

for cm in */CMakeLists.txt; do
  dir=${cm%/CMakeLists.txt}
  out=
  # Los avisos se buscan en la salida: un -Werror quedaría en la caché de build/ y afectaría a build.sh
  if cmake -S "$dir" -B "$dir/build" > /dev/null && out=$(cmake --build "$dir/build" -j"$(nproc)" 2>&1) &&
     ! grep -q 'warning:' <<< "$out"; then
    echo "OK     $dir"
  else
    echo "FALLO  $dir"
    grep -E 'warning:|error:' <<< "$out"
    fail=1
  fi
done
exit $fail
