<!--
  Plantilla de README para los ejemplos de OpenCV/.
  - Copie la carpeta _PLANTILLA/ completa y renombre la carpeta, el .cpp y el project() del
    CMakeLists.txt con el mismo nombre (regla carpeta = .cpp = target = ejecutable).
  - Conserve exactamente los encabezados ## y ### de este archivo y en el mismo orden:
    scripts/opencv-build-all.sh los compara con los de cada ejemplo.
  - Sustituya cada <marcador> y borre este comentario.
-->

# <Título del ejemplo>

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi-C51A4A.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-5C3EE8.svg)
![Language](https://img.shields.io/badge/C%2B%2B-11-00599C.svg)

## Tabla de Contenidos

- [\<Título del ejemplo\>](#título-del-ejemplo)
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
    - [Coste en la Raspberry Pi](#coste-en-la-raspberry-pi)
  - [Descripción del Código](#descripción-del-código)
  - [Funciones OpenCV utilizadas](#funciones-opencv-utilizadas)
  - [Limitaciones conocidas](#limitaciones-conocidas)
  - [Recursos de aprendizaje](#recursos-de-aprendizaje)
  - [Licencia](#licencia)

## Descripción

<Qué hace el programa y qué concepto de visión enseña, en uno o dos párrafos.>

## Características

- <Característica 1>
- <Característica 2>

## Requisitos

### Hardware

- Raspberry Pi 5 o Raspberry Pi Zero 2W (o cualquier equipo Linux).
- Pantalla o sesión gráfica para las ventanas de `imshow`.

### Software

- OpenCV 4.x con los módulos `<core, imgcodecs, highgui>` (vale el `libopencv-dev` 4.6 de apt; ver [INSTALACION.md](../INSTALACION.md)).
- CMake 3.12 o superior y un compilador de C++11.

## Compilación y Prueba

### Dependencias

```bash
sudo apt install libopencv-dev cmake g++ time
```

Si ya compiló OpenCV desde el código fuente con [INSTALACION.md](../INSTALACION.md), omita `libopencv-dev`. El paquete `time` instala `/usr/bin/time`, que se usa en la [Verificación](#verificación).

### Compilar

```bash
cd OpenCV/<Ejemplo>
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
./<Ejemplo>
```

El programa se ejecuta desde `build/` porque abre `avatares.png` con una ruta relativa, y CMake copia ahí [assets/avatares.png](../assets/avatares.png). Por SSH, conéctese con `ssh -X` o, si hay una sesión gráfica abierta en la Pi, use `DISPLAY=:0 ./<Ejemplo>`.

### Resultado esperado

<Ventanas que se abren (con su título exacto), qué se ve en cada una, teclas o controles y cómo termina.>

### Verificación

Desde `build/`:

```bash
/usr/bin/time -f 'RSS máx: %M kB | tiempo: %e s | salida: %x' ./<Ejemplo>
```

- <Comprobación del funcionamiento normal: salida esperada y `salida: 0`.>
- <Comprobación del error: qué imprime y con qué código sale si falta el recurso.>

`RSS máx` es la memoria pico del proceso. En los ejemplos con ventana, `tiempo` incluye el rato que la ventana queda abierta, así que solo sirve como cota.

## Teoría de Visión Embebida

### Problema que resuelve

<Qué problema de visión resuelve el ejemplo y dónde aparece en un sistema embebido.>

### Fundamentos

<Base matemática del algoritmo: fórmulas, *kernels* y por qué funciona. Divídala con encabezados `####`.>

### Coste en la Raspberry Pi

<Memoria de cada matriz, operaciones por píxel y consumo. Marque como estimación lo que no se haya medido.>

## Descripción del Código

<Recorrido por el código en el orden en que se ejecuta, con fragmentos breves.>

## Funciones OpenCV utilizadas

| Función | Módulo | Uso en este ejemplo |
|---|---|---|
| `imread` | imgcodecs | Carga `avatares.png` |
| `imshow` | highgui | Muestra una imagen en una ventana |
| `waitKey` | highgui | Espera una tecla y procesa los eventos de las ventanas |

## Limitaciones conocidas

- <Comportamiento conocido que no se corrige en este ejemplo.>

## Recursos de aprendizaje

- [Documentación oficial de OpenCV](https://docs.opencv.org/4.x/)

## Licencia

Este proyecto está bajo la licencia **MIT**. Consulte el archivo [LICENSE](../../LICENSE.md) para más detalles.
