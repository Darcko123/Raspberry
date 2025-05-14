# Detección de Bordes con Filtro Laplaciano en OpenCV

![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este programa demuestra el uso del operador Laplaciano para la detección de bordes en imágenes.

## 📌 Características principales

- Aplicación de filtro Gaussiano para reducción de ruido
- Conversión a escala de grises
- Detección de bordes con operador Laplaciano
- Visualización del resultado
- Código bien comentado y estructurado
## 🚀 Cómo ejecutar
1. Copiar los archivos build.sh y CMakeLists.txt
   ### build.sh
   ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./FiltroLaplaciano
   ```
   ### CMakeLists.txt
   ```bash
   cmake_minimum_required(VERSION 3.12)
   project(FiltroLaplaciano LANGUAGES CXX)

   # Establecer flags del compilador (puede ser útil en la Raspberry Pi Zero 2W)
   set(CMAKE_CXX_FLAGS "" CACHE STRING "Flags for C++ compiler" FORCE)

   # Configuración de OpenCV
   find_package(OpenCV 4.7.0 REQUIRED)
   message(STATUS "OpenCV version: ${OpenCV_VERSION}")

   # Buscar wiringPi
   find_library(WIRINGPI_LIB wiringPi)
   if(WIRINGPI_LIB)
       message(STATUS "Found wiringPi library: ${WIRINGPI_LIB}")
   else()
       message(WARNING "wiringPi library not found. Proceeding without it.")
   endif()

   # Crear ejecutable
   add_executable(FiltroLaplaciano FiltroLaplaciano.cpp)
   target_include_directories(FiltroLaplaciano PRIVATE ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(FiltroLaplaciano PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
   ```

2. Dar permisos

    ``` bash
    chmod +x build.sh
    ```
3. Ejecutar
   ```bash
    ./build.sh
   ```

## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Laplace Operator](https://docs.opencv.org/3.4/d5/db5/tutorial_laplace_operator.html)
