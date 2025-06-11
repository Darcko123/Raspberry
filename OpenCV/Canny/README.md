# Filtro Canny Interactivo con OpenCV

![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este proyecto muestra cómo aplicar el filtro *Canny* para detección de bordes en imágenes, permitiendo experimentar en tiempo real con los umbrales bajos y altos mediante *trackbars*. Está pensado para propósitos educativos y de prueba visual de parámetros en procesamiento de imágenes.

## 📌 Características principales

- Conversión de imagen a escala de grises
- Aplicación de filtro Gaussiano para suavizado
- Detección de bordes mediante el filtro Canny
- Interfaz interactiva con trackbars para ajustar:
  - Umbral bajo
  - Umbral alto
- Visualización simultánea de todas las etapas

## 🚀 Cómo ejecutar
1. Copiar los archivos build.sh y CMakeLists.txt
   ### build.sh
   ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./CannyFilter
   ```
   ### CMakeLists.txt
   ```bash
   cmake_minimum_required(VERSION 3.12)
   project(CannyFilter LANGUAGES CXX)

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
   add_executable(CannyFilter CannyFilter.cpp)
   target_include_directories(CannyFilter PRIVATE ${OpenCV_INCLUDE_DIRS})
   target_link_libraries(CannyFilter PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
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
> NOTA: Asegurese de que el archivo `Lenna.png` esté en el mismo directorio del ejecutable

## 🧪 Descripción del Código
### 🔹 Preprocesamiento
- Lectura de imagen: `imread()`
- Conversión a escala de grises: `cvtColor()`
- Suavizado con filtro Gaussiano: `GaussianBlur()`
  
### 🔹 Filtro Canny
- Se usa cv::Canny() con valores ajustables en tiempo real mediante trackbars:
    ```cpp
        Canny(imagen, salida, umbralBajo, umbralAlto, 3);
    ```

### 🔹 Interfaz con Trackbars
- Se emplean dos barras deslizantes para modificar los umbrales de Canny:
    ```cpp
        createTrackbar("Umbral Bajo Canny", "Config", &cannyLow, 255);
        createTrackbar("Umbral Alto Canny", "Config", &cannyHigh, 255);
    ```

### 🔹 Ventanas de Visualización
Se muestran las siguientes ventanas:

- `Imagen Original`
- `Imagen en Escala de Grises`
- `Filtro Gaussiano`
- `Filtro Canny`
- `Config` (controles)

## 🎮 Controles
- Mueve los sliders para ajustar los valores de umbral bajo y alto del filtro Canny.
- Presiona ESC para salir del programa.

## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [OpenCV Canny Edge Detection](https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html)