#!/bin/bash
rm -rf CMakeFiles CMakeCache.txt
cmake -DCMAKE_CXX_FLAGS="" .
make
./Imagen
