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
    - [Problema que resuelve](#problema-que-resuelve)
    - [Fundamentos](#fundamentos)
      - [1. Características de Haar](#1-características-de-haar)
      - [2. Imagen integral](#2-imagen-integral)
      - [3. AdaBoost](#3-adaboost)
      - [4. Cascada atencional](#4-cascada-atencional)
      - [Búsqueda multiescala: `detectMultiScale`](#búsqueda-multiescala-detectmultiscale)
      - [Ecualización del histograma](#ecualización-del-histograma)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
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

El programa imprime `Detección de rostros iniciada. Presiona 'q' o 'Esc' para salir.` y abre la ventana **Rostros detectados** con el vídeo de la cámara. Cada rostro de frente aparece enmarcado con un rectángulo azul. Al pulsar **q** o **ESC** con la ventana seleccionada, imprime `Saliendo del programa...` y termina un segundo después.

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

### Problema que resuelve

Encontrar los rostros de cada fotograma de la cámara en tiempo real, en una CPU modesta y sin GPU ni redes neuronales. El detector de Viola y Jones (2001), que es el que usa este ejemplo, fue el primero en conseguirlo y sigue siendo útil en hardware limitado.

### Fundamentos

La idea es recorrer la imagen con una ventana, a varias escalas, y decidir en cada posición si contiene un rostro o no. Con cientos de miles de ventanas por fotograma, la decisión tiene que ser casi gratuita para la gran mayoría de ellas. Viola y Jones lo consiguieron combinando cuatro ideas.

#### 1. Características de Haar

En lugar de mirar píxeles sueltos, el detector mide contrastes entre rectángulos adyacentes: la suma de los píxeles de una zona menos la de la zona vecina. Por ejemplo, la franja de los ojos es más oscura que la de las mejillas, y el puente de la nariz más claro que los ojos que tiene a los lados. Cada característica se calcula como:

```math
h = \sum_i w_i \cdot \operatorname{Suma}(R_i)
```

con 2, 3 o 4 rectángulos $R_i$ y pesos $w_i$ de signo opuesto. Combinando posiciones, tamaños y tipos, dentro de una ventana pequeña caben decenas de miles de características distintas.

#### 2. Imagen integral

Cada elemento de la **imagen integral** guarda la suma de todos los píxeles que quedan por encima y a la izquierda:

```math
II(x, y) = \sum_{x' \le x} \ \sum_{y' \le y} f(x', y')
```

Se calcula en una sola pasada y, a partir de ella, la suma de **cualquier** rectángulo cuesta 4 lecturas, sea cual sea su tamaño. Si $A$, $B$, $C$ y $D$ son sus esquinas superior izquierda, superior derecha, inferior izquierda e inferior derecha:

```math
\operatorname{Suma}(R) = II(D) - II(B) - II(C) + II(A)
```

Como los rectángulos de una característica comparten esquinas, una de 2 rectángulos cuesta 6 lecturas y una de 3, 8. Con una segunda imagen integral de $f^2$ se calcula también la varianza de cada ventana, $\sigma^2 = E[f^2] - E[f]^2$, que el detector usa para normalizar su contraste: así la respuesta no depende de la iluminación.

#### 3. AdaBoost

Cada característica, por sí sola, es un clasificador **débil** $h_t(\mathbf{x}) \in \{0, 1\}$: un umbral sobre su valor que acierta poco más que el azar. AdaBoost combina muchos en un clasificador **fuerte** mediante una votación ponderada:

```math
C(\mathbf{x}) = \begin{cases} 1 & \text{si } \sum_t \alpha_t \, h_t(\mathbf{x}) \ge \Theta \\ 0 & \text{en otro caso} \end{cases}
```

Durante el entrenamiento, en cada ronda se elige la característica que menos se equivoca, se le da un peso $\alpha_t$ mayor cuanto más acierta y se aumenta el peso de los ejemplos que ha fallado, para que la siguiente ronda se concentre en ellos. De las decenas de miles posibles, el entrenamiento se queda con las que más ayudan: 2135 en este clasificador.

#### 4. Cascada atencional

Los clasificadores fuertes se encadenan en **etapas** de complejidad creciente. Una ventana tiene que superar todas para considerarse un rostro, y en cuanto una etapa la rechaza, se abandona. `haarcascade_frontalface_alt.xml` usa una ventana de 20 × 20 y tiene 22 etapas con 2135 clasificadores débiles en total; la primera etapa usa solo 3 y la segunda, 16.

La inmensa mayoría de las ventanas de una imagen no contiene un rostro y se descarta en las primeras etapas, así que el coste medio por ventana es de unas pocas características, no de miles: en el artículo original, con una cascada de 38 etapas y unas 6000 características, se evaluaban de media unas 10 por ventana.

Cada etapa se entrena para dejar pasar casi todos los rostros aunque acepte muchas ventanas falsas. Con valores ilustrativos, si cada etapa conserva el 99,9 % de los rostros y deja pasar el 50 % de los falsos, al encadenar 22:

```math
\text{falsos positivos: } 0{,}5^{22} \approx 2{,}4 \cdot 10^{-7}, \qquad \text{rostros detectados: } 0{,}999^{22} \approx 97{,}8\ \%
```

#### Búsqueda multiescala: `detectMultiScale`

Los rostros pueden aparecer a cualquier tamaño, así que la búsqueda se repite a varias escalas. OpenCV reduce la imagen por el factor `scaleFactor` en cada paso, formando una pirámide, y recorre cada nivel con la ventana fija de 20 × 20. Con los parámetros del ejemplo (`scaleFactor = 1.1`, `minSize = Size(30, 30)`) y un fotograma de 640 × 480 (estimación analítica):

- La primera escala es $1{,}1^5 \approx 1{,}61$, la primera con la que la ventana ($20 \cdot 1{,}61 \approx 32$ px) alcanza los 30 px de `minSize`. La última es $1{,}1^{33} \approx 23{,}2$, la última con la que la ventana cabe en los 480 px de alto. En total, **29 escalas**.
- OpenCV avanza la ventana de 2 en 2 píxeles en las escalas menores que 2 y de 1 en 1 en las demás. Sumando todas las posiciones de las 29 escalas salen unas **357.000 ventanas por fotograma**.

`scaleFactor` es el compromiso principal entre velocidad y detección: con 1,05 habría el doble de escalas (más lento, pero detecta mejor los tamaños intermedios), y con 1,3, casi tres veces menos.

`minNeighbors = 4` filtra los falsos positivos: un rostro real activa la detección en varias ventanas solapadas (posiciones y escalas vecinas), mientras que un falso positivo suele aparecer aislado. OpenCV agrupa los rectángulos parecidos y descarta los grupos que no tienen suficientes vecinos.

#### Ecualización del histograma

Antes de detectar, el fotograma pasa a gris y se ecualiza con `equalizeHist`. La transformación $g = \operatorname{round}(255 \cdot \mathrm{CDF}(f))$, explicada en [HistogramBrightnessContrast](../HistogramBrightnessContrast/), reparte los niveles por todo el rango y hace la detección menos sensible a las imágenes oscuras o con poco contraste.

### Coste en la Raspberry Pi

Las cifras son estimaciones analíticas para 640 × 480; la cámara puede entregar otra resolución si no admite la pedida.

- **Memoria por fotograma:** BGR de 640 × 480 (922 KB) y su versión en gris (307 KB). Los 29 niveles de la pirámide suman unas 2,2 veces el área del fotograma (unos 680.000 píxeles), y cada nivel necesita sus imágenes integrales, de 4 bytes por elemento en lugar de 1.
- **Cómputo:** es el ejemplo más caro de la carpeta. El número de ventanas es proporcional al área de la imagen, así que reducir el fotograma a la mitad (320 × 240) divide el trabajo por unas 4. Subir `minSize` elimina las escalas pequeñas, que son las que más ventanas tienen. La otra gran palanca es limitar la búsqueda a una ROI alrededor del último rostro detectado (ver [RegionOfInterest](../RegionOfInterest/)).
- **Energía:** el bucle procesa los fotogramas tan rápido como llegan (`waitKey(1)` apenas espera). Si la detección tarda más que el periodo de la cámara, la CPU no descansa nunca mientras el programa está abierto.

**Sin medir:** la memoria pico no se pudo medir en la Raspberry Pi 5, porque la Pi no tiene cámara conectada y el programa termina con 1 al no poder abrirla.

## Descripción del Código

El programa ([`FaceDetect.cpp`](FaceDetect.cpp)) se divide en `main` y la función `detectAndDraw`, definida antes que `main`:

1. **Clasificador:** `classifier.load("haarcascade_frontalface_alt.xml")`; si falla, sale con `EXIT_FAILURE`.
2. **Cámara:** `VideoCapture capture(0)` abre la primera cámara, se comprueba con `isOpened()` y se piden 640×480 con `capture.set(CAP_PROP_FRAME_WIDTH/HEIGHT, …)`. La cámara puede ignorar la petición si no admite esa resolución.
3. **Bucle:** `capture >> frame` lee un fotograma; si llega vacío, el programa sale con `EXIT_FAILURE`. Después se llama a `detectAndDraw` y `waitKey(1)` comprueba si se pulsó `q` o ESC.
4. **`detectAndDraw`:** convierte a grises, ecualiza el histograma para compensar la iluminación y busca rostros:
   ```cpp
   classifier.detectMultiScale(gray, faces, 1.1, 4,
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
- La cámara CSI en Bookworm necesita `libcamerify`, que aún no se ha probado con este ejemplo en la Pi.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Cascade Classifier](https://docs.opencv.org/4.x/db/d28/tutorial_cascade_classifier.html)
- [Clasificadores Haar de OpenCV en GitHub](https://github.com/opencv/opencv/tree/master/data/haarcascades)
- [Raspberry Pi: documentación de la cámara](https://www.raspberrypi.com/documentation/computers/camera_software.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
