# Carga y visualización de una imagen con OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Carga y visualización de una imagen con OpenCV](#carga-y-visualización-de-una-imagen-con-opencv)
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

Ejemplo básico de entrada y salida de imágenes: lee `avatares.png` desde disco, comprueba que la lectura fue correcta y la muestra en una ventana hasta que se pulsa una tecla. Es la base de todos los ejemplos siguientes, que parten de la misma carga.

## Características

- Lectura de la imagen a color con `imread`.
- Comprobación de la carga con `Mat::empty()` y mensaje de error descriptivo.
- Visualización en una ventana con `imshow`.

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Pantalla o sesión gráfica para las ventanas de `imshow`.

### Software

- OpenCV 4.x con los módulos `core`, `imgcodecs` y `highgui` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
- CMake 3.12 o superior y un compilador de C++11.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/LoadImage
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
./LoadImage
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./LoadImage`.

### Resultado esperado

Se abre la ventana **Imagen avatares** con la imagen de 800×450 píxeles. Al pulsar cualquier tecla con la ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./LoadImage
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/` (por ejemplo, `./build/LoadImage` desde la carpeta del ejemplo), debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que la ventana queda abierta, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`LoadImage.cpp`](LoadImage.cpp)) hace tres pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` decodifica el PNG en una matriz `Mat` de tipo `CV_8UC3` (3 canales de 8 bits en orden **BGR**, no RGB). `IMREAD_COLOR` descarta el canal alfa del PNG.
2. **Comprobación:** si el archivo no existe o no se puede decodificar, `imread` no lanza ninguna excepción: devuelve una `Mat` vacía. Por eso se comprueba con `img.empty()` y se sale con `EXIT_FAILURE`.
3. **Visualización:** `imshow("Imagen avatares", img)` crea la ventana y `waitKey(0)` bloquea hasta que se pulsa una tecla. Sin `waitKey` la ventana no llega a dibujarse, porque es esa función la que procesa los eventos de la interfaz.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR, sin alfa) |
| `Mat::empty` | core | Detecta que la carga falló |
| `imshow` | highgui | Muestra la imagen en la ventana *Imagen avatares* |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de la ventana |

## Limitaciones conocidas

- El nombre del archivo está fijo en el código: para usar otra imagen hay que cambiar la cadena y recompilar, o copiarla a `build/` con el nombre `avatares.png`.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Función waitKey](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga5628525ad33f52eab17feebcfba38bd7)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
