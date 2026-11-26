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
    - [Problema que resuelve](#problema-que-resuelve)
    - [Fundamentos](#fundamentos)
      - [Archivo frente a imagen en memoria](#archivo-frente-a-imagen-en-memoria)
      - [Qué hace `imread`](#qué-hace-imread)
      - [De la matriz a la pantalla](#de-la-matriz-a-la-pantalla)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
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

### Problema que resuelve

Una imagen se guarda en disco comprimida y en un formato concreto (PNG, JPEG, …), pero los algoritmos necesitan una matriz de píxeles sin comprimir en memoria. Este ejemplo muestra el paso de uno a otro, la **decodificación**, y el camino de la matriz hasta la pantalla.

### Fundamentos

#### Archivo frente a imagen en memoria

`avatares.png` ocupa 505 KB en disco y 1,08 MB una vez decodificada. La diferencia es la compresión, que cada formato hace de forma distinta:

- **PNG** comprime **sin pérdidas**. Cada fila pasa por un filtro predictivo que sustituye cada byte por su diferencia con un vecino (el de la izquierda, el de arriba, su media, …), lo que deja muchos valores cercanos a cero. El resultado se comprime con DEFLATE (LZ77 seguido de un código de Huffman). Al decodificar se recupera exactamente el original.
- **JPEG** comprime **con pérdidas**. Pasa la imagen a YCbCr (brillo y dos componentes de color), suele reducir la resolución del color porque el ojo es menos sensible a él que al brillo, divide la imagen en bloques de 8×8, aplica a cada uno la transformada discreta del coseno (DCT) y cuantifica con más fuerza las frecuencias altas. Ocupa menos, pero cada vez que se guarda pierde detalle y aparecen artefactos en los bloques. No conviene usarlo para resultados intermedios ni para máscaras.

#### Qué hace `imread`

1. Reconoce el formato por la firma de los primeros bytes del archivo, no por la extensión. Por eso el antiguo `avatares.jpg`, que en realidad era un PNG, se cargaba sin problemas.
2. Decodifica el contenido a una matriz de píxeles.
3. La convierte al formato pedido. Con `IMREAD_COLOR` el resultado es siempre `CV_8UC3` (3 canales de 8 bits) en orden **BGR**. `avatares.png` tiene 4 canales (RGBA), así que el canal alfa (la transparencia) se descarta. `IMREAD_UNCHANGED` lo conservaría (`CV_8UC4`) e `IMREAD_GRAYSCALE` daría un solo canal.

El orden BGR es una herencia de los primeros años de OpenCV y coincide con el de los mapas de bits BMP. Las bibliotecas que esperan RGB, como la mayoría de las redes neuronales, necesitan antes `cvtColor(img, img, COLOR_BGR2RGB)`.

#### De la matriz a la pantalla

`imshow` no dibuja en el momento: entrega la imagen al *backend* gráfico (GTK en Raspberry Pi OS) y deja pendiente el repintado. Quien atiende los eventos de la interfaz es `waitKey`: repinta las ventanas, procesa el ratón y el teclado, y devuelve la tecla pulsada. Sin una llamada a `waitKey`, la ventana no llega a mostrarse; con `waitKey(0)`, el programa queda bloqueado hasta que se pulse una tecla.

### Coste en la Raspberry Pi

| Representación de `avatares.png` | Tamaño |
|---|---|
| Archivo PNG en disco | 505 KB |
| `IMREAD_COLOR`: 800 × 450 × 3 bytes | 1.080.000 B ≈ 1,08 MB |
| `IMREAD_UNCHANGED`: 800 × 450 × 4 bytes | 1,44 MB |
| `IMREAD_GRAYSCALE`: 800 × 450 × 1 byte | 360 KB |

- La memoria crece con el área de la imagen. Un fotograma completo de la Camera Module 3 (4608 × 2592, 12 MP) ocupa 4608 × 2592 × 3 ≈ 35,8 MB en BGR: en una Pi Zero 2W, con 512 MB compartidos con la GPU y el sistema, unas pocas copias bastan para quedarse sin memoria.
- Si solo hace falta una versión pequeña, `IMREAD_REDUCED_COLOR_2`, `_4` u `_8` reducen la imagen al cargarla. Con JPEG el decodificador escala directamente en el dominio de la DCT y se ahorra la mayor parte del trabajo; con PNG decodifica la imagen entera y la reduce después, así que solo ahorra la memoria del resultado.
- DEFLATE se descomprime de forma secuencial, así que la decodificación de un PNG usa un solo núcleo de la CPU.

**Medido** en una Raspberry Pi 5 con OpenCV 4.9 (mediana de 3 ejecuciones, con las ventanas abiertas): `RSS máx` = 31.408 kB, unos 21,5 MB más que [HelloWorld](../HelloWorld/). La imagen solo supone 1,08 MB de esa diferencia. Un programa de prueba que solo llama a `imread` ocupa 18.544 kB, y el mismo programa con `imshow` sube a 31.328 kB: cargar las bibliotecas y decodificar la imagen cuesta unos 8,7 MB, y abrir la primera ventana (iniciar GTK) unos 12,8 MB. En un sistema sin pantalla, no abrir ventanas es el mayor ahorro de memoria posible.

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
