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
    - [Problema que resuelve](#problema-que-resuelve)
    - [Fundamentos](#fundamentos)
      - [ROI rectangular: una vista, no una copia](#roi-rectangular-una-vista-no-una-copia)
      - [ROI de forma arbitraria: máscaras](#roi-de-forma-arbitraria-máscaras)
      - [Máscara frente a recorte](#máscara-frente-a-recorte)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Muestra cuatro formas de quedarse solo con una parte de la imagen: un recorte rectangular elegido con el ratón y tres máscaras con forma de rombo, rectángulo y círculo. Cada resultado se muestra en una ventana y se guarda como JPEG. Limitar el procesamiento a una región de interés es una de las técnicas más útiles para reducir el cómputo en un sistema embebido.

## Características

- Selección interactiva de un rectángulo con `selectROI` y recorte con `image(roi)`.
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
   - **ROI manual**: el recorte seleccionado, con su tamaño real.
   - **ROI poligonal**: la imagen completa en negro salvo un rombo central.
   - **ROI rectangular**: en negro salvo el rectángulo central (la mitad del ancho y del alto).
   - **ROI circular**: en negro salvo un círculo central de radio igual al 25 % del lado menor.
3. Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

Los cuatro resultados se guardan en `build/` como `roi_image_target.jpg`, `roi_image_Polygon.jpg`, `roi_image_Rectangular.jpg` y `roi_image_Circular.jpg` (el `.gitignore` del repositorio los excluye).

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./RegionOfInterest
ls roi_image_*.jpg
```

- Tras cerrar con una tecla debe mostrar `salida: 0`, y `ls` debe listar los 4 archivos.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

Rara vez interesa toda la imagen: una matrícula ocupa un rectángulo, el carril de una carretera un trapecio, la pieza que se inspecciona un círculo. Procesar solo la **región de interés** (ROI) reduce el trabajo en proporción a su área y evita que el resto de la escena produzca detecciones falsas. En un sistema embebido es la forma más directa de ganar velocidad.

### Fundamentos

#### ROI rectangular: una vista, no una copia

Un rectángulo $(x_0, y_0, w, h)$ dentro de la imagen se representa con otra cabecera de `Mat` que apunta al mismo búfer de píxeles (ver la estructura de `cv::Mat` en [HelloWorld](../HelloWorld/)):

```math
\text{data}_{\text{ROI}} = \text{data} + y_0 \cdot \text{step} + x_0 \cdot C, \qquad \text{step}_{\text{ROI}} = \text{step}
```

`image(roi)` solo calcula ese puntero y conserva el `step` de la imagen original para saltar de una fila a la siguiente. Es O(1) en tiempo y en memoria, sea cual sea el tamaño del recorte, y cualquier función que reciba la ROI procesa solo sus $w \cdot h$ píxeles. Tiene dos consecuencias:

- La ROI **comparte** los píxeles con la imagen: escribir en `crop` modifica `image`. Para separarlas hace falta `crop.clone()`.
- Salvo que ocupe filas completas, la ROI no es continua en memoria (`isContinuous()` devuelve `false`), porque entre el final de una de sus filas y el principio de la siguiente está el resto de la fila de la imagen. Las funciones de OpenCV lo tienen en cuenta y la recorren fila a fila.

#### ROI de forma arbitraria: máscaras

Para las formas que no son rectángulos se usa una **máscara**: una imagen de 1 canal $m(x, y) \in \{0, 255\}$, del tamaño de la original, en la que la ROI son los píxeles distintos de cero. `image.copyTo(salida, mascara)` aplica:

```math
g(x, y) = \begin{cases} f(x, y) & \text{si } m(x, y) \neq 0 \\ g(x, y) \ \text{(sin cambios)} & \text{si } m(x, y) = 0 \end{cases}
```

Por eso las salidas se crean antes con `Mat::zeros`: lo que la máscara no cubre se queda en negro. La máscara se dibuja **rasterizando** la figura, es decir, decidiendo qué píxeles quedan dentro:

- **Rombo** (`fillConvexPoly`): se recorre la imagen por filas (*scanline*). En un polígono convexo, cada fila corta el contorno en dos puntos como mucho, y se pintan los píxeles entre ellos. Por eso es más rápido que `fillPoly`, que admite polígonos cóncavos y tiene que ordenar todos los cortes de cada fila.
- **Rectángulo** (`rectangle` con grosor `-1`): los $(x, y)$ con $x_0 \le x < x_0 + w$ e $y_0 \le y < y_0 + h$.
- **Círculo** (`circle` con grosor `-1`): los $(x, y)$ con $(x - c_x)^2 + (y - c_y)^2 \le r^2$.

En `avatares.png` (800 × 450 = 360.000 píxeles), las tres máscaras ocupan:

| ROI | Definición | Píxeles | Fracción |
|---|---|---|---|
| Rombo | diagonales de 400 × 225 | 45.401 | 12,6 % |
| Rectángulo | 400 × 225, centrado | 90.000 | 25 % |
| Círculo | radio 112 (el 25 % de 450, truncado) | 39.381 | 10,9 % |

El área teórica del rombo es la mitad del producto de sus diagonales, $400 \cdot 225 / 2 = 45.000$, y la del círculo $\pi \cdot 112^2 \approx 39.408$. La diferencia con lo medido son los píxeles del contorno, que la rasterización cuenta enteros o no cuenta.

#### Máscara frente a recorte

Las tres máscaras cubren como mucho un cuarto de la imagen, pero no ahorran cómputo: `copyTo` con máscara recorre los 360.000 píxeles, y cada salida es una imagen completa de 1,08 MB, casi toda negra. La máscara **selecciona** píxeles; solo el recorte **reduce** el trabajo. Para que una ROI de forma arbitraria abarate el procesado, se combinan las dos: se procesa solo el rectángulo que la envuelve (`boundingRect` del polígono) como vista, y la máscara se recorta a ese mismo rectángulo.

### Coste en la Raspberry Pi

| Matriz | Tamaño |
|---|---|
| `image` (BGR) | 1,08 MB |
| `crop` | 0 bytes de píxeles (vista sobre `image`) |
| 3 máscaras de 1 canal | 3 × 360 KB = 1,08 MB |
| 3 salidas BGR | 3 × 1,08 MB = 3,24 MB |
| **Total** | **≈ 5,4 MB** |

Cada `imwrite` añade el coste de codificar un JPEG (ver [LoadImage](../LoadImage/)) y de escribirlo en la tarjeta SD. Dentro de un bucle conviene evitarlo: además de lento, desgasta la tarjeta, que admite un número limitado de escrituras.

**Medido** en una Raspberry Pi 5 con OpenCV 4.9 (mediana de 3 ejecuciones, con las ventanas abiertas): `RSS máx` = 33.792 kB con la ventana de `selectROI` abierta, es decir, antes de crear las máscaras. Los ≈ 5,4 MB de matrices y las cuatro ventanas de resultado aparecen después de confirmar la selección, y esa parte no se midió.

## Descripción del Código

El programa ([`RegionOfInterest.cpp`](RegionOfInterest.cpp)) carga `avatares.png`, guarda su ancho (`cols`) y alto (`rows`), y aplica cuatro métodos uno detrás de otro:

1. **ROI manual:** `selectROI(image)` devuelve el rectángulo dibujado y `image(roi)` lo recorta. El recorte es una **vista** sobre los datos de la imagen original, no una copia.
2. **ROI poligonal:** se definen 4 vértices en los puntos medios de la imagen, que forman un rombo; `fillConvexPoly` los rellena con 255 en una máscara de un canal y `image.copyTo(polygonResult, polygonMask)` copia solo los píxeles de dentro.
3. **ROI rectangular:** `Rect roiRect(cols·0,25, rows·0,25, cols·0,5, rows·0,5)`, dibujado relleno (grosor `-1`) en una máscara con `rectangle`, y el mismo `copyTo`.
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

- Si se cancela la selección (tecla `c`) o se confirma sin dibujar nada, `selectROI` devuelve un rectángulo vacío: el programa avisa por `cerr`, omite la ROI manual (no muestra ni guarda `roi_image_target.jpg`) y sigue con las otras tres.
- Las tres máscaras usan proporciones fijas del tamaño de la imagen; no se pueden ajustar sin recompilar.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Conceptos básicos de ROI](https://learnopencv.com/how-to-select-a-bounding-box-roi-in-opencv-cpp-python/)
- [Documentación oficial de OpenCV - Clase Mat (`copyTo` con máscara)](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
