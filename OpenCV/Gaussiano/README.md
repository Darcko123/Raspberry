# Ejemplo de Filtro RGB en OpenCV

![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

Este repositorio contiene un ejemplo de cómo aplicar un filtro Gaussiano a una imagen utilizando la librería OpenCV en C++. El filtro Gaussiano es una técnica de suavizado que reduce el ruido y los detalles finos de una imagen, preservando mejor los bordes que otros métodos de suavizado.

## Características principales
- Carga de una imagen utilizando la función `imread` de OpenCV.
- Aplicación de un filtro Gaussiano utilizando la función `GaussianBlur` de OpenCV.
- Visualización de la imagen original y la imagen filtrada.
- Código bien comentado y estructurado.

## Cómo ejecutar
1. Copiar los archivos `build.sh` y `CMakeLists.txt` al mismo directorio que el archivo fuente (`.cpp`).
    ### build.sh
    ```bash
    #!/bin/bash
    rm -rf CMakeFiles CMakeCache.txt
    cmake -DCMAKE_CXX_FLAGS="" .
    make
    ./FiltroRGB
    ```
    ### CMakeLists.txt 
    ```bash
    cmake_minimum_required(VERSION 3.12)
    project(FiltroGaussiano LANGUAGES CXX)

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
    add_executable(FiltroGaussiano FiltroGaussiano.cpp)
    target_include_directories(FiltroGaussiano PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(FiltroGaussiano PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
    ```
2. Asegúrate de tener una imagen llamada `avatares.jpg` en el mismo directorio que los archivos. Puedes reemplazar este nombre por el de tu imagen
   
3. Dar permisos de ejecución al script `build.sh`:
    ``` bash
    chmod +x build.sh
    ```

4. Ejecutar
    ```bash
    ./build.sh
    ```

## Estructura del código
El programa realiza las siguientes acciones principales:
- **Carga la imagen:** Lee la imagen "avatares.jpg" y la almacena en la matriz imageOriginal.
- **Verifica la carga:** Comprueba si la imagen se cargó correctamente. Si no, muestra un mensaje de error y termina la ejecución.
- **Aplica el filtro Gaussiano:** 
  - Utiliza la función `GaussianBlur` de OpenCV para aplicar el filtro.
  - El tamaño del kernel del filtro es de 3x3.
  - Los valores de sigma X e Y se calculan automáticamente.
  - Se utiliza el modo de borde `BORDER_DEFAULT`.
- **Muestra las imágenes:** 
  - Muestra la imagen original en una ventana llamada "Imagen Original".
  - Muestra la imagen filtrada en una ventana llamada "Filtro Gaussiano - Suavizado de Imagen".
- **Espera la pulsación de una tecla:** El programa espera a que el usuario presione una tecla para finalizar y cerrar las ventanas.
  
## Resultados esperados
Al ejecutar el programa, se mostrarán dos ventanas:
  - "Imagen Original": La imagen cargada desde el archivo.

  - "Filtro Gaussiano - Suavizado de Imagen": La imagen resultante después de aplicar el filtro Gaussiano. El ruido y los detalles finos se reducirán, y la imagen se verá más borrosa.

## Uso
1. Asegúrate de que el archivo `avatares.jpg` esté en el mismo directorio que el ejecutable.
2. Ejecuta el programa.
3. Se mostrarán las dos ventanas con las imágenes.
4. Presiona cualquier tecla para cerrar las ventanas y finalizar el programa.
   
## Recursos de aprendizaje
- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Función GaussianBlur](https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#gae8bdcd9154ed5ca3cbc1766d960f45c1)