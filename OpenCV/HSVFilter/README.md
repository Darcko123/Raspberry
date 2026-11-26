# Segmentación por color en HSV con OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Segmentación por color en HSV con OpenCV](#segmentación-por-color-en-hsv-con-opencv)
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

Convierte la imagen al espacio de color HSV (tono, saturación y valor) y extrae las zonas rojas, verdes y azules con rangos de tono fijos. A diferencia de [RGBFilter](../RGBFilter/), que compara canales píxel a píxel, aquí se usan máscaras binarias y operaciones sobre la matriz completa, y los colores poco saturados (grises) quedan fuera.

## Características

- Conversión de BGR a HSV con `cvtColor`.
- Una máscara binaria por color con `inRange`.
- El rojo usa dos rangos de tono, porque en HSV el tono es circular y el rojo está a ambos lados del 0.
- Aplicación de las máscaras con `bitwise_and` para conservar los colores originales.

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
cd OpenCV/HSVFilter
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
./HSVFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./HSVFilter`.

### Resultado esperado

Se abren cuatro ventanas:

1. **Imagen original**: la imagen cargada.
2. **Rojo (HSV)**: solo las zonas rojas, con su color original; el resto, en negro.
3. **Verde (HSV)**: solo las zonas verdes.
4. **Azul (HSV)**: solo las zonas azules.

Las zonas grises, oscuras o poco saturadas no aparecen en ninguna salida. Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./HSVFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo abrir o encontrar la imagen.` y salir con `255` (el código devuelve `-1`).

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`HSVFilter.cpp`](HSVFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `image.empty()`.
2. **Conversión:** `cvtColor(image, imageHSV, COLOR_BGR2HSV)`. En imágenes de 8 bits, OpenCV guarda el tono (H) en el rango 0–179 (grados divididos entre 2) y la saturación (S) y el valor (V) en 0–255.
3. **Máscaras:** `inRange` pone a 255 los píxeles dentro del rango y a 0 el resto. Todos los rangos exigen S ≥ 100 y V ≥ 100 para descartar grises y zonas oscuras:

   | Color | Rango de H | Máscara |
   |---|---|---|
   | Rojo | 0–10 y 160–179 | `maskRojo1 \| maskRojo2` |
   | Verde | 35–85 | `maskVerde` |
   | Azul | 100–130 | `maskAzul` |

4. **Aplicación:** `bitwise_and(image, image, rojo, maskRojo)` copia la imagen original solo donde la máscara vale 255, así que cada salida conserva los colores reales.
5. **Visualización:** `imshow` de las cuatro imágenes y `waitKey(0)`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `cvtColor` | imgproc | Convierte de BGR a HSV |
| `inRange` | core | Genera la máscara binaria de cada color |
| operador `\|` sobre `Mat` | core | Une los dos rangos del rojo |
| `bitwise_and` | core | Aplica la máscara a la imagen original |
| `imshow` | highgui | Muestra la original y las tres salidas |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- Los rangos están fijos en el código; para ajustarlos a otra imagen o a otra iluminación hay que editar los `Scalar` y recompilar.
- Los tonos entre rangos (amarillos, cian, magentas) no aparecen en ninguna salida.
- En caso de error devuelve `-1` (el sistema lo ve como `255`) en lugar de `EXIT_FAILURE`.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Changing Colorspaces](https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html)
- [Thresholding Operations using inRange](https://docs.opencv.org/4.x/da/d97/tutorial_threshold_inRange.html)
- [Color conversions (BGR ↔ HSV)](https://docs.opencv.org/4.x/de/d25/imgproc_color_conversions.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
