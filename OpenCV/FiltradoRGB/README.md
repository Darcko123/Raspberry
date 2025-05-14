# Ejemplo de Filtro RGB en OpenCV
![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)
Este repositorio contiene un ejemplo de cómo aplicar un filtro RGB a una imagen utilizando la librería OpenCV en C++. El código separa la imagen en sus componentes de color (rojo, verde y azul) y muestra cada canal por separado, destacando el color dominante en cada píxel.

## 📌 Características principales
- Carga de una imagen utilizando la función imread de OpenCV.
- Separación de la imagen en sus canales de color rojo, verde y azul.
- Identificación del color dominante en cada píxel.
- Visualización de la imagen original y los canales de color resultantes.
- Código bien comentado y estructurado.

## 🚀 Cómo ejecutar
1. Copiar los archivos build.sh y CMakeLists.txt al mismo directorio que el archivo fuente (.cpp).
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
    project(FiltroRGB LANGUAGES CXX)

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
    add_executable(FiltroRGB FiltroRGB.cpp)
    target_include_directories(FiltroRGB PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(FiltroRGB PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
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
- **Carga la imagen:** Utiliza la función `imread()` para cargar la imagen "avatares.jpg" en formato color.
- **Verifica la carga:** Comprueba si la imagen se cargó correctamente. Si no, muestra un mensaje de error y termina la ejecución.
- **Crea matrices para los canales de color:** Se crean tres matrices (`rojo`, `verde`, `azul`) del mismo tamaño y tipo que la imagen original, inicializadas con valores de cero (`negro`).
- **Procesa cada píxel:** 
  - Itera sobre cada fila y columna de la imagen.
  - Obtiene los valores de los componentes azul, verde y rojo del píxel actual.
  - Compara los valores de los componentes para determinar el color dominante.
  - Asigna el valor del componente dominante a la matriz correspondiente (rojo, verde o azul), y los otros componentes se establecen en cero. Si no hay un color dominante claro, el píxel se deja en negro.
- **Muestra las imágenes:** Utiliza la función `imshow()` para mostrar la imagen original y las imágenes resultantes de los canales rojo, verde y azul en ventanas separadas.
- **Espera la pulsación de una tecla:** La función `waitKey(0)` hace que el programa espere hasta que se presione una tecla, manteniendo las ventanas visibles.
- 
## 📸 Resultados esperados
Al ejecutar el programa, se mostrarán cuatro ventanas:
1. "Imagen original": La imagen cargada.
2. "Canal Rojo": La imagen filtrada donde solo se muestra el componente rojo dominante en cada píxel.
3. "Canal Verde": La imagen filtrada donde solo se muestra el componente verde dominante en cada píxel.
4. "Canal Azul": La imagen filtrada donde solo se muestra el componente azul dominante en cada píxel.

## 🖥️ Uso
1. Asegúrate de que el archivo `avatares.jpg` esté en el mismo directorio que el ejecutable.
2. Ejecuta el programa.
3. Se mostrarán las cuatro ventanas con las imágenes.
4. Presiona cualquier tecla para cerrar las ventanas y finalizar el programa.
   
## 📚 Recursos de aprendizaje
- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Clase Mat](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)
- [Documentación oficial de OpenCV - Vec3b](https://docs.opencv.org/4.x/d6/dcf/classcv_1_1Vec.html#a72280b48a5d716939114289dc0930ba6)