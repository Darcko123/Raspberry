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
    - [Problema que resuelve](#problema-que-resuelve)
    - [Fundamentos](#fundamentos)
      - [1. Suavizado](#1-suavizado)
      - [2. Gradiente](#2-gradiente)
      - [3. Supresión de no máximos](#3-supresión-de-no-máximos)
      - [4. Umbral con histéresis](#4-umbral-con-histéresis)
      - [Escala de los umbrales](#escala-de-los-umbrales)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
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

### Problema que resuelve

El Laplaciano ([LaplacianFilter](../LaplacianFilter/)) marca los bordes como franjas gruesas de brillo variable y deja pasar mucho ruido. Lo que suelen necesitar las etapas siguientes (buscar contornos, rectas o formas) es otra cosa: un mapa **binario**, con bordes de **un píxel** de ancho, continuos y sin respuestas falsas. Eso es lo que produce el detector de Canny.

### Fundamentos

John Canny (1986) planteó la detección de bordes como un problema de optimización con tres criterios:

1. **Buena detección:** perder pocos bordes reales y marcar pocos falsos.
2. **Buena localización:** que el borde detectado esté lo más cerca posible del real.
3. **Respuesta única:** un solo píxel por cada borde.

El algoritmo que se deduce de ellos tiene cuatro etapas.

#### 1. Suavizado

Igual que en el Laplaciano, primero se reduce el ruido con un filtro Gaussiano ([GaussianFilter](../GaussianFilter/)). En este ejemplo se hace fuera de `Canny`, con `GaussianBlur` de 3×3 y σ = 3. El último argumento de `Canny` (`3`) no es un suavizado, aunque el comentario del código lo llame así: es el tamaño del *kernel* de Sobel de la etapa siguiente.

#### 2. Gradiente

La primera derivada en cada dirección se calcula con los *kernels* de Sobel, que derivan en una dirección y suavizan en la otra:

```math
K_x = \begin{bmatrix} -1 & 0 & 1 \\ -2 & 0 & 2 \\ -1 & 0 & 1 \end{bmatrix}, \qquad
K_y = \begin{bmatrix} -1 & -2 & -1 \\ 0 & 0 & 0 \\ 1 & 2 & 1 \end{bmatrix}, \qquad
G_x = K_x * f, \quad G_y = K_y * f
```

El vector $(G_x, G_y)$ es el **gradiente**: apunta hacia donde la intensidad crece más deprisa y es perpendicular al borde. Su módulo mide la fuerza del borde y su ángulo, la orientación:

```math
\left| \nabla f \right| \approx |G_x| + |G_y|, \qquad \theta = \operatorname{atan2}(G_y, \, G_x)
```

OpenCV usa por defecto la norma L1 ($|G_x| + |G_y|$) porque evita la raíz cuadrada. Con `L2gradient = true` usaría la norma euclídea $\sqrt{G_x^2 + G_y^2}$, más precisa y más cara.

#### 3. Supresión de no máximos

El gradiente de un borde real ocupa varios píxeles de ancho. Para quedarse con uno solo, el ángulo $\theta$ se redondea a una de cuatro direcciones (0°, 45°, 90° o 135°) y cada píxel se compara con sus dos vecinos **en la dirección del gradiente**, es decir, a uno y otro lado del borde. Si no es mayor que los dos, se descarta. Queda una línea de un píxel sobre la cresta del gradiente.

#### 4. Umbral con histéresis

Para decidir qué crestas son bordes se usan dos umbrales, `cannyLow` ($T_L$) y `cannyHigh` ($T_H$):

| Módulo del gradiente | Clasificación |
|---|---|
| Mayor o igual que $T_H$ | **Borde fuerte**: se acepta siempre |
| Entre $T_L$ y $T_H$ | **Borde débil**: se acepta solo si está conectado (con vecindad de 8) a un borde fuerte, directamente o a través de otros débiles |
| Menor que $T_L$ | Se descarta |

Con un único umbral habría que elegir entre cortar los bordes reales donde se debilitan o aceptar ruido. Con dos, un borde que empieza en un tramo fuerte se sigue por los tramos débiles conectados, y las respuestas aisladas del ruido, que no llegan a fuertes, desaparecen. Canny recomendaba una relación $T_H : T_L$ de entre 2:1 y 3:1; los valores iniciales del ejemplo (150 y 50) están en 3:1.

#### Escala de los umbrales

Los umbrales se comparan con el módulo del gradiente, no con la intensidad de los píxeles. Con Sobel de 3×3 sobre una imagen de 8 bits, $|G_x|$ puede llegar a $4 \cdot 255 = 1020$, y en `avatares.png`, después del suavizado, el máximo de $|G_x| + |G_y|$ es 644. Como los *trackbars* llegan solo a 255, permiten explorar la parte baja del rango, la de los bordes más débiles: solo el 1,6 % de los píxeles supera 255.

### Coste en la Raspberry Pi

- **Memoria:** la original en BGR (1,08 MB) y tres imágenes de un canal (gris, suavizada y bordes, 3 × 360 KB), unos 2,2 MB, más los búferes internos de `Canny` para el gradiente.
- **Cómputo y energía:** el bucle llama a `Canny` y a los cuatro `imshow` cada 30 ms, unas 33 veces por segundo, aunque nadie toque los *trackbars*, y repinta cuatro ventanas con imágenes que no han cambiado. En una Pi alimentada con batería, eso es consumo continuo para un resultado constante. Lo eficiente sería recalcular solo cuando cambia un umbral, en el *callback* del *trackbar* como hace [HistogramBrightnessContrast](../HistogramBrightnessContrast/), y dejar la CPU en reposo con `waitKey(0)`.

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
