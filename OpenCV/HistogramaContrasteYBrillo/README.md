# Ajuste de Brillo/Contraste con Visualización de Histograma en OpenCV

![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Aplicación interactiva que permite ajustar brillo y contraste de una imagen en escala de grises mientras visualiza su histograma en tiempo real.

## 📌 Características principales

- Ajuste interactivo de brillo y contraste mediante trackbars
- Visualización del histograma actualizado en tiempo real
- Implementación de fórmula profesional para transformación de imagen
- Interfaz simple con dos ventanas (imagen + histograma)
  
## 🚀 Cómo ejecutar
1. Copiar los archivos build.sh y CMakeLists.txt
   ### build.sh
   ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./Histograma
   ```
   ### CMakeLists.txt
   ```bash
   cmake_minimum_required(VERSION 3.12)
   project(Histograma LANGUAGES CXX)

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
   add_executable(Histograma Histograma.cpp)
   target_include_directories(Histograma PRIVATE ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(Histograma PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
   ```

2. Dar permisos

    ``` bash
    chmod +x build.sh
    ```
3. Ejecutar
   ```bash
    ./build.sh
   ```


## Controles

- **Trackbar "Brillo"**: Ajusta el brillo (100 = neutral)
- **Trackbar "Contraste"**: Ajusta el contraste (100 = neutral)

## Fórmula utilizada

El programa implementa una transformación lineal basada en el trabajo de Werner D. Streidt:
- Para contraste > 100: comprime el rango alrededor del centro (128)
- Para contraste < 100: expande el rango desde el centro

## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Trackbar en OpenCV](https://docs.opencv.org/3.4/da/d6a/tutorial_trackbar.html)