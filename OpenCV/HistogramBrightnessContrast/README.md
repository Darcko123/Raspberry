# Ajuste de brillo y contraste con histograma en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Ajuste de brillo y contraste con histograma en OpenCV](#ajuste-de-brillo-y-contraste-con-histograma-en-opencv)
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
      - [El histograma](#el-histograma)
      - [Brillo y contraste: una transformación lineal](#brillo-y-contraste-una-transformación-lineal)
      - [Las fórmulas del ejemplo](#las-fórmulas-del-ejemplo)
      - [Relación con la ecualización](#relación-con-la-ecualización)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Aplicación interactiva que ajusta el brillo y el contraste de la imagen en escala de grises mediante dos *trackbars* y redibuja su histograma cada vez que se mueven. Enseña la transformación lineal de intensidad `dst = a·src + b` y cómo se refleja en la distribución de grises.

## Características

- Carga de la imagen directamente en escala de grises.
- Dos *trackbars*, **Brillo** y **Contraste**, en el rango 0–200 (100 = sin cambio).
- Transformación lineal con saturación a 0–255 mediante `convertTo`.
- Histograma de 256 niveles recalculado y dibujado en cada cambio.

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
cd OpenCV/HistogramBrightnessContrast
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
./HistogramBrightnessContrast
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./HistogramBrightnessContrast`.

### Resultado esperado

Se abren dos ventanas:

1. **Imagen original**: la imagen en escala de grises con los *trackbars* **Brillo** y **Contraste**.
2. **Histograma**: una curva azul de 512×400 píxeles con la distribución de los 256 niveles de gris.

Al subir el brillo, la curva se desplaza a la derecha; al subir el contraste, se ensancha y se acumula en los extremos (0 y 255). Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./HistogramBrightnessContrast
```

- Con los dos *trackbars* en 100, la imagen debe verse igual que la original en grises (la fórmula da `a = 256/255`, así que algunos píxeles cambian como mucho 1 nivel). Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

Ver cómo se reparten las intensidades de una imagen y corregir su exposición: una foto subexpuesta, sobreexpuesta o con poco contraste. Es el ajuste más barato que existe, una operación por píxel, y el primer paso para entender por qué un algoritmo funciona con una iluminación y falla con otra.

### Fundamentos

#### El histograma

El histograma de una imagen en gris de 8 bits cuenta cuántos píxeles tienen cada nivel de intensidad:

```math
h(k) = \#\{ (x, y) : f(x, y) = k \}, \quad k = 0, \dots, 255, \qquad p(k) = \frac{h(k)}{M \cdot N}
```

Normalizado, $p(k)$ es la probabilidad de que un píxel tomado al azar tenga el nivel $k$. El histograma no dice dónde está cada píxel, solo cuántos hay de cada nivel, pero su forma revela la exposición:

- Concentrado a la izquierda: imagen oscura. A la derecha: imagen clara.
- Estrecho: poco contraste, la imagen usa pocos de los 256 niveles disponibles.
- Con picos en 0 o en 255: zonas saturadas, cuya información se ha perdido.

#### Brillo y contraste: una transformación lineal

Ajustar el brillo y el contraste es una **operación puntual**: cada píxel de salida depende solo del píxel de entrada en la misma posición:

```math
g(x, y) = \operatorname{sat}\big( a \cdot f(x, y) + b \big), \qquad \operatorname{sat}(v) = \min\big(255, \max(0, \operatorname{round}(v))\big)
```

- $b$ (brillo) desplaza el histograma a la izquierda o a la derecha.
- $a$ (contraste) lo estira ($a > 1$) o lo comprime ($a < 1$).
- $\operatorname{sat}$ es la saturación que aplica `convertTo` al volver a `CV_8U`: todo lo que queda fuera del rango se acumula en 0 o en 255.

El nivel $k$ de la entrada pasa al nivel $a k + b$ de la salida. Al estirar, niveles consecutivos de la entrada caen separados en la salida y aparecen niveles vacíos entre ellos (el histograma toma forma de peine): no se crea información, solo se reparte la que había. Al comprimir, varios niveles de entrada caen en el mismo nivel de salida y ya no se pueden separar.

#### Las fórmulas del ejemplo

Los *trackbars* van de 0 a 200 y el código les resta 100, así que el brillo $\beta$ y el contraste $c$ van de −100 a +100. Con contraste positivo:

```math
\delta = 1{,}27 \, c, \qquad a = \frac{255}{255 - 2\delta}, \qquad b = a \, (\beta - \delta)
\quad \Rightarrow \quad g = a \, (f + \beta - \delta)
```

El intervalo de entrada $[\delta - \beta, \ 255 - \delta - \beta]$ se estira hasta ocupar todo $[0, 255]$, y lo que queda fuera se satura. En el extremo, $c = 100$ da $\delta = 127$ y $a = 255$: la imagen queda prácticamente en blanco y negro, como con un umbral. Con contraste negativo o nulo:

```math
\delta = -1{,}28 \, c, \qquad a = \frac{256 - 2\delta}{255}, \qquad b = a \beta + \delta
\quad \Rightarrow \quad g = a \, (f + \beta) + \delta
```

Ahora es el rango de salida el que se reduce, a $[\delta, \ 256 - \delta]$ para $\beta = 0$. En el extremo, $c = -100$ da $\delta = 128$ y $a = 0$: toda la imagen queda en el gris 128.

En la posición neutra (los dos *trackbars* en 100), el código toma esta segunda rama con $\delta = 0$, así que $a = 256 / 255 \approx 1{,}004$ en lugar de 1: los niveles a partir de 128 suben uno (128 pasa a 129). Es la diferencia de 1 nivel que se menciona en la [Verificación](#verificación).

#### Relación con la ecualización

Una transformación lineal desplaza y estira el histograma, pero no cambia su forma. La **ecualización del histograma** (`equalizeHist`, que usa [FaceDetect](../FaceDetect/)) aplica en cambio una función no lineal, la distribución acumulada, que reparte los niveles para que el histograma resultante sea aproximadamente plano:

```math
g = \operatorname{round}\left( 255 \sum_{j=0}^{f} p(j) \right)
```

### Coste en la Raspberry Pi

- **Memoria:** la imagen en gris (360 KB), el resultado `dst` (360 KB) y la imagen del histograma, de 512 × 400 × 3 = 614 KB. El histograma en sí son 256 valores `float`: 1 KB.
- **Cómputo:** cada movimiento de un *trackbar* recorre la imagen dos veces: `convertTo` hace una multiplicación y una suma por píxel (360.000 en total) y `calcHist` un incremento por píxel. Todo ocurre dentro del *callback*, así que mientras no se toca nada el programa no consume CPU: es el patrón contrario al bucle de [CannyFilter](../CannyFilter/).
- **Tablas de consulta:** una operación puntual sobre 8 bits solo puede dar 256 resultados distintos, así que se puede precalcular en una tabla de 256 entradas y aplicar con `LUT`: cada píxel cuesta una lectura en lugar de una multiplicación en coma flotante. Es la técnica habitual para corregir brillo, contraste o gamma en tiempo real.

**Medido** en una Raspberry Pi 5 con OpenCV 4.9 (mediana de 3 ejecuciones, con las ventanas abiertas): `RSS máx` = 42.288 kB, 10,9 MB más que [LoadImage](../LoadImage/), que abre una sola ventana. Las matrices de este ejemplo no llegan a 1,4 MB (la imagen en gris es más pequeña que la de color de LoadImage), así que casi toda la diferencia es de la interfaz: la segunda ventana y los dos *trackbars*.

## Descripción del Código

El programa ([`HistogramBrightnessContrast.cpp`](HistogramBrightnessContrast.cpp)) se organiza en `main` y una función *callback*:

1. **`main`:** carga la imagen con `imread("avatares.png", IMREAD_GRAYSCALE)`, crea las ventanas, registra los dos *trackbars* con `updateBrightnessContrast` como *callback*, la llama una vez para dibujar el estado inicial y espera con `waitKey(0)`. Mientras espera, `waitKey` procesa los eventos de los *trackbars*.
2. **Parámetros de la transformación** (`updateBrightnessContrast`): los valores se centran en 0 (`brillo = brightnessTrackbar − 100`, `contraste = contrastTrackbar − 100`) y se calculan `a` (ganancia) y `b` (desplazamiento), según la fórmula de Werner D. Streidt:
   - **Contraste > 0:** `δ = 127·c/100`, `a = 255 / (255 − 2δ)`, `b = a·(brillo − δ)`. Como `a > 1`, el rango `[δ, 255 − δ]` se estira hasta `[0, 255]` y aumenta el contraste.
   - **Contraste ≤ 0:** `δ = −128·c/100`, `a = (256 − 2δ) / 255`, `b = a·brillo + δ`. Como `a ≤ 1`, todo el rango se comprime hacia el centro y disminuye el contraste.
3. **Aplicación:** `image.convertTo(dst, CV_8U, a, b)` calcula `a·src + b` y satura el resultado a 0–255.
4. **Histograma:** `calcHist` cuenta los píxeles de cada uno de los 256 niveles, `normalize(..., NORM_MINMAX)` escala el máximo a la altura de la ventana (400 px) y un bucle de `line` une los 256 puntos.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` en escala de grises |
| `namedWindow` | highgui | Crea las ventanas antes de añadir los *trackbars* |
| `createTrackbar` | highgui | Crea los controles **Brillo** y **Contraste** |
| `Mat::convertTo` | core | Aplica `a·src + b` con saturación a 8 bits |
| `calcHist` | imgproc | Calcula el histograma de 256 niveles |
| `normalize` | core | Escala el histograma a la altura de la ventana |
| `line` | imgproc | Dibuja la curva del histograma |
| `imshow` | highgui | Muestra la imagen ajustada y el histograma |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de los *trackbars* |

## Limitaciones conocidas

- El estado se guarda en variables globales (`image`, `hist`, `brightnessTrackbar`, `contrastTrackbar`) porque el *callback* no recibe datos propios (`userdata` es `nullptr`).
- El histograma se normaliza a su propio máximo en cada cambio, así que la altura de la curva es relativa: no sirve para comparar el número absoluto de píxeles entre dos ajustes.
- Con OpenCV 4.6 y posteriores se muestra el aviso `createTrackbar ... Using 'value' pointer is unsafe and deprecated`, porque se pasa la variable por puntero. No afecta al funcionamiento.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Changing the contrast and brightness of an image](https://docs.opencv.org/4.x/d3/dc1/tutorial_basic_linear_transform.html)
- [Histogram Calculation](https://docs.opencv.org/4.x/d8/dbc/tutorial_histogram_calculation.html)
- [Adding a Trackbar to our applications](https://docs.opencv.org/4.x/da/d6a/tutorial_trackbar.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
