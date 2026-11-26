# Comprobación de la instalación de OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Comprobación de la instalación de OpenCV](#comprobación-de-la-instalación-de-opencv)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Descripción](#descripción)
  - [Características](#características)
  - [Requisitos](#requisitos)
    - [Hardware](#hardware)
    - [Software](#software)
  - [Compilación y Prueba](#compilación-y-prueba)
    - [Dependencias](#dependencias)
    - [Compilar](#compilar)
    - [Ejecutar](#ejecutar)
    - [Resultado esperado](#resultado-esperado)
    - [Verificación](#verificación)
  - [Teoría de Visión Embebida](#teoría-de-visión-embebida)
    - [Problema que resuelve](#problema-que-resuelve)
    - [Fundamentos](#fundamentos)
      - [La imagen digital](#la-imagen-digital)
      - [`cv::Mat`, la estructura central de OpenCV](#cvmat-la-estructura-central-de-opencv)
      - [Cabeceras frente a biblioteca](#cabeceras-frente-a-biblioteca)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Primer ejemplo de la colección: un programa mínimo que imprime la versión de OpenCV con la que se compiló. Sirve para comprobar que la instalación de OpenCV, CMake y el compilador funcionan antes de pasar a los ejemplos con imágenes.

## Características

- Solo enlaza el módulo `core` de OpenCV.
- No abre ventanas ni carga archivos: funciona por SSH sin display.
- Permite confirmar qué versión de OpenCV encuentra CMake cuando hay varias instaladas (por ejemplo, la 4.6 de apt y una 4.9 compilada desde el código fuente).

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- No necesita pantalla.

### Software

- OpenCV 4.x con el módulo `core` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
- CMake 3.12 o superior y un compilador de C++11.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/HelloWorld
./build.sh
```

[`build.sh`](build.sh) configura [`CMakeLists.txt`](CMakeLists.txt) en `build/`, compila en modo Release (`-O3`) y lanza el programa. Para compilar sin ejecutar:

```bash
cmake -S . -B build
cmake --build build -j"$(nproc)"
```

### Ejecutar

```bash
cd build
./HelloWorld
```

### Resultado esperado

El programa imprime una sola línea y termina:

```
OpenCV version: 4.6.0
```

El número depende de la instalación: `4.6.0` con el `libopencv-dev` de apt en Raspberry Pi OS Bookworm o Ubuntu 24.04, y `4.9.0` si se siguió el tutorial de la Pi 5.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./HelloWorld
```

- Debe imprimir la versión y `salida: 0`.
- La versión debe coincidir con la línea `-- OpenCV version: …` que imprime CMake al configurar. Si no coincide, CMake está usando otra instalación de OpenCV (se puede forzar con `cmake -S . -B build -DOpenCV_DIR=/usr/local/lib/cmake/opencv4`).
- `RSS máx` es la memoria pico del proceso: unos pocos MB, porque no se reserva ninguna imagen.

## Teoría de Visión Embebida

### Problema que resuelve

Antes de procesar una sola imagen hay que comprobar la cadena de herramientas: que el compilador encuentra las cabeceras de OpenCV, que el enlazador encuentra sus bibliotecas y que el programa resultante arranca en la Raspberry Pi. Este ejemplo hace solo esa comprobación, sin cámara, sin ventanas y sin archivos, así que cualquier fallo es de la instalación y no del código.

### Fundamentos

#### La imagen digital

Una imagen digital es una función de intensidad $f(x, y)$ que se ha **muestreado** en una rejilla de $M \times N$ puntos (los píxeles) y **cuantificado** a un número finito de niveles. Con 8 bits por muestra hay $2^8 = 256$ niveles, de 0 (negro) a 255 (blanco). Una imagen a color guarda 3 muestras por píxel, una por canal.

- El **muestreo** fija la resolución espacial: el detalle más pequeño que se puede representar.
- La **cuantificación** fija la resolución de intensidad: cuántos tonos se distinguen.

Las dos fijan también la memoria que ocupa la imagen, que es lo primero que se agota en un sistema embebido:

```math
\text{bytes} = M \cdot N \cdot C \cdot \text{bytes por muestra}
```

#### `cv::Mat`, la estructura central de OpenCV

Todos los ejemplos de esta carpeta trabajan con `cv::Mat`, que separa dos cosas:

- **La cabecera**, de unas decenas de bytes: filas, columnas, tipo (`CV_8UC3` = 8 bits sin signo y 3 canales), el *stride* `step` (bytes entre el comienzo de una fila y el de la siguiente) y un puntero `data` a los píxeles.
- **El búfer de datos**, que comparten todas las cabeceras que lo apuntan. Un contador de referencias lo libera cuando ya no lo usa ninguna.

Los píxeles se guardan fila a fila (*row-major*) y con los canales intercalados (B, G, R, B, G, R, …), así que el canal $c$ del píxel $(x, y)$ está en:

```math
\text{data} + y \cdot \text{step} + x \cdot C + c
```

Por eso `Mat b = a;` es O(1): copia la cabecera y suma una referencia, pero las dos variables ven los mismos píxeles. Una copia real necesita `clone()` o `copyTo()`. [RegionOfInterest](../RegionOfInterest/) usa esta separación para recortar una imagen sin copiarla.

#### Cabeceras frente a biblioteca

`CV_VERSION` es una macro de `opencv2/core/version.hpp`: el preprocesador la sustituye por una cadena al **compilar**. Lo que imprime el programa es, por tanto, la versión de las cabeceras, no la de la biblioteca `libopencv_core.so` que el enlazador dinámico carga al **ejecutar**. Coinciden mientras se compile y se ejecute con la misma instalación. Si hace falta distinguirlas, `cv::getVersionString()` devuelve la versión de la biblioteca.

### Coste en la Raspberry Pi

El programa solo enlaza `opencv_core` y no reserva ninguna imagen, así que su único coste es arrancar: el enlazador dinámico carga `libopencv_core` y sus dependencias, y el programa termina. Sirve de referencia para los demás ejemplos: el `RSS máx` que da la [Verificación](#verificación) es la memoria mínima de un programa con OpenCV en ese sistema, y lo que los demás midan por encima es lo que cuestan sus imágenes y sus ventanas.

## Descripción del Código

El programa ([`HelloWorld.cpp`](HelloWorld.cpp)) solo incluye `opencv2/core.hpp` e imprime la macro `CV_VERSION`:

```cpp
cout << "OpenCV version: " << CV_VERSION << endl;
```

`CV_VERSION` se fija en tiempo de compilación a partir de las cabeceras de OpenCV que encontró CMake, así que el programa muestra la versión de las cabeceras, no la de la biblioteca que se carga al ejecutarlo.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `CV_VERSION` (macro) | core | Cadena con la versión de OpenCV de las cabeceras |

## Limitaciones conocidas

- Si las cabeceras y la biblioteca compartida son de versiones distintas (por ejemplo, tras actualizar OpenCV sin recompilar), el programa mostrará la versión de las cabeceras. `ldd build/HelloWorld | grep opencv` muestra la biblioteca que se enlaza realmente.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Using OpenCV with gcc and CMake](https://docs.opencv.org/4.x/db/df5/tutorial_linux_gcc_cmake.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
