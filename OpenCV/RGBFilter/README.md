# Separación por color dominante (RGB) con OpenCV

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [Separación por color dominante (RGB) con OpenCV](#separación-por-color-dominante-rgb-con-opencv)
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
      - [El modelo RGB](#el-modelo-rgb)
      - [Clasificación por canal dominante](#clasificación-por-canal-dominante)
      - [Por qué RGB separa mal los colores](#por-qué-rgb-separa-mal-los-colores)
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

Recorre la imagen píxel a píxel y clasifica cada uno según su componente de color dominante (rojo, verde o azul). Genera tres imágenes, una por color, en las que solo aparecen los píxeles donde ese canal es estrictamente mayor que los otros dos. Enseña cómo se guarda una imagen en memoria (orden BGR) y cómo se accede a un píxel concreto.

## Características

- Acceso directo a los píxeles con `Mat::at<Vec3b>(y, x)`.
- Clasificación por componente dominante en el espacio BGR.
- Tres imágenes de salida, una por canal, inicializadas en negro con `Mat::zeros`.
- Visualización de la imagen original y de las tres salidas.

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
cd OpenCV/RGBFilter
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
./RGBFilter
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./RGBFilter`.

### Resultado esperado

Se abren cuatro ventanas:

1. **Imagen original**: la imagen cargada.
2. **Canal Rojo**: solo los píxeles con el rojo dominante, con su valor de rojo; el resto, en negro.
3. **Canal Verde**: lo mismo para el verde.
4. **Canal Azul**: lo mismo para el azul.

Los píxeles grises, blancos o negros (sin un canal claramente mayor) no aparecen en ninguna salida. Al pulsar cualquier tecla con una ventana seleccionada, el programa termina.

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./RGBFilter
```

- Tras cerrar con una tecla debe mostrar `salida: 0`.
- Si se ejecuta fuera de `build/`, debe imprimir `Error: No se pudo abrir o encontrar la imagen.` y salir con `255` (el código devuelve `-1`).

`RSS máx` es la memoria pico del proceso. `tiempo` incluye el rato que las ventanas quedan abiertas, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

Separar los píxeles de una imagen según su color dominante. Es la forma más simple de **segmentación por color**, que en visión embebida se usa para seguir objetos de un color conocido (una pelota, una marca, una línea en el suelo) sin necesidad de un detector costoso.

### Fundamentos

#### El modelo RGB

RGB es un modelo **aditivo**: cada color se forma sumando luz roja, verde y azul, igual que los subpíxeles de una pantalla o los filtros de Bayer que hay sobre el sensor de la cámara. Con 8 bits por canal, cada píxel es un punto del cubo $[0, 255]^3$. El negro está en el origen, el blanco en $(255, 255, 255)$ y todos los grises sobre la diagonal $R = G = B$.

#### Clasificación por canal dominante

El programa asigna cada píxel al canal cuyo valor es **estrictamente** mayor que los otros dos:

```math
\text{clase}(x, y) = \arg\max_{c \,\in\, \{R, G, B\}} I_c(x, y)
```

Geométricamente, los planos $R = G$, $R = B$ y $G = B$ dividen el cubo en tres regiones, una por canal, que se tocan en la diagonal de los grises. Los píxeles que caen **sobre** uno de esos planos, es decir, los que empatan en el máximo, quedan en negro en las tres salidas. Además de los grises, esto incluye colores puros como el amarillo $(R, G, B) = (255, 255, 0)$, el cian y el magenta.

#### Por qué RGB separa mal los colores

En RGB, el brillo y el color están repartidos entre los tres canales: al oscurecer un objeto rojo bajan a la vez sus tres componentes. Una regla que solo compara canales no puede separar esas dos magnitudes:

- **No mira la saturación**: un gris casi perfecto como $(R, G, B) = (101, 100, 100)$ se clasifica como rojo.
- **No mira la intensidad**: un píxel casi negro como $(3, 1, 1)$ también es rojo, así que el ruido del sensor en las sombras se reparte entre las tres salidas.

En `avatares.png`, la regla asigna el 54 % de los píxeles al azul (194.444 de 360.000), el 33 % al rojo y el 11 % al verde; solo empata el 1,8 %. Con la misma imagen, [HSVFilter](../HSVFilter/), que exige una saturación y un brillo mínimos, se queda con 56.428 píxeles azules, 15.481 rojos y 342 verdes.

Sin salir de RGB, la mejora clásica es la **cromaticidad normalizada**, que divide cada canal entre la suma de los tres y elimina así la intensidad:

```math
r = \frac{R}{R + G + B}, \qquad g = \frac{G}{R + G + B}, \qquad b = 1 - r - g
```

La solución habitual es cambiar a un espacio de color que separe el tono del brillo, como hace [HSVFilter](../HSVFilter/).

### Coste en la Raspberry Pi

- **Memoria:** la original y las tres salidas, del mismo tamaño y tipo: 4 × 1,08 MB = 4,32 MB. `Mat::zeros` además escribe ceros en las tres salidas (3,24 MB) antes del bucle, y la rama `else` vuelve a escribir ceros donde ya los había.
- **Cómputo:** 360.000 píxeles, con hasta 6 comparaciones y una escritura por píxel. El número de operaciones es pequeño; lo que limita la velocidad es la forma del bucle. Cada píxel toma un camino del `if` que depende de sus datos, y el compilador no puede convertir esas ramas en instrucciones vectoriales. Las funciones de OpenCV, en cambio, usan NEON, cuyos registros de 128 bits procesan 16 bytes por instrucción. La misma clasificación sin ramas, sobre la matriz completa, sería:

  ```cpp
  std::vector<Mat> bgr;
  split(image, bgr);                                    // 3 matrices de 1 canal
  Mat esRojo = (bgr[2] > bgr[1]) & (bgr[2] > bgr[0]);   // 255 donde domina R
  ```

## Descripción del Código

El programa ([`RGBFilter.cpp`](RGBFilter.cpp)) sigue estos pasos:

1. **Carga:** `imread("avatares.png", IMREAD_COLOR)` y comprobación con `image.empty()`.
2. **Imágenes de salida:** `Mat::zeros(image.size(), image.type())` crea `rojo`, `verde` y `azul` del mismo tamaño y tipo (`CV_8UC3`) que la original, todas en negro.
3. **Recorrido:** dos bucles anidados (filas `y`, columnas `x`) leen cada píxel como `Vec3b`. OpenCV guarda los canales en orden **BGR**, así que `pixel[0]` es el azul y `pixel[2]` el rojo:
   ```cpp
   Vec3b pixel = image.at<Vec3b>(y, x);
   uchar b = pixel[0], g = pixel[1], r = pixel[2];
   ```
4. **Clasificación:** si un canal es estrictamente mayor que los otros dos, se escribe solo ese canal en su imagen de salida (por ejemplo, `Vec3b(0, 0, r)` en `rojo`). Si hay empate, el píxel queda en negro en las tres.
5. **Visualización:** `imshow` de las cuatro imágenes y `waitKey(0)`.

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` a color (BGR) |
| `Mat::zeros` | core | Crea las tres imágenes de salida en negro |
| `Mat::at<Vec3b>` | core | Lee y escribe un píxel de 3 canales |
| `imshow` | highgui | Muestra la original y las tres salidas |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- El recorrido píxel a píxel con `at<>` y ramas `if` no aprovecha las instrucciones vectoriales (NEON) de la Raspberry Pi. Es didáctico, pero bastante más lento que las operaciones de OpenCV sobre la matriz completa (`split`, `compare`, `max`).
- La regla de "canal estrictamente mayor" clasifica como rojo un píxel casi gris como `(100, 100, 101)`. No hay umbral de saturación; el ejemplo [HSVFilter](../HSVFilter/) resuelve esto trabajando en HSV.
- En caso de error devuelve `-1` (el sistema lo ve como `255`) en lugar de `EXIT_FAILURE`.

## Recursos de aprendizaje

- [Documentación oficial de OpenCV - Función imread](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html)
- [Documentación oficial de OpenCV - Función imshow](https://docs.opencv.org/4.x/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563)
- [Documentación oficial de OpenCV - Clase Mat](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)
- [Documentación oficial de OpenCV - Vec3b](https://docs.opencv.org/4.x/d6/dcf/classcv_1_1Vec.html#a72280b48a5d716939114289dc0930ba6)
- [How to scan images, lookup tables and time measurement](https://docs.opencv.org/4.x/db/da5/tutorial_how_to_scan_images.html)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
