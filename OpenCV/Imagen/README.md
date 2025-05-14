# Ejemplo de Carga de Imagen en OpenCV
![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este repositorio contiene un ejemplo sencillo de cómo cargar una imagen utilizando la librería OpenCV en C++. El código muestra cómo leer una imagen desde un archivo y verificar si la carga fue exitosa.

## 📌 Características principales
- Carga de imágenes utilizando la función `imread` de OpenCV.
- Verificación de la correcta carga de la imagen con mensajes de error descriptivos.
- Visualización de la imagen cargada en una ventana.
- Código bien comentado y fácil de entender.

## 🚀 Cómo ejecutar
1. Copiar los archivos `build.sh` y `CMakeLists.txt` al mismo directorio que el archivo fuente (`.cpp`).
    ### build.sh
    ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./Imagen
    ```

    ### CMakeLists.txt
    ```bash
    cmake_minimum_required(VERSION 3.12)
    project(Imagen LANGUAGES CXX)

    # Establecer flags del compilador (puede ser útil en la Raspberry Pi Zero 2W)
    set(CMAKE_CXX_FLAGS "" CACHE STRING "Flags for C++ compiler" FORCE)

    # Configuración de OpenCV
    find_package(OpenCV 4.7.0 REQUIRED)
    message(STATUS "OpenCV version: ${OpenCV_VERSION}")

    # Buscar wiringPi (opcional, puedes eliminarlo si no lo usas)
    find_library(WIRINGPI_LIB wiringPi)
    if(WIRINGPI_LIB)
        message(STATUS "Found wiringPi library: ${WIRINGPI_LIB}")
    else()
        message(WARNING "wiringPi library not found. Proceeding without it.")
    endif()

    # Crear ejecutable
    add_executable(Imagen Imagen.cpp)
    target_include_directories(Imagen PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(Imagen PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
    ```

2. Asegúrate de tener una imagen llamada avatares.jpg en el mismo directorio que los archivos. Puedes reemplazar este nombre por el de tu imagen
   
3. Dar permisos de ejecución al script build.sh:
    ``` bash
    chmod +x build.sh
    ```

4. Ejecutar
    ```bash
    ./build.sh
    ```

## Estructura del código
El programa realiza las siguientes acciones principales:

- **Incluye las cabeceras necesarias:** Se incluyen las librerías de entrada/salida estándar (`iostream`) y las principales de OpenCV (`opencv2/opencv.hpp`, `opencv2/highgui.hpp`, `opencv2/imgproc.hpp`, `opencv2**/imgcodecs.hpp`).
- **Carga la imagen:** Utiliza la función `imread()` para leer la imagen llamada `avatares.jpg`. El segundo argumento `IMREAD_COLOR` indica que se cargue la imagen a color.
- **Verifica la carga:** Comprueba si la matriz `img` está vacía. Si lo está, significa que la imagen no se pudo cargar y se muestra un mensaje de error.
- **Muestra la imagen:** Si la carga es exitosa, la función `imshow()` muestra la imagen en una ventana con el título "Imagen avatares".
- **Espera una pulsación de tecla:** `waitKey(0)` detiene la ejecución del programa hasta que se presiona cualquier tecla, cerrando así la ventana de la imagen.

## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Función waitKey](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga5628525ad33f52eab17feebcfba38bd7)
