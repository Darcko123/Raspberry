# Suavizado con filtro Gaussiano en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Suavizado con filtro Gaussiano en OpenCV](#suavizado-con-filtro-gaussiano-en-opencv)
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
      - [La función Gaussiana](#la-función-gaussiana)
      - [Convolución: un promedio ponderado](#convolución-un-promedio-ponderado)
      - [El *kernel* real de este ejemplo](#el-kernel-real-de-este-ejemplo)
      - [Separabilidad](#separabilidad)
      - [Interpretación en frecuencia](#interpretación-en-frecuencia)
      - [Tamaño del *kernel* y bordes](#tamaño-del-kernel-y-bordes)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Aplica un filtro Gaussiano a la imagen a color y muestra el original junto al resultado. El filtro Gaussiano es un suavizado que reduce el ruido y los detalles finos ponderando cada píxel vecino según una campana de Gauss, y es el paso previo habitual de los detectores de bordes ([LaplacianFilter](../LaplacianFilter/) y [CannyFilter](../CannyFilter/)).

## Características

- Suavizado con `GaussianBlur` y un *kernel* de 3×3.
- Sigma calculada automáticamente a partir del tamaño del *kernel*.
- Funciona directamente sobre los 3 canales de la imagen a color.
- Visualización del original y del resultado en dos ventanas.

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
cd OpenCV/GaussianFilter
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
./GaussianFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./GaussianFilter`.

### Resultado esperado

El programa imprime `Presione cualquier tecla para salir...` y abre dos ventanas:

1. **Imagen original**: la imagen cargada.
2. **Filtro Gaussiano - Suavizado de imagen**: la imagen ligeramente desenfocada; se nota sobre todo en los bordes y en las texturas finas.

Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./GaussianFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

Toda imagen de una cámara trae **ruido**: variaciones aleatorias de intensidad que vienen del propio sensor (ruido térmico, la llegada aleatoria de los fotones) y de la compresión. Los algoritmos que derivan la imagen, como los detectores de bordes, amplifican ese ruido. El filtro Gaussiano lo atenúa suavizando la imagen, a cambio de difuminar un poco los detalles finos.

### Fundamentos

#### La función Gaussiana

La teoría del filtro Gaussiano se fundamenta en usar una función Gaussiana (la curva de campana de la distribución normal) como respuesta al impulso del filtro. En dos dimensiones y centrada en el origen:

```math
G_\sigma(x, y) = \frac{1}{2\pi\sigma^2} \, e^{-\frac{x^2 + y^2}{2\sigma^2}}
```

- La desviación estándar $\sigma$ fija el ancho de la campana y, con él, la fuerza del suavizado.
- El factor $1 / (2\pi\sigma^2)$ hace que el volumen bajo la superficie valga 1, de modo que el filtro no cambia el brillo medio de la imagen.

#### Convolución: un promedio ponderado

Filtrar es **convolucionar** la imagen de entrada $f$ con la Gaussiana. En una imagen discreta, con un *kernel* de $(2k + 1) \times (2k + 1)$:

```math
g(x, y) = \sum_{i=-k}^{k} \sum_{j=-k}^{k} G(i, j) \, f(x - i, \, y - j), \qquad \sum_{i, j} G(i, j) = 1
```

Cada píxel de salida es un **promedio ponderado** de sus vecinos, donde el peso decrece con la distancia al centro. Frente a un filtro de media, en el que todos los vecinos pesan igual, el vecino inmediato cuenta más que el lejano, y la imagen se suaviza sin que los bordes se emborronen tanto.

#### El *kernel* real de este ejemplo

Con `Size(3, 3)` y sigma `0`, OpenCV no evalúa la fórmula: para los tamaños 3, 5 y 7 usa una tabla fija. En 3×3 es el *kernel* binomial `[1 2 1] / 4` en cada dirección. Los coeficientes binomiales son el resultado de convolucionar `[1 1] / 2` consigo mismo, y por el teorema central del límite tienden a una Gaussiana al repetir la operación; con 3 coeficientes, la aproximación corresponde a $\sigma = \sqrt{0{,}5} \approx 0{,}71$:

```math
G_{3 \times 3} = \frac{1}{4}\begin{bmatrix} 1 \\ 2 \\ 1 \end{bmatrix} \cdot \frac{1}{4}\begin{bmatrix} 1 & 2 & 1 \end{bmatrix} = \frac{1}{16}\begin{bmatrix} 1 & 2 & 1 \\ 2 & 4 & 2 \\ 1 & 2 & 1 \end{bmatrix}
```

El píxel central pesa el 25 %, cada vecino en cruz el 12,5 % y cada vecino en diagonal el 6,25 %. La fórmula de la documentación, $\sigma = 0{,}3 \, ((k - 1) \cdot 0{,}5 - 1) + 0{,}8$, solo se aplica con sigma `0` a partir de 9×9.

#### Separabilidad

La exponencial de una suma es el producto de las exponenciales, así que la Gaussiana 2D se descompone en el producto de dos Gaussianas 1D:

```math
G_\sigma(x, y) = g_\sigma(x) \, g_\sigma(y), \qquad g_\sigma(t) = \frac{1}{\sqrt{2\pi} \, \sigma} \, e^{-\frac{t^2}{2\sigma^2}}
```

Por eso el filtro se aplica en dos pasadas, una horizontal y otra vertical, con el *kernel* 1D, y el resultado es idéntico. Un *kernel* de $k \times k$ cuesta $k^2$ multiplicaciones por píxel en 2D y solo $2k$ separado: 9 frente a 6 en 3×3, y 49 frente a 14 en 7×7. `GaussianBlur` lo hace así internamente.

#### Interpretación en frecuencia

La transformada de Fourier de una Gaussiana es otra Gaussiana, de anchura inversa:

```math
\mathcal{F}\{g_\sigma\}(u) = e^{-2\pi^2\sigma^2 u^2}
```

El filtro es, por tanto, un **paso bajo**: atenúa las frecuencias altas (ruido, texturas finas, bordes nítidos) y deja pasar las bajas (las zonas uniformes). Cuanto mayor es $\sigma$ en el espacio, más estrecha es la banda que pasa. A diferencia del filtro de media, cuya respuesta en frecuencia es una función sinc con lóbulos laterales, la Gaussiana decae sin oscilar y no produce artefactos de anillo. La teoría del espacio de escalas (*scale space*) demuestra además que es el único *kernel* lineal que, al aumentar $\sigma$, no crea estructuras nuevas en la imagen, y por eso es la base de detectores multiescala como SIFT.

#### Tamaño del *kernel* y bordes

- La Gaussiana nunca llega a cero, así que el *kernel* la trunca. En cada dirección, el 99,7 % del peso está a menos de $3\sigma$ del centro, de ahí la regla habitual $k \approx 6\sigma + 1$ (redondeado a impar). Con un *kernel* demasiado pequeño para su $\sigma$, el filtro deja de ser Gaussiano: [CannyFilter](../CannyFilter/) usa σ = 3 en 3×3 y sus pesos son 0,327, 0,346 y 0,327, casi un filtro de media.
- En los bordes de la imagen faltan vecinos. `BORDER_DEFAULT`, que es `BORDER_REFLECT_101`, los inventa reflejando la imagen sin repetir el píxel del borde: `gfedcb|abcdefgh|gfedcba`.

### Coste en la Raspberry Pi

- **Memoria:** la original y el resultado, 2 × 1,08 MB = 2,16 MB, más unas pocas filas de búfer para las dos pasadas.
- **Cómputo:** 800 × 450 píxeles × 3 canales = 1,08 millones de muestras. Con el *kernel* separable de 3, son unas 6 multiplicaciones y sumas por muestra, unos 6,5 millones en total. Para imágenes de 8 bits, OpenCV usa aritmética entera de punto fijo en lugar de coma flotante: el resultado es exacto e idéntico en cualquier CPU, y se vectoriza con NEON.
- Suavizar la imagen a color cuesta el triple que en gris. Si el paso siguiente trabaja en gris, como en [LaplacianFilter](../LaplacianFilter/) y [CannyFilter](../CannyFilter/), conviene convertir antes.

**Medido** en una Raspberry Pi 5 con OpenCV 4.9 (mediana de 3 ejecuciones, con las ventanas abiertas): `RSS máx` = 35.680 kB, 4,3 MB más que [LoadImage](../LoadImage/), que carga la misma imagen y abre una sola ventana: 1,08 MB son el resultado y el resto, la segunda ventana.

## Descripción del Código

El programa ([`GaussianFilter.cpp`](GaussianFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `image.empty()`.
2. **Filtro:** una sola llamada aplica el suavizado a los 3 canales:
   ```cpp
   GaussianBlur(image, blurred,
                Size(kernelSize, kernelSize), // 3×3, debe ser impar
                0, 0,                         // sigma X e Y: 0 = calculada
                BORDER_DEFAULT);
   ```
   Con sigma `0` y un *kernel* de 3×3, OpenCV usa el *kernel* binomial fijo `[1 2 1] / 4` en cada dirección (ver [Fundamentos](#fundamentos)). `BORDER_DEFAULT` refleja los píxeles del borde para poder aplicar el *kernel* en los bordes de la imagen.
3. **Visualización:** `imshow` del original y del resultado, mensaje por consola y `waitKey(0)`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `GaussianBlur` | imgproc | Suaviza la imagen con un *kernel* Gaussiano de 3×3 |
| `imshow` | highgui | Muestra el original y el resultado |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- El tamaño del *kernel* (3×3) está fijo en el código, así que el desenfoque es sutil. Para verlo más marcado hay que cambiar `kernelSize` (5, 7, …) y recompilar.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Función GaussianBlur](https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#gae8bdcd9154ed5ca3cbc1766d960f45c1)
- [Smoothing Images](https://docs.opencv.org/4.x/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
