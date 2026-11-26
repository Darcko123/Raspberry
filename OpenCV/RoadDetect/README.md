# Detección de líneas de carril en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Detección de líneas de carril en OpenCV](#detección-de-líneas-de-carril-en-opencv)
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
      - [1. Preprocesamiento](#1-preprocesamiento)
      - [2. Región de interés](#2-región-de-interés)
      - [3. Detección de bordes](#3-detección-de-bordes)
      - [4. Transformada de Hough probabilística](#4-transformada-de-hough-probabilística)
      - [5. Filtrado y clasificación](#5-filtrado-y-clasificación)
      - [6. Promedio y extensión](#6-promedio-y-extensión)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Captura el vídeo de la cámara y busca las marcas que delimitan el carril: convierte cada fotograma a escala de grises, lo suaviza, extrae bordes con Canny y aplica la transformada de Hough probabilística solo en la parte baja de la imagen. Las líneas detectadas se filtran por ángulo, se clasifican en izquierda y derecha y se dibujan extendidas hasta el borde inferior.

Es el ejemplo más cercano a un sistema de asistencia a la conducción: reúne preprocesamiento, [ROI](../RegionOfInterest/), [Canny](../CannyFilter/) y detección de rectas en un único bucle en tiempo real.

## Características

- Captura de vídeo con `VideoCapture` a 640×480 y 60 FPS solicitados, con brillo y contraste fijados en 128.
- Si la cámara no abre o no entrega fotogramas, avisa y procesa una foto de [assets/](../assets/): funciona también sin cámara.
- ROI trapezoidal que cubre el 22 % del fotograma (de la fila 60 % a la última).
- Bordes con Canny (umbrales 50 y 150) y detección de segmentos con `HoughLinesP`.
- Filtro de ángulo (20°–70°), clasificación por pendiente y promedio de las líneas izquierda y derecha.
- Tres ventanas: original, ROI enmascarada y resultado con las líneas dibujadas.

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Una cámara: USB (UVC) o el módulo CSI de la Raspberry Pi (ver [Ejecutar](#ejecutar)).
- Pantalla o sesión gráfica para las ventanas de `imshow`.

### Software

- OpenCV 4.x con los módulos `core`, `imgproc`, `imgcodecs`, `highgui` y `videoio` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
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
cd OpenCV/RoadDetect
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
./RoadDetect
```

Con la cámara CSI en Raspberry Pi OS Bookworm:

```bash
cd build
libcamerify ./RoadDetect
```

En Bookworm, la cámara CSI se gestiona con libcamera y no aparece como un dispositivo V4L2 normal, así que `VideoCapture(0)` no la puede abrir directamente. `libcamerify` traduce las llamadas V4L2 a libcamera sin cambiar el código, igual que en [FaceDetect](../FaceDetect/).

El programa intenta abrir la cámara; si no lo consigue o no entrega ningún fotograma, avisa y carga la primera foto que encuentra en [assets/](../assets/): [carretera2_1.jpg](../assets/carretera2_1.jpg), [carretera3_1.jpg](../assets/carretera3_1.jpg) o [carretera4_1.jpg](../assets/carretera4_1.jpg). CMake copia las tres a `build/`, que es donde las busca. En modo imagen se procesa un único fotograma y se termina al pulsar cualquier tecla. Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./RoadDetect`.

### Resultado esperado

Se abren tres ventanas:

1. **Imagen original**: el fotograma tal como llega de la cámara.
2. **Region de interes (ROI)**: bordes de Canny dentro del trapecio, en blanco sobre fondo negro; fuera de la ROI todo queda a cero.
3. **Lineas detectadas**: copia del fotograma con el trapecio en magenta, la línea izquierda en azul y la derecha en rojo. Si no hay segmentos, aparece `No se detectaron lineas`; si falta un lado, aparece `No hay linea izquierda` o `No hay linea derecha`.

Con cámara, al pulsar **q** o **ESC** con alguna ventana seleccionada imprime `Saliendo del programa...` y termina un segundo después. Sin cámara, imprime `Aviso: No se pudo abrir la cámara; se prueba con una imagen de assets/` o `Aviso: Usando 'carretera2_1.jpg' en lugar de la cámara.`, muestra las mismas tres ventanas con la foto y termina al pulsar cualquier tecla.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./RoadDetect
```

- Con cámara, tras salir con `q` o ESC debe mostrar `salida: 0`.
- Sin cámara, debe imprimir los avisos de la cámara y de la imagen elegida, abrir las ventanas con la foto y mostrar `salida: 0` tras pulsar una tecla. En el equipo de desarrollo, sin cámara física, OpenCV abre un dispositivo por GStreamer, no entrega frames y se activa el fallback; los avisos `GStreamer warning` no vienen del código.
- Si se lanza desde fuera de `build/`, no encuentra las fotos y debe imprimir `Error: No se pudo abrir la cámara ni cargar una imagen de assets.` con `salida: 1`.
- Sin pantalla, se puede comprobar el procesamiento con `timeout 5 env QT_QPA_PLATFORM=offscreen ./RoadDetect`: debe llegar al aviso de la imagen y quedarse esperando (salida `124` del `timeout`).

`RSS máx` incluye el búfer de la cámara y las ventanas. `tiempo` es la duración de la sesión, no el tiempo por fotograma.

## Teoría de Visión Embebida

### Problema que resuelve

Encontrar las dos marcas que delimitan el carril en cada fotograma, con una CPU modesta y sin GPU. Buscar rectas en la imagen completa es caro y propenso a confusiones: el cielo, la montaña, los árboles y el arcén también producen bordes. El truco central es reducir la búsqueda a la zona donde pueden estar las marcas y convertir cada segmento detectado en una única línea representativa por lado.

### Fundamentos

El programa encadena seis etapas; cada una reduce los datos que recibe la siguiente.

#### 1. Preprocesamiento

La conversión a grises (`COLOR_BGR2GRAY`) pasa de tres canales a uno y elimina información de color que no aporta a la detección de bordes. Después se aplica un Gaussiano 3×3 con σ = 3 en X e Y, igual que en [CannyFilter](../CannyFilter/): con un kernel tan pequeño y una sigma tan alta, los pesos casi no se diferencian y el resultado se parece a una media. Suavizar antes de derivar evita que el ruido de la carretera se convierta en bordes falsos.

#### 2. Región de interés

La ROI es un trapecio definido con coordenadas relativas: la base superior va del 45 % al 55 % del ancho en la fila 60 %, y la base inferior ocupa todo el ancho en la última fila. Su área es

```math
A = \frac{0{,}4 \cdot (1{,}0 + 0{,}1)}{2} = 0{,}22
```

del fotograma: Hough procesa solo el 22 % de los píxeles, unas 4,5 veces menos que la imagen completa. `fillConvexPoly` rellena la máscara con 255 dentro del polígono y `copyTo` pone a cero el resto. Es la misma idea de [RegionOfInterest](../RegionOfInterest/) aplicada a vídeo: `image(r)` sería una vista sin copia, pero aquí la forma trapezoidal exige una máscara.

#### 3. Detección de bordes

`Canny` con umbrales 50 y 150 (relación 3:1) produce bordes de un píxel con histéresis: los píxeles con gradiente por encima de 150 son borde seguro y los que quedan entre 50 y 150 solo se conservan si están conectados a uno seguro. La etapa de Canny se explica con detalle en [CannyFilter](../CannyFilter/).

#### 4. Transformada de Hough probabilística

La transformada clásica vota en un espacio de parámetros $(\rho, \theta)$; la versión probabilística `HoughLinesP` devuelve directamente segmentos $(x_1, y_1, x_2, y_2)$ y es mucho más barata. Los parámetros del ejemplo son:

| Parámetro | Valor | Significado |
|---|---|---|
| `rho` | 1 px | Resolución del espacio de líneas |
| `theta` | $\pi/180$ | Resolución angular (1°) |
| `threshold` | 50 | Votos mínimos para aceptar un segmento |
| `minLineLength` | 50 px | Longitud mínima |
| `maxLineGap` | 10 px | Hueco máximo entre segmentos colineales |

Un valor bajo de `maxLineGap` une tramos separados por pequeñas oclusiones; subirlo puede unir marcas de carriles distintos.

#### 5. Filtrado y clasificación

La pendiente de cada segmento se calcula con un pequeño término en el denominador para evitar la división por cero:

```math
m = \frac{y_2 - y_1}{x_2 - x_1 + 10^{-5}}, \qquad \alpha = \arctan(m) \cdot \frac{180}{\pi}
```

Se conservan los segmentos con $20^\circ < |\alpha| < 70^\circ$: se descartan las marcas horizontales y las casi verticales. Luego se clasifican por pendiente **y** posición: izquierda si $m < 0$ y ambos extremos están antes de la columna `cols/2`, derecha si $m > 0$ y ambos están después. En coordenadas de imagen el eje Y crece hacia abajo, así que una línea que se abre hacia la izquierda tiene pendiente negativa.

#### 6. Promedio y extensión

Se promedian los cuatro extremos de los segmentos de cada grupo para obtener una línea representativa y se extiende desde el 60 % de la altura hasta el borde inferior usando la pendiente:

```math
x_{bottom} = x_1 + \frac{y_{bottom} - y_1}{m}, \qquad
x_{top} = x_1 + \frac{y_{top} - y_1}{m}
```

Así una marca rota en muchos segmentos se dibuja como una sola línea continua. El resultado es una referencia visual, no una medida de distancia: no hay calibración de cámara ni corrección de perspectiva.

### Coste en la Raspberry Pi

Las cifras son tamaños **calculados** para el fotograma solicitado de 640×480, no mediciones:

| Matriz | Tamaño |
|---|---|
| `frame` (BGR) | 921.600 B (900 KiB) |
| `gray`, `blurred`, `edges`, `laneMask` (8UC1) | 4 × 307.200 B (300 KiB cada una) |
| `roiImage` (BGR) | 921.600 B (900 KiB) |
| `houghResult` (clon de `frame`) | 921.600 B (900 KiB) |

En total, unas 3,8 MiB de matrices por fotograma, más los búferes internos de `VideoCapture`, `Canny` y las tres ventanas. Los búferes de la GUI pesan más que las imágenes, como se comprobó en los ejemplos con `imshow`. En modo imagen, `carretera2_1.jpg` es de 1000×533 y las matrices suman unas 6,6 MiB.

- **Cómputo:** el Gaussiano 3×3 y Canny recorren los 307.200 píxeles del fotograma en gris; Hough solo los de la ROI (22 %). El filtrado y el promedio trabajan sobre decenas de segmentos, así que su coste es insignificante al lado de Canny y Hough. Reducir la resolución de captura es la palanca más eficaz: el trabajo crece con el número de píxeles.
- **Energía:** `waitKey(1)` apenas espera, así que el bucle procesa fotogramas tan rápido como llegan y la CPU no descansa mientras el programa está abierto. No se mide ningún FPS.

**Sin medir:** no hay cámara conectada en el equipo de desarrollo ni en la Raspberry Pi de referencia, así que no se pudo obtener la memoria pico real. El ejemplo es de los pocos que no se pueden verificar sin hardware.

## Descripción del Código

El programa ([`RoadDetect.cpp`](RoadDetect.cpp)) tiene seis funciones auxiliares `static`, con los prototipos antes de `main` y las definiciones después, y un bucle de captura:

1. **Entrada:** `VideoCapture capture(0)` y comprobación con `isOpened()`; se piden 640×480, 60 FPS, brillo 128 y contraste 128. Se lee un primer frame porque algunas cámaras abren pero no entregan imagen. Si no hay cámara o el frame está vacío, `loadFallbackImage` prueba las tres fotos de `build/` con `imread` y avisa de cuál usa; si no hay ninguna, termina con `EXIT_FAILURE`.
2. **Máscaras:** `laneMask` y `roiImage` se crean con el tamaño del primer frame. En el código original se inicializaban con `Mat::zeros(imagenOriginal.size(), …)` antes de capturar, cuando `imagenOriginal` estaba vacía: la máscara seguía vacía y `copyTo` copiaba el fotograma entero, sin aplicar la ROI.
3. **Preprocesamiento:** `cvtColor(frame, gray, COLOR_BGR2GRAY)`, `GaussianBlur` de 3×3 y `Canny` con umbrales 50 y 150.
4. **ROI:** los cuatro vértices del trapecio se calculan a partir de `frame.cols` y `frame.rows`; `fillConvexPoly` rellena `laneMask` y `edges.copyTo(roiImage, laneMask)` deja a cero el exterior.
5. **`processLines`:** llama a `HoughLinesP`, clona el fotograma en `houghResult`, dibuja el trapecio y las líneas. Si no hay segmentos, escribe el aviso y sale; si falta la izquierda o la derecha, escribe el aviso correspondiente en esa posición.
6. **Interfaz:** se muestran las tres ventanas. Con cámara, `waitKey(1)` comprueba `q` o ESC y vuelve a capturar; sin cámara (`useImage`), `waitKey(0)` espera una tecla y termina después de esa única vuelta. En ambos casos imprime `Saliendo del programa...`, espera un segundo y devuelve `EXIT_SUCCESS`.

```cpp
HoughLinesP(roiImage, lines, 1, CV_PI / 180, 50, 50, 10);
```

Los dos últimos valores son la longitud mínima del segmento (50 px) y el hueco máximo entre segmentos (10 px).

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `VideoCapture` | videoio | Abre la cámara y lee fotogramas |
| `VideoCapture::set` | videoio | Pide resolución, FPS, brillo y contraste |
| `imread` | imgcodecs | Carga una foto de `assets/` si no hay cámara |
| `cvtColor` | imgproc | Convierte el fotograma a escala de grises |
| `GaussianBlur` | imgproc | Suaviza antes de Canny |
| `Canny` | imgproc | Extrae bordes con histéresis |
| `fillConvexPoly` | imgproc | Rellena la máscara trapezoidal de la ROI |
| `Mat::copyTo` | core | Aplica la máscara a los bordes |
| `HoughLinesP` | imgproc | Detecta segmentos de recta en la ROI |
| `polylines`, `line`, `putText` | imgproc | Dibujan la ROI, las líneas y los avisos |
| `imshow` | highgui | Muestra las tres ventanas |
| `waitKey` | highgui | Espera 1 ms a una tecla y procesa los eventos |

## Limitaciones conocidas

- Sin cámara solo procesa una foto fija de [assets/](../assets/) (la primera que cargue); no hay argumentos para elegir otra ni para leer vídeo desde archivo.
- La cámara CSI en Bookworm necesita `libcamerify`; si no está, el programa cae al modo imagen en lugar de usar la cámara.
- El modo imagen necesita una sesión gráfica para `imshow`: por SSH hay que usar `ssh -X` o `DISPLAY=:0`.
- Los parámetros de Canny y Hough están fijos: con mucha luz, sombras o marcas desgastadas la detección se degrada y no hay ajuste en tiempo real.
- Solo promedia un segmento por lado: en una autopista con varios carriles puede elegir una marca equivocada y no hay seguimiento entre fotogramas.
- Cada fotograma clona la imagen para dibujar el resultado, una copia completa de 900 KiB.
- No se muestran los FPS ni el tiempo por fotograma, así que no se puede estimar el consumo energético real.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Hough Line Transform](https://docs.opencv.org/4.x/d6/dc7/tutorial_img_houghlines.html)
- [Canny Edge Detector](https://docs.opencv.org/4.x/da/d5c/tutorial_canny_detector.html)
- [Raspberry Pi: documentación de la cámara](https://www.raspberrypi.com/documentation/computers/camera_software.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
