# Segmentación por color en HSV con OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Segmentación por color en HSV con OpenCV](#segmentación-por-color-en-hsv-con-opencv)
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
      - [Del cubo RGB al cono HSV](#del-cubo-rgb-al-cono-hsv)
      - [HSV en OpenCV con 8 bits](#hsv-en-opencv-con-8-bits)
      - [Máscaras binarias](#máscaras-binarias)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Convierte la imagen al espacio de color HSV (tono, saturación y valor) y extrae las zonas rojas, verdes y azules con rangos de tono fijos. A diferencia de [RGBFilter](../RGBFilter/), que compara canales píxel a píxel, aquí se usan máscaras binarias y operaciones sobre la matriz completa, y los colores poco saturados (grises) quedan fuera.

## Características

- Conversión de BGR a HSV con `cvtColor`.
- Una máscara binaria por color con `inRange`.
- El rojo usa dos rangos de tono, porque en HSV el tono es circular y el rojo está a ambos lados del 0.
- Aplicación de las máscaras con `bitwise_and` para conservar los colores originales.

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
cd OpenCV/HSVFilter
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
./HSVFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./HSVFilter`.

### Resultado esperado

Se abren cuatro ventanas:

1. **Imagen original**: la imagen cargada.
2. **Rojo (HSV)**: solo las zonas rojas, con su color original; el resto, en negro.
3. **Verde (HSV)**: solo las zonas verdes.
4. **Azul (HSV)**: solo las zonas azules.

Las zonas grises, oscuras o poco saturadas no aparecen en ninguna salida. Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./HSVFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo cargar la imagen 'avatares.png'` y salir con `1`.

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

El mismo que [RGBFilter](../RGBFilter/), separar los píxeles rojos, verdes y azules, pero de forma robusta frente a los cambios de iluminación y sin clasificar como color los grises, los blancos y las sombras.

### Fundamentos

#### Del cubo RGB al cono HSV

HSV describe el color con tres magnitudes cercanas a cómo lo percibimos:

- **Tono** (H, *hue*): qué color es, como un ángulo en la rueda de colores (0° rojo, 120° verde, 240° azul).
- **Saturación** (S): cuánto se aleja del gris (0 = gris, máximo = color puro).
- **Valor** (V): cuánta luz tiene (0 = negro).

Con $R, G, B \in [0, 1]$, $V = \max(R, G, B)$, $m = \min(R, G, B)$ y $\Delta = V - m$:

```math
S = \begin{cases} \Delta / V & \text{si } V > 0 \\ 0 & \text{si } V = 0 \end{cases}
\qquad
H = \begin{cases}
60^\circ \cdot \dfrac{G - B}{\Delta} & \text{si } V = R \\[6pt]
60^\circ \cdot \left(2 + \dfrac{B - R}{\Delta}\right) & \text{si } V = G \\[6pt]
60^\circ \cdot \left(4 + \dfrac{R - G}{\Delta}\right) & \text{si } V = B
\end{cases}
```

(si $H$ sale negativo se le suman 360°; si $\Delta = 0$ el píxel es gris y el tono no está definido). Geométricamente equivale a mirar el cubo RGB a lo largo de la diagonal de los grises: el hexágono que se ve es la rueda de tonos, la distancia a la diagonal es la saturación y la altura a lo largo de ella, el valor.

La propiedad que interesa es que **el tono no depende de la cantidad de luz**. Oscurecer un objeto multiplica $R$, $G$ y $B$ por el mismo factor $k$: $V$ y $\Delta$ se multiplican por $k$, pero los cocientes que definen $H$ y $S$ no cambian. Es una propiedad del modelo ideal; en una cámara real, la corrección gamma y el balance de blancos la cumplen solo de forma aproximada, por eso los rangos tienen cierto margen.

#### HSV en OpenCV con 8 bits

Un ángulo de 0° a 360° no cabe en un `uchar`, así que `cvtColor(..., COLOR_BGR2HSV)` guarda $H/2$ (de 0 a 179) y escala $S$ y $V$ a 0–255. Los rangos del código equivalen a:

| Salida | H en OpenCV | Tono real | S y V |
|---|---|---|---|
| Rojo | 0–10 y 160–179 | 0°–20° y 320°–358° | ≥ 100 |
| Verde | 35–85 | 70°–170° | ≥ 100 |
| Azul | 100–130 | 200°–260° | ≥ 100 |

- El rojo necesita dos rangos porque está justo donde la rueda de tonos se cierra (0° = 360°).
- $S \ge 100$ y $V \ge 100$ (un 39 % del máximo) descartan los grises, los blancos y las sombras, que es lo que [RGBFilter](../RGBFilter/) no podía hacer.
- Ejemplos: el amarillo puro da $H = 30$ y no entra en ningún rango; un rojo apagado como $(R, G, B) = (180, 60, 60)$ da $(H, S, V) = (0, 170, 180)$ y sí se clasifica como rojo.

#### Máscaras binarias

`inRange` comprueba, para cada píxel, si los tres canales están dentro de sus límites $[L_c, U_c]$:

```math
M(x, y) = \begin{cases} 255 & \text{si } L_c \le I_c(x, y) \le U_c \text{ para } c \in \{H, S, V\} \\ 0 & \text{en otro caso} \end{cases}
```

La unión de los dos rangos del rojo es un OR píxel a píxel (`maskRed1 | maskRed2`). `bitwise_and(image, image, salida, mascara)` copia la imagen original solo donde la máscara vale 255; como `salida` se crea en esa misma llamada, OpenCV la inicializa a cero y el resto queda en negro.

Una máscara de 1 byte por píxel es el resultado habitual de una segmentación. A partir de ella se puede contar el área del objeto (`countNonZero`), buscar sus contornos (`findContours`) o calcular su centro (`moments`), que es lo que hace un seguidor de objetos por color.

### Coste en la Raspberry Pi

| Matriz | Tamaño |
|---|---|
| `image` (BGR) | 1,08 MB |
| `hsv` | 1,08 MB |
| 5 máscaras de 1 canal (`maskRed1`, `maskRed2`, `maskRed`, `maskGreen`, `maskBlue`) | 5 × 360 KB = 1,8 MB |
| 3 salidas BGR (`red`, `green`, `blue`) | 3 × 1,08 MB = 3,24 MB |
| **Total** | **≈ 7,2 MB** |

- **Cómputo:** el paso caro es `cvtColor`, que por cada píxel calcula un máximo, un mínimo y dos divisiones. Para imágenes de 8 bits, OpenCV sustituye las divisiones por tablas precalculadas y procesa varios píxeles a la vez con NEON. `inRange`, `|` y `bitwise_and` son comparaciones y operaciones lógicas por bytes, también vectorizadas. A diferencia de [RGBFilter](../RGBFilter/), el código del ejemplo no tiene ninguna rama por píxel.
- **Memoria:** si solo interesa la máscara (para contar píxeles o buscar contornos), no hace falta crear las tres imágenes de salida, que son casi la mitad de la memoria del programa.

**Medido** en una Raspberry Pi 5 con OpenCV 4.9 (mediana de 3 ejecuciones, con las ventanas abiertas): `RSS máx` = 46.032 kB, 14,6 MB más que [LoadImage](../LoadImage/), que carga la misma imagen y abre una sola ventana. Las matrices añaden 6,1 MB; el resto viene de las tres ventanas adicionales.

## Descripción del Código

El programa ([`HSVFilter.cpp`](HSVFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `image.empty()`.
2. **Conversión:** `cvtColor(image, hsv, COLOR_BGR2HSV)`. En imágenes de 8 bits, OpenCV guarda el tono (H) en el rango 0–179 (grados divididos entre 2) y la saturación (S) y el valor (V) en 0–255.
3. **Máscaras:** `inRange` pone a 255 los píxeles dentro del rango y a 0 el resto. Todos los rangos exigen S ≥ 100 y V ≥ 100 para descartar grises y zonas oscuras:

   | Color | Rango de H | Máscara |
   |---|---|---|
   | Rojo | 0–10 y 160–179 | `maskRed1 \| maskRed2` |
   | Verde | 35–85 | `maskGreen` |
   | Azul | 100–130 | `maskBlue` |

4. **Aplicación:** `bitwise_and(image, image, red, maskRed)` copia la imagen original solo donde la máscara vale 255, así que cada salida conserva los colores reales.
5. **Visualización:** `imshow` de las cuatro imágenes y `waitKey(0)`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `cvtColor` | imgproc | Convierte de BGR a HSV |
| `inRange` | core | Genera la máscara binaria de cada color |
| operador `\|` sobre `Mat` | core | Une los dos rangos del rojo |
| `bitwise_and` | core | Aplica la máscara a la imagen original |
| `imshow` | highgui | Muestra la original y las tres salidas |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- Los rangos están fijos en el código; para ajustarlos a otra imagen o a otra iluminación hay que editar los `Scalar` y recompilar.
- Los tonos entre rangos (amarillos, cian, magentas) no aparecen en ninguna salida.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)
- [Changing Colorspaces](https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html)
- [Thresholding Operations using inRange](https://docs.opencv.org/4.x/da/d97/tutorial_threshold_inRange.html)
- [Color conversions (BGR ↔ HSV)](https://docs.opencv.org/4.x/de/d25/imgproc_color_conversions.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
