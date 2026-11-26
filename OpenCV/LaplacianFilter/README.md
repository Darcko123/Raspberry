# Detección de bordes con filtro Laplaciano en OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Detección de bordes con filtro Laplaciano en OpenCV](#detección-de-bordes-con-filtro-laplaciano-en-opencv)
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
      - [Derivadas de una imagen](#derivadas-de-una-imagen)
      - [El operador Laplaciano](#el-operador-laplaciano)
      - [Sensibilidad al ruido y suavizado previo](#sensibilidad-al-ruido-y-suavizado-previo)
      - [Por qué `CV_16S` y `convertScaleAbs`](#por-qué-cv_16s-y-convertscaleabs)
      - [Paso a escala de grises](#paso-a-escala-de-grises)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Detecta bordes con el operador Laplaciano, que calcula la segunda derivada de la intensidad y responde allí donde el brillo cambia bruscamente. Como la segunda derivada amplifica el ruido, antes se suaviza la imagen con un filtro Gaussiano (ver [GaussianFilter](../GaussianFilter/)).

## Características

- Suavizado previo con `GaussianBlur` de 3×3.
- Conversión a escala de grises, porque el Laplaciano trabaja sobre un solo canal.
- Laplaciano con salida en 16 bits con signo (`CV_16S`) para no perder los valores negativos.
- Conversión del resultado a 8 bits con valor absoluto (`convertScaleAbs`).

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
cd OpenCV/LaplacianFilter
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
./LaplacianFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./LaplacianFilter`.

### Resultado esperado

Se abre una sola ventana, **Filtro Laplaciano - Detección de Bordes**, con fondo casi negro y los contornos de la imagen en tonos claros. No se muestra la imagen original. Al pulsar cualquier tecla con la ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./LaplacianFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que la ventana queda abierta, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

Detectar **bordes**: los lugares donde la intensidad cambia bruscamente, que suelen coincidir con los contornos de los objetos. Un borde concentra mucha información en pocos píxeles, y por eso es el punto de partida de la detección de formas, la medición y el seguimiento.

### Fundamentos

#### Derivadas de una imagen

Un borde es un cambio rápido de intensidad, así que se detecta con derivadas. En una imagen discreta se aproximan con diferencias finitas entre píxeles vecinos:

```math
\frac{\partial f}{\partial x} \approx f(x + 1, y) - f(x, y), \qquad
\frac{\partial^2 f}{\partial x^2} \approx f(x + 1, y) - 2 f(x, y) + f(x - 1, y)
```

Al atravesar un borde, la primera derivada pasa por un **máximo** y la segunda pasa por **cero**: es positiva a un lado del borde y negativa al otro.

#### El operador Laplaciano

El Laplaciano suma las segundas derivadas en las dos direcciones:

```math
\nabla^2 f = \frac{\partial^2 f}{\partial x^2} + \frac{\partial^2 f}{\partial y^2}
```

Es **isótropo**: responde igual a un borde sea cual sea su orientación, con una sola convolución y sin calcular la dirección. Sustituyendo las diferencias finitas se obtiene el *kernel* clásico de 4 vecinos, que OpenCV usa con `ksize = 1`:

```math
\begin{bmatrix} 0 & 1 & 0 \\ 1 & -4 & 1 \\ 0 & 1 & 0 \end{bmatrix}
```

Con `ksize = 3`, como en este ejemplo, OpenCV calcula cada segunda derivada con un *kernel* de Sobel, que deriva en una dirección y suaviza con `[1 2 1]` en la otra. La suma de los dos da el *kernel* que se aplica de verdad:

```math
\underbrace{\begin{bmatrix} 1 & -2 & 1 \\ 2 & -4 & 2 \\ 1 & -2 & 1 \end{bmatrix}}_{\partial^2 / \partial x^2}
+ \underbrace{\begin{bmatrix} 1 & 2 & 1 \\ -2 & -4 & -2 \\ 1 & 2 & 1 \end{bmatrix}}_{\partial^2 / \partial y^2}
= \begin{bmatrix} 2 & 0 & 2 \\ 0 & -8 & 0 \\ 2 & 0 & 2 \end{bmatrix}
```

En los dos casos los pesos suman 0: sobre una zona uniforme la salida es 0, y el Laplaciano solo responde a los cambios.

#### Sensibilidad al ruido y suavizado previo

La segunda derivada amplifica las frecuencias altas. En el dominio de la frecuencia, el Laplaciano multiplica cada componente por $-4\pi^2(u^2 + v^2)$: cuanto más fina es una variación, más se refuerza. El ruido del sensor es justo eso, variaciones finas, y el Laplaciano directo da una imagen llena de puntos. Por eso el ejemplo suaviza antes con un filtro Gaussiano ([GaussianFilter](../GaussianFilter/)). Como los dos operadores son lineales, aplicar uno tras otro equivale a convolucionar con un único *kernel*, el **Laplaciano de la Gaussiana** (LoG), que es la base del detector de bordes de Marr y Hildreth:

```math
\nabla^2 (G_\sigma * f) = (\nabla^2 G_\sigma) * f
```

#### Por qué `CV_16S` y `convertScaleAbs`

Con el *kernel* `[2 0 2; 0 -8 0; 2 0 2]` y una entrada de 0 a 255, la salida puede ir de $-8 \cdot 255 = -2040$ a $+2040$. En `CV_8U` (0 a 255), los valores negativos se saturarían a 0 y se perdería la mitad de cada borde; `CV_16S` (−32.768 a 32.767) los guarda sin pérdida. Para mostrar el resultado, `convertScaleAbs` vuelve a 8 bits con:

```math
g(x, y) = \min\left(255, \ \left| \nabla^2 f(x, y) \right| \right)
```

El valor absoluto hace que los dos lados del borde se vean claros, pero pierde el signo, y con él la posibilidad de buscar los cruces por cero, que es la forma de localizar el borde con precisión de un píxel.

#### Paso a escala de grises

El Laplaciano se calcula sobre un solo canal. `COLOR_BGR2GRAY` combina los tres con los pesos de luminancia de la norma ITU-R BT.601, que reflejan la sensibilidad del ojo humano, mayor al verde que al rojo y mucho mayor que al azul:

```math
Y = 0{,}299 \, R + 0{,}587 \, G + 0{,}114 \, B
```

### Coste en la Raspberry Pi

| Matriz | Tamaño |
|---|---|
| `imagenOriginal` (BGR, se suaviza sobre sí misma) | 1,08 MB |
| `grayImage` | 360 KB |
| `laplacian16S` (2 bytes por píxel) | 720 KB |
| `laplacianResult` | 360 KB |
| **Total** | **≈ 2,5 MB** |

- **Orden de las operaciones:** el ejemplo suaviza la imagen a color y después la pasa a gris. La conversión a gris y el filtro Gaussiano son lineales, así que el orden no cambia el resultado (salvo por el redondeo), pero suavizar después de convertir procesa 1 canal en lugar de 3: la tercera parte del trabajo del suavizado.
- **Cómputo:** el *kernel* `[2 0 2; 0 -8 0; 2 0 2]` tiene 5 coeficientes distintos de cero, así que el Laplaciano cuesta unas 5 multiplicaciones y sumas por píxel, unos 1,8 millones en total para los 360.000 píxeles en gris.

## Descripción del Código

El programa ([`LaplacianFilter.cpp`](LaplacianFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `empty()`.
2. **Suavizado:** `GaussianBlur` de 3×3 con sigma automática, aplicado sobre la propia imagen original (entrada y salida son la misma `Mat`).
3. **Escala de grises:** `cvtColor(..., COLOR_BGR2GRAY)`.
4. **Laplaciano:**
   ```cpp
   Laplacian(grayImage, laplacian16S, CV_16S, 3, 1, 0, BORDER_DEFAULT);
   ```
   La salida es `CV_16S` porque la segunda derivada de una imagen de 8 bits toma valores negativos y mayores que 255; con `CV_8U` se recortarían y se perderían la mitad de los bordes.
5. **Vuelta a 8 bits:** `convertScaleAbs` toma el valor absoluto y satura a 0–255, para que los dos lados de cada borde se vean claros.
6. **Visualización:** `imshow` del resultado y `waitKey(0)`. La línea `imwrite("bordes_laplaciano.jpg", ...)` está comentada; al descomentarla, el resultado se guarda en `build/`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `GaussianBlur` | imgproc | Reduce el ruido antes de derivar |
| `cvtColor` | imgproc | Convierte a escala de grises |
| `Laplacian` | imgproc | Calcula la segunda derivada en `CV_16S` |
| `convertScaleAbs` | core | Valor absoluto y conversión a 8 bits |
| `imshow` | highgui | Muestra los bordes |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de la ventana |
| `imwrite` (comentada) | imgcodecs | Guardaría el resultado en disco |

## Limitaciones conocidas

- La imagen original se sobrescribe con la suavizada, así que no se puede mostrar junto al resultado sin cambiar el código.
- El Laplaciano no distingue la dirección del borde ni aplica umbral: todo cambio de intensidad aparece con más o menos brillo. Para bordes finos y binarios, ver [CannyFilter](../CannyFilter/).

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Tutoriales de OpenCV](https://docs.opencv.org/4.x/d9/df8/tutorial_root.html)
- [Laplace Operator](https://docs.opencv.org/4.x/d5/db5/tutorial_laplace_operator.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
