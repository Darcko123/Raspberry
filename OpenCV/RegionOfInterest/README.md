# Regiones de interés (ROI) en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Regiones de interés (ROI) en OpenCV](#regiones-de-interés-roi-en-opencv)
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

Muestra cuatro formas de quedarse solo con una parte de la imagen: un recorte rectangular elegido con el ratón y tres máscaras con forma de rombo, rectángulo y círculo. Cada resultado se muestra en una ventana y se guarda como JPEG. Limitar el procesamiento a una región de interés es una de las técnicas más útiles para reducir el cómputo en un sistema embebido.

## Características

- Selección interactiva de un rectángulo con `selectROI` y recorte con `image(r)`.
- Máscara poligonal (rombo) con `fillConvexPoly`.
- Máscara rectangular con `rectangle` y máscara circular con `circle`.
- Aplicación de las máscaras con `copyTo` y guardado de los 4 resultados con `imwrite`.

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Pantalla o sesión gráfica para las ventanas de `imshow`, y ratón para seleccionar la región.

### Software

- OpenCV 4.x con los módulos `core`, `imgproc`, `imgcodecs` y `highgui` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)). `selectROI` está en `highgui`, así que no hace falta `opencv_contrib`.
- CMake 3.12 o superior y un compilador de C++11.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/RegionOfInterest
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
./RegionOfInterest
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./RegionOfInterest`.

### Resultado esperado

1. Se abre la ventana **ROI selector** con la imagen completa. Arrastre el ratón para dibujar un rectángulo y confirme con **ESPACIO** o **ENTER**.
2. A continuación se abren a la vez cuatro ventanas:
   - **ROI Manual**: el recorte seleccionado, con su tamaño real.
   - **ROI Poligonal**: la imagen completa en negro salvo un rombo central.
   - **ROI Rectangular**: en negro salvo el rectángulo central (la mitad del ancho y del alto).
   - **ROI Circular**: en negro salvo un círculo central de radio igual al 25 % del lado menor.
3. Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

Los cuatro resultados se guardan en `build/` como `roi_image_target.jpg`, `roi_image_Polygon.jpg`, `roi_image_Rectangular.jpg` y `roi_image_Circular.jpg` (el `.gitignore` del repositorio los excluye).

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./RegionOfInterest
ls roi_image_*.jpg
```

- Tras cerrar con una tecla debe mostrar `salida: 0`, y `ls` debe listar los 4 archivos.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen.` y salir con `255` (el código devuelve `-1`).

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`RegionOfInterest.cpp`](RegionOfInterest.cpp)) carga `avatares.png`, guarda su ancho (`cols`) y alto (`rows`), y aplica cuatro métodos uno detrás de otro:

1. **ROI manual:** `selectROI(image)` devuelve el rectángulo dibujado y `image(r)` lo recorta. El recorte es una **vista** sobre los datos de la imagen original, no una copia.
2. **ROI poligonal:** se definen 4 vértices en los puntos medios de la imagen (el código lo llama trapecio, pero la figura es un rombo), `fillConvexPoly` los rellena con 255 en una máscara de un canal y `image.copyTo(poligonalResult, mask)` copia solo los píxeles de dentro.
3. **ROI rectangular:** `Rect roi_rect(cols·0,25, rows·0,25, cols·0,5, rows·0,5)`, dibujado relleno (grosor `-1`) en una máscara con `rectangle`, y el mismo `copyTo`.
4. **ROI circular:** `circle` con centro en el centro de la imagen y radio `min(cols, rows)·0,25`, relleno en una máscara, y el mismo `copyTo`.

Cada resultado se muestra con `imshow` y se guarda con `imwrite`. Al final, `waitKey(0)` espera una tecla.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `selectROI` | highgui | Deja dibujar un rectángulo con el ratón |
| `Mat::operator()(Rect)` | core | Recorta la imagen sin copiar los datos |
| `Mat::zeros` | core | Crea las máscaras y las imágenes de salida en negro |
| `fillConvexPoly` | imgproc | Rellena el rombo en la máscara |
| `rectangle` | imgproc | Rellena el rectángulo en la máscara |
| `circle` | imgproc | Rellena el círculo en la máscara |
| `Mat::copyTo` (con máscara) | core | Copia solo los píxeles de dentro de la máscara |
| `imshow` | highgui | Muestra los cuatro resultados |
| `imwrite` | imgcodecs | Guarda los cuatro resultados en JPEG |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- Si se cancela la selección (tecla `c`) o se confirma sin dibujar nada, `selectROI` devuelve un rectángulo vacío, `image(r)` produce una `Mat` vacía y `imshow` la rechaza: el programa termina con una excepción de OpenCV.
- Las tres máscaras usan proporciones fijas del tamaño de la imagen; no se pueden ajustar sin recompilar.
- En caso de error devuelve `-1` (el sistema lo ve como `255`) en lugar de `EXIT_FAILURE`, y escribe el mensaje por `cout` en lugar de `cerr`.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Conceptos básicos de ROI](https://learnopencv.com/how-to-select-a-bounding-box-roi-in-opencv-cpp-python/)
- [Documentación oficial de OpenCV - Clase Mat (`copyTo` con máscara)](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
