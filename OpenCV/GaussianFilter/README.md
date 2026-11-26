# Suavizado con filtro Gaussiano en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Suavizado con filtro Gaussiano en OpenCV](#suavizado-con-filtro-gaussiano-en-opencv)
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

Aplica un filtro Gaussiano a la imagen a color y muestra el original junto al resultado. El filtro Gaussiano es un suavizado que reduce el ruido y los detalles finos ponderando cada píxel vecino según una campana de Gauss, y es el paso previo habitual de los detectores de bordes ([LaplacianFilter](../LaplacianFilter/) y [CannyFilter](../CannyFilter/)).

## Características

- Suavizado con `GaussianBlur` y un *kernel* de 3×3.
- Sigma calculada automáticamente a partir del tamaño del *kernel*.
- Funciona directamente sobre los 3 canales de la imagen a color.
- Visualización del original y del resultado en dos ventanas.

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
cd OpenCV/GaussianFilter
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
./GaussianFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./GaussianFilter`.

### Resultado esperado

El programa imprime `Presione cualquier tecla para salir...` y abre dos ventanas:

1. **Imagen Original**: la imagen cargada.
2. **Filtro Gaussiano - Suavizado de Imagen**: la imagen ligeramente desenfocada; se nota sobre todo en los bordes y en las texturas finas.

Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./GaussianFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` seguido de una lista de comprobaciones, y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`GaussianFilter.cpp`](GaussianFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `empty()`.
2. **Filtro:** una sola llamada aplica el suavizado a los 3 canales:
   ```cpp
   GaussianBlur(imageOriginal, gaussianResult,
                Size(kernelSize, kernelSize), // 3×3, debe ser impar
                0, 0,                         // sigma X e Y: 0 = calculada
                BORDER_DEFAULT);
   ```
   Con sigma `0`, OpenCV la calcula como `0,3·((k − 1)·0,5 − 1) + 0,8`, que para `k = 3` vale `0,8`. `BORDER_DEFAULT` refleja los píxeles del borde para poder aplicar el *kernel* en los bordes de la imagen.
3. **Visualización:** `imshow` del original y del resultado, mensaje por consola y `waitKey(0)`. La línea `imwrite("imagen_suavizada_gaussiano.jpg", ...)` está comentada; al descomentarla, el resultado se guarda en `build/`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `GaussianBlur` | imgproc | Suaviza la imagen con un *kernel* Gaussiano de 3×3 |
| `imshow` | highgui | Muestra el original y el resultado |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |
| `imwrite` (comentada) | imgcodecs | Guardaría el resultado en disco |

## Limitaciones conocidas

- El tamaño del *kernel* (3×3) está fijo en el código, así que el desenfoque es sutil. Para verlo más marcado hay que cambiar `kernelSize` (5, 7, …) y recompilar.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Función GaussianBlur](https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#gae8bdcd9154ed5ca3cbc1766d960f45c1)
- [Smoothing Images](https://docs.opencv.org/4.x/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
