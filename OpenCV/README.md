# Ejemplos de OpenCV en C++ para Raspberry Pi

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

Colección de ejemplos de procesamiento de imágenes y visión por computadora con OpenCV en C++, pensados para la Raspberry Pi 5 y la Raspberry Pi Zero 2W. Cada carpeta es un programa independiente que se compila con CMake.

## Tabla de Contenidos

- [Ejemplos de OpenCV en C++ para Raspberry Pi](#ejemplos-de-opencv-en-c-para-raspberry-pi)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Ejemplos](#ejemplos)
  - [Estructura](#estructura)
  - [Requisitos](#requisitos)
  - [Cómo empezar](#cómo-empezar)
  - [Licencia](#licencia)

## Ejemplos

En orden de aprendizaje: cada ejemplo usa conceptos de los anteriores.

| # | Ejemplo | Qué enseña | Entrada |
|---|---|---|---|
| 1 | [HelloWorld](HelloWorld/) | Comprobar que OpenCV está instalado: imprime su versión | — |
| 2 | [LoadImage](LoadImage/) | Cargar una imagen con `imread` y mostrarla con `imshow` | Imagen |
| 3 | [RGBFilter](RGBFilter/) | Recorrer los píxeles y separar los canales de color | Imagen |
| 4 | [HSVFilter](HSVFilter/) | Pasar a HSV y segmentar por color con `inRange` y máscaras | Imagen |
| 5 | [HistogramBrightnessContrast](HistogramBrightnessContrast/) | Ajustar brillo y contraste con *trackbars* y ver el histograma | Imagen |
| 6 | [GaussianFilter](GaussianFilter/) | Suavizar la imagen con un filtro Gaussiano | Imagen |
| 7 | [LaplacianFilter](LaplacianFilter/) | Detectar bordes con la segunda derivada (Laplaciano) | Imagen |
| 8 | [CannyFilter](CannyFilter/) | Detectar bordes con Canny y ajustar sus umbrales en vivo | Imagen |
| 9 | [RegionOfInterest](RegionOfInterest/) | Seleccionar y recortar regiones de interés (ROI) | Imagen |
| 10 | [FaceDetect](FaceDetect/) | Detectar rostros con un clasificador Haar en cascada | Cámara |

Los ejemplos con imagen usan [assets/avatares.png](assets/avatares.png), que CMake copia a la carpeta `build/` de cada ejemplo.

## Estructura

Cada ejemplo sigue la regla **carpeta = `.cpp` = target de CMake = ejecutable**:

```
OpenCV/
├── README.md                  # Este índice
├── INSTALACION.md             # Tutorial de instalación de OpenCV
├── assets/
│   └── avatares.png           # Imagen compartida por los ejemplos
├── _PLANTILLA/                # Molde para crear un ejemplo nuevo
├── HelloWorld/
│   ├── README.md              # Compilación, prueba y explicación del ejemplo
│   ├── HelloWorld.cpp
│   ├── CMakeLists.txt
│   └── build.sh               # Compila en build/ y ejecuta
├── LoadImage/
├── RGBFilter/
├── HSVFilter/
├── HistogramBrightnessContrast/
├── GaussianFilter/
├── LaplacianFilter/
├── CannyFilter/
├── RegionOfInterest/
└── FaceDetect/
    ├── README.md
    ├── FaceDetect.cpp
    ├── CMakeLists.txt
    ├── build.sh
    └── haarcascade_frontalface_alt.xml
```

Todos los READMEs de los ejemplos tienen las mismas secciones, en el mismo orden que [`_PLANTILLA/README.md`](_PLANTILLA/README.md). Para crear un ejemplo nuevo, copie [`_PLANTILLA/`](_PLANTILLA/) y renombre la carpeta, el `.cpp` y el `project()` del `CMakeLists.txt` con el mismo nombre. El código de todos los ejemplos sigue las convenciones que lista la cabecera de [`_PLANTILLA.cpp`](_PLANTILLA/_PLANTILLA.cpp) y se compila con `-Wall -Wextra`.

[`scripts/opencv-build-all.sh`](../scripts/opencv-build-all.sh) compila todos los ejemplos (y la plantilla) y comprueba la regla de nombres, los encabezados de los READMEs y los enlaces relativos; sale con 1 si algo falla.

## Requisitos

- **Hardware:** Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux), con pantalla o sesión gráfica para las ventanas de `imshow`. `FaceDetect` necesita además una cámara.
- **Software:** OpenCV 4.x (vale el `libopencv-dev` 4.6 de apt), CMake 3.12 o superior y un compilador de C++11. Ver [INSTALACION.md](INSTALACION.md).

## Cómo empezar

1. Instale OpenCV siguiendo [INSTALACION.md](INSTALACION.md).
2. Clone el repositorio y entre en un ejemplo:
   ```bash
   git clone https://github.com/Darcko123/Raspberry.git
   cd Raspberry/OpenCV/HelloWorld
   ```
3. Compile y ejecute:
   ```bash
   ./build.sh
   ```
   El ejecutable queda en `build/` y se puede volver a lanzar con `cd build && ./HelloWorld`.

> [!NOTE]
> Por SSH, las ventanas de OpenCV necesitan un display: conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, ejecute con `DISPLAY=:0 ./build.sh`.

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../LICENSE.md) para más detalles.
