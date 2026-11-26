# Detección de bordes con filtro Laplaciano en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Detección de bordes con filtro Laplaciano en OpenCV](#detección-de-bordes-con-filtro-laplaciano-en-opencv)
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
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Detecta bordes con el operador Laplaciano, que calcula la segunda derivada de la intensidad y responde allí donde el brillo cambia bruscamente. Como la segunda derivada amplifica el ruido, antes se suaviza la imagen con un filtro Gaussiano (ver [GaussianFilter](../GaussianFilter/)).

## Características

- Suavizado previo con `GaussianBlur` de 3×3.
- Conversión a escala de grises, porque el Laplaciano trabaja sobre un solo canal.
- Laplaciano con salida en 16 bits con signo (`CV_16S`) para no perder los valores negativos.
- Conversión del resultado a 8 bits con valor absoluto (`convertScaleAbs`).

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Pantalla o sesión gráfica para las ventanas de `imshow`.

### Software

- OpenCV 4.x con los módulos `core`, `imgproc`, `imgcodecs` y `highgui` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
- CMake 3.12 o superior y un compilador de C++11.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/LaplacianFilter
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
./LaplacianFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./LaplacianFilter`.

### Resultado esperado

Se abre una sola ventana, **Filtro Laplaciano - Detección de Bordes**, con fondo casi negro y los contornos de la imagen en tonos claros. No se muestra la imagen original. Al pulsar cualquier tecla con la ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./LaplacianFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que la ventana queda abierta, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`LaplacianFilter.cpp`](LaplacianFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `empty()`.
2. **Suavizado:** `GaussianBlur` de 3×3 con sigma automática, aplicado sobre la propia imagen original (entrada y salida son la misma `Mat`).
3. **Escala de grises:** `cvtColor(..., COLOR_BGR2GRAY)`.
4. **Laplaciano:**
   ```cpp
   Laplacian(grayImage, laplacian16S, CV_16S, 3, 1, 0, BORDER_DEFAULT);
   ```
   La salida es `CV_16S` porque la segunda derivada de una imagen de 8 bits toma valores negativos y mayores que 255; con `CV_8U` se recortarían y se perderían la mitad de los bordes.
5. **Vuelta a 8 bits:** `convertScaleAbs` toma el valor absoluto y satura a 0–255, para que los dos lados de cada borde se vean claros.
6. **Visualización:** `imshow` del resultado y `waitKey(0)`. La línea `imwrite("bordes_laplaciano.jpg", ...)` está comentada; al descomentarla, el resultado se guarda en `build/`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `GaussianBlur` | imgproc | Reduce el ruido antes de derivar |
| `cvtColor` | imgproc | Convierte a escala de grises |
| `Laplacian` | imgproc | Calcula la segunda derivada en `CV_16S` |
| `convertScaleAbs` | core | Valor absoluto y conversión a 8 bits |
| `imshow` | highgui | Muestra los bordes |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de la ventana |
| `imwrite` (comentada) | imgcodecs | Guardaría el resultado en disco |

## Limitaciones conocidas

- La imagen original se sobrescribe con la suavizada, así que no se puede mostrar junto al resultado sin cambiar el código.
- El Laplaciano no distingue la dirección del borde ni aplica umbral: todo cambio de intensidad aparece con más o menos brillo. Para bordes finos y binarios, ver [CannyFilter](../CannyFilter/).

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Laplace Operator](https://docs.opencv.org/4.x/d5/db5/tutorial_laplace_operator.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
