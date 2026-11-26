# Detección de bordes con Canny interactivo en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Detección de bordes con Canny interactivo en OpenCV](#detección-de-bordes-con-canny-interactivo-en-opencv)
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

Detecta bordes con el algoritmo de Canny y permite ajustar en vivo sus dos umbrales con *trackbars*. Muestra a la vez todas las etapas del proceso (original, grises, suavizado y bordes), así que sirve para ver cómo influye cada umbral en el resultado.

## Características

- Conversión a escala de grises y suavizado Gaussiano previo.
- Detección de bordes con `Canny` y umbral con histéresis.
- Dos *trackbars* para los umbrales bajo y alto (0–255).
- Cuatro ventanas con cada etapa del proceso, más una de controles.

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Pantalla o sesión gráfica para las ventanas de `imshow`, y ratón para mover los *trackbars*.

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
cd OpenCV/CannyFilter
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
./CannyFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./CannyFilter`.

### Resultado esperado

Se abren cinco ventanas:

1. **Imagen Original**: la imagen cargada.
2. **Imagen en Escala de Grises**.
3. **Filtro Gaussiano**: la imagen en grises suavizada.
4. **Filtro Canny**: bordes blancos de un píxel de grosor sobre fondo negro.
5. **Config**: los *trackbars* **Umbral Bajo Canny** (inicial 50) y **Umbral Alto Canny** (inicial 150).

Al bajar los umbrales aparecen más bordes (y más ruido); al subirlos, solo quedan los contornos más marcados. El programa termina al pulsar **ESC** con una ventana seleccionada.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./CannyFilter
```

- Con el umbral bajo por encima del alto, OpenCV los intercambia internamente y el resultado no cambia al invertirlos. Tras salir con ESC debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` seguido de una lista de comprobaciones, y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`CannyFilter.cpp`](CannyFilter.cpp)) prepara la imagen una vez y luego entra en un bucle:

1. **Carga:** `imread("avatares.png")` (a color por defecto) y comprobación con `empty()`.
2. **Preprocesamiento (una sola vez):** `cvtColor(..., COLOR_BGR2GRAY)` y `GaussianBlur` con *kernel* 3×3 y sigma 3 en X e Y.
3. **Interfaz:** `namedWindow` crea **Config** y **Filtro Canny**, y `createTrackbar` asocia los dos *trackbars* a las variables `cannyLow` y `cannyHigh`.
4. **Bucle principal:** en cada vuelta se aplica Canny con los valores actuales, se muestran las cuatro imágenes y se espera 30 ms a una tecla; con ESC (código 27) se sale:
   ```cpp
   Canny(filtroGausiano, filtroCanny, cannyLow, cannyHigh, 3);
   int key = waitKey(30);
   if (key == 27) break;
   ```
   El último argumento (`3`) es la apertura del operador de Sobel con el que Canny calcula el gradiente. Los píxeles con gradiente mayor que `cannyHigh` son borde seguro; los que están entre los dos umbrales solo se conservan si están conectados a un borde seguro (histéresis).

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `cvtColor` | imgproc | Convierte a escala de grises |
| `GaussianBlur` | imgproc | Suaviza antes de calcular el gradiente |
| `namedWindow` | highgui | Crea la ventana de controles y la del resultado |
| `createTrackbar` | highgui | Crea los controles de los dos umbrales |
| `Canny` | imgproc | Detecta los bordes con histéresis |
| `imshow` | highgui | Muestra las cuatro etapas |
| `waitKey` | highgui | Espera 30 ms a una tecla y procesa los eventos |

## Limitaciones conocidas

- Canny se recalcula cada 30 ms aunque los umbrales no hayan cambiado, lo que mantiene la CPU ocupada mientras el programa está abierto.
- Con sigma 3 sobre un *kernel* de solo 3×3, los pesos son casi iguales y el suavizado se parece más a un filtro de media que a uno Gaussiano.
- Cerrar las ventanas con el ratón no termina el programa: `imshow` las vuelve a abrir en la siguiente vuelta. Hay que salir con ESC.
- Con OpenCV 4.9 se muestra el aviso `createTrackbar ... Using 'value' pointer is unsafe and deprecated`, porque se pasa la variable por puntero. No afecta al funcionamiento.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Canny Edge Detector](https://docs.opencv.org/4.x/da/d5c/tutorial_canny_detector.html)
- [OpenCV Canny Edge Detection](https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
