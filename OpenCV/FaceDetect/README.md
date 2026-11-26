# Detección de rostros en tiempo real con OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Detección de rostros en tiempo real con OpenCV](#detección-de-rostros-en-tiempo-real-con-opencv)
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

Captura vídeo de la cámara y detecta rostros de frente en cada fotograma con un clasificador Haar en cascada preentrenado de OpenCV. Los rostros detectados se marcan con un rectángulo. Es el único ejemplo de la colección que trabaja con vídeo en tiempo real y el punto de partida para sistemas de vigilancia o de reconocimiento facial.

## Características

- Captura de vídeo con `VideoCapture` a 640×480.
- Detección con el clasificador `haarcascade_frontalface_alt.xml`, incluido en esta carpeta.
- Preprocesamiento con escala de grises y ecualización del histograma.
- Rectángulos sobre cada rostro detectado, actualizados en cada fotograma.

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Una cámara: USB (UVC) o el módulo de cámara CSI de la Raspberry Pi (ver [Ejecutar](#ejecutar)).
- Pantalla o sesión gráfica para la ventana de `imshow`.

### Software

- OpenCV 4.x con los módulos `core`, `imgproc`, `highgui`, `objdetect` y `videoio` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
- CMake 3.12 o superior y un compilador de C++11.
- Con cámara CSI en Raspberry Pi OS Bookworm: `libcamerify`, del paquete `libcamera-tools`.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
sudo apt install libcamera-tools   # solo con cámara CSI en Raspberry Pi OS
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/FaceDetect
./build.sh
```

[`build.sh`](build.sh) configura [`CMakeLists.txt`](CMakeLists.txt) en `build/`, compila en modo Release (`-O3`) y lanza el programa. Para compilar sin ejecutar:

```bash
cmake -S . -B build
cmake --build build -j"$(nproc)"
```

### Ejecutar

Con una cámara USB:

```bash
cd build
./FaceDetect
```

Con la cámara CSI en Raspberry Pi OS Bookworm:

```bash
cd build
libcamerify ./FaceDetect
```

En Bookworm, la cámara CSI se gestiona con libcamera y no aparece como un dispositivo V4L2 normal, así que `VideoCapture(0)` no la puede abrir directamente. `libcamerify` intercepta las llamadas V4L2 del programa y las traduce a libcamera sin cambiar el código. La otra alternativa, una *pipeline* de GStreamer con `libcamerasrc`, sí exige modificar el `VideoCapture`.

El programa se ejecuta desde `build/` porque abre `haarcascade_frontalface_alt.xml` con una ruta relativa, y CMake lo copia ahí. Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./FaceDetect`.

### Resultado esperado

El programa imprime `Detección de rostros iniciada. Presiona 'q' o 'Esc' para salir.` y abre la ventana **Rostros Detectados** con el vídeo de la cámara. Cada rostro de frente aparece enmarcado con un rectángulo azul. Al pulsar **q** o **ESC** con la ventana seleccionada, imprime `Saliendo del programa...` y termina un segundo después.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./FaceDetect
```

- Tras salir con `q` o ESC debe mostrar `salida: 0`.
- Sin cámara, OpenCV avisa de que no puede abrir la cámara, el programa imprime `Error: No se pudo abrir la cámara.` y sale con `1`.
- Si se ejecuta desde una carpeta sin `haarcascade_frontalface_alt.xml`, imprime `Error: No se pudo cargar el clasificador Haar.` y sale con `1`.

`RSS máx` es la memoria pico del proceso, incluido el búfer de la cámara. `tiempo` es la duración de la sesión, no el tiempo por fotograma.

## Teoría de Visión Embebida

<!-- FASE 4: problema que resuelve, fundamentos y restricciones de memoria, cómputo y energía, con cifras medidas en la Raspberry Pi. -->

> [!NOTE]
> Sección en preparación.

## Descripción del Código

El programa ([`FaceDetect.cpp`](FaceDetect.cpp)) se divide en `main` y la función `detectarYMostrar`:

1. **Clasificador:** `clasificador.load("haarcascade_frontalface_alt.xml")`; si falla, sale con `EXIT_FAILURE`.
2. **Cámara:** `VideoCapture capture(0)` abre la primera cámara, se comprueba con `isOpened()` y se piden 640×480 con `capture.set(CAP_PROP_FRAME_WIDTH/HEIGHT, …)`. La cámara puede ignorar la petición si no admite esa resolución.
3. **Bucle:** `capture >> imagenOriginal` lee un fotograma; si llega vacío, el programa sale con `EXIT_FAILURE`. Después se llama a `detectarYMostrar` y `waitKey(1)` comprueba si se pulsó `q` o ESC.
4. **`detectarYMostrar`:** convierte a grises, ecualiza el histograma para compensar la iluminación y busca rostros:
   ```cpp
   clasificador.detectMultiScale(imagenGris, rostros, 1.1, 4,
                                 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
   ```
   `1.1` es el factor de escala entre niveles de la pirámide (un 10 % de reducción en cada paso), `4` es el número mínimo de detecciones vecinas para aceptar un rostro y `Size(30, 30)` es el rostro más pequeño que se busca. Cada `Rect` encontrado se dibuja con `rectangle` en azul (BGR `255, 0, 0`) y grosor 2.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `CascadeClassifier::load` | objdetect | Carga el clasificador Haar desde el XML |
| `VideoCapture` | videoio | Abre la cámara y lee fotogramas |
| `VideoCapture::set` | videoio | Pide la resolución de 640×480 |
| `cvtColor` | imgproc | Convierte el fotograma a grises |
| `equalizeHist` | imgproc | Ecualiza el histograma para mejorar la detección |
| `CascadeClassifier::detectMultiScale` | objdetect | Busca rostros a varias escalas |
| `rectangle` | imgproc | Marca cada rostro detectado |
| `namedWindow`, `imshow` | highgui | Crea y actualiza la ventana del vídeo |
| `waitKey` | highgui | Espera 1 ms a una tecla y procesa los eventos |

## Limitaciones conocidas

- El clasificador `frontalface_alt` solo detecta rostros de frente; los perfiles, los rostros inclinados o con poca luz se pierden con facilidad.
- La detección se hace sobre el fotograma completo en cada vuelta, sin reducir la resolución ni limitar la búsqueda a una región, así que es la parte más costosa del bucle.
- El programa no muestra los FPS ni el tiempo por fotograma.
- La variable `scale` y la cabecera `opencv2/features2d.hpp` se declaran pero no se usan.
- La cámara CSI en Bookworm necesita `libcamerify`, que aún no se ha probado con este ejemplo en la Pi.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Cascade Classifier](https://docs.opencv.org/4.x/db/d28/tutorial_cascade_classifier.html)
- [Clasificadores Haar de OpenCV en GitHub](https://github.com/opencv/opencv/tree/master/data/haarcascades)
- [Raspberry Pi: documentación de la cámara](https://www.raspberrypi.com/documentation/computers/camera_software.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
