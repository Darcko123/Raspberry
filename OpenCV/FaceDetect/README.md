# 🧠 Detección de Rostros en Tiempo Real con OpenCV

![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este proyecto implementa un sistema de detección de rostros utilizando clasificadores Haar en cascada de OpenCV, trabajando en tiempo real mediante la cámara web. Es útil para tareas de visión artificial básica, sistemas de vigilancia o como paso inicial para sistemas más complejos de reconocimiento facial.

## 📌 Características principales

- Captura de video en tiempo real desde cámara web
- Detección de rostros usando `haarcascade_frontalface_alt.xml`
- Preprocesamiento mediante conversión a escala de grises y ecualización de histograma
- Dibujo de rectángulos sobre los rostros detectados
- Salida visual en ventana de interfaz gráfica

## 🚀 Cómo ejecutar

1. Copiar los archivos build.sh y CMakeLists.txt
   ### build.sh
   ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./FaceDetect
   ```
   ### CMakeLists.txt
   ```bash
   cmake_minimum_required(VERSION 3.12)
   project(FaceDetect LANGUAGES CXX)

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
   add_executable(FaceDetect FaceDetect.cpp)
   target_include_directories(FaceDetect PRIVATE ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(FaceDetect PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
   ```

2. Dar permisos
    ``` bash
    chmod +x build.sh
    ```
3. Ejecutar
   ```bash
    ./build.sh
   ```

> [!NOTE]
> NOTA: Asegurese de que el archivo `haarcascade_frontalface_alt.xml` esté en el mismo directorio del ejecutable.
> Este archivo se encuentra en el repositorio oficial de OpenCV en data/haarcascades/

## 🧪 Descripción del Código
- Lectura de video: `capture >> imagenOriginal;`
- Conversión a escala de grises: `cvtColor()`
- Ecualización de imagen: `equalizeHist()`
- Detección de clasificador: `detectMulltiScale`

## 🔍 Detalles Técnicos
- *Clasificador Haar*:
    Utiliza un modelo preentrenado de detección de rostros proporcionado por OpenCV.
- *Preprocesamiento*:
    La imagen capturada es convertida a escala de grises y se aplica ecualización de histograma para mejorar la detección:
    ```cpp
        cvtColor(imagen, imagenGris, COLOR_BGR2GRAY);
        equalizeHist(imagenGris, imagenGris);
    ```

## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Clasificadores Harr OpenCV github](https://github.com/opencv/opencv/tree/master/data/haarcascades)