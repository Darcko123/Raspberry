# Ejemplo de Regiones de Interés (ROI) en OpenCV
![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este repositorio contiene un ejemplo completo de cómo trabajar con Regiones de Interés (ROI) en OpenCV usando C++. El código demuestra cuatro métodos diferentes para definir y extraer ROI de una imagen.

## Características principales
- Selección manual de ROI interactiva
- Definición de ROI poligonal (trapecio)
- Extracción de ROI rectangular
- Creación de ROI circular
- Generación de imágenes de resultado para cada método
- Código bien comentado y estructurado

## Cómo ejecutar
1. Copiar los archivos build.sh y CMakeLists.txt
   ### build.sh
   ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./ROI
   ```
   ### CMakeLists.txt
   ```bash
   cmake_minimum_required(VERSION 3.12)
   project(ROI LANGUAGES CXX)

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
   add_executable(ROI ROI.cpp)
   target_include_directories(ROI PRIVATE ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(ROI PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
   ```

2. Dar permisos

    ``` bash
    chmod +x build.sh
    ```
3. Ejecutar
   ```bash
    ./build.sh
   ```

## Estructura del código
El programa contiene cuatro métodos principales para trabajar con ROI:
1. ROI Manual: Permite seleccionar interactivamente un área rectangular con el mouse.
2. ROI Poligonal: Define un trapecio como región de interés.
3. ROI Rectangular: Extrae un rectángulo en posición y tamaño específicos.
4. ROI Circular: Crea una región circular centrada en la imagen.

## Resultados esperados
El programa generará cuatro archivos de imagen:
1. _roi_image_target.jpg_ - ROI seleccionada manualmente
2. _roi_image_Polygon.jpg_ - ROI poligonal (trapecio)
3. _roi_image_Rectangular.jpg_ - ROI rectangular
4. _roi_image_Circular.jpg_ - ROI circular

## Uso interactivo
1. Al ejecutar, primero se mostrará la imagen completa.
2. Para la ROI manual:
    - Selecciona un área rectangular con el mouse
    - Presiona ESPACIO o ENTER para confirmar
    - Presiona ESC para cancelar
3. Las otras ROI se mostrarán automáticamente.
4. Presiona cualquier tecla para avanzar entre resultados.
5. Presiona ESC para salir del programa.
   
## Recursos de aprendizaje
- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Conceptos básicos de ROI](https://learnopencv.com/how-to-select-a-bounding-box-roi-in-opencv-cpp-python/)