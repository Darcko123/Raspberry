# Tutorial para instalación y uso de OpenCV en C++ para Raspberry
![OpenCV Logo](https://opencv.org/wp-content/uploads/2022/05/logo.png)

El siguiente documento describe el proceso para la descarga y compilación de OpenCV tanto para *Raspberry Pi 5* como para *Raspberry Pi Zero 2W*. Volver al [índice de ejemplos](README.md).

- [Tutorial para instalación y uso de OpenCV en C++ para Raspberry](#tutorial-para-instalación-y-uso-de-opencv-en-c-para-raspberry)
- [Opción rápida: OpenCV desde apt](#opción-rápida-opencv-desde-apt)
- [Instalación de OpenCV 4.9.0 en Raspberry Pi 5 (C++)](#instalación-de-opencv-490-en-raspberry-pi-5-c)
  - [Requisitos Previos](#requisitos-previos)
  - [1. Actualizar el Sistema y Dependencias](#1-actualizar-el-sistema-y-dependencias)
  - [2. Descargar OpenCV y OpenCV Contrib](#2-descargar-opencv-y-opencv-contrib)
  - [3. Preparar el Entorno de Compilación](#3-preparar-el-entorno-de-compilación)
  - [4. Configurar CMake](#4-configurar-cmake)
  - [5. Compilar e Instalar](#5-compilar-e-instalar)
- [Instalación de OpenCV 4.7.0 en Raspberry Pi Zero 2W (C++)](#instalación-de-opencv-470-en-raspberry-pi-zero-2w-c)
  - [Requisitos Previos](#requisitos-previos-1)
  - [1. Actualizar el Sistema y Dependencias](#1-actualizar-el-sistema-y-dependencias-1)
  - [2. Ampliar la Swap](#2-ampliar-la-swap)
  - [3. Descargar OpenCV y OpenCV Contrib](#3-descargar-opencv-y-opencv-contrib)
  - [4. Preparar el Entorno de Compilación](#4-preparar-el-entorno-de-compilación)
  - [5. Configurar CMake](#5-configurar-cmake)
  - [6. Compilar e Instalar](#6-compilar-e-instalar)
  - [7. Restaurar la Swap](#7-restaurar-la-swap)
- [Prueba de Verificación en C++](#prueba-de-verificación-en-c)

# Opción rápida: OpenCV desde apt
Los ejemplos de esta carpeta funcionan con cualquier OpenCV 4.x. Si no necesita los módulos de `opencv_contrib` ni la última versión, basta con el paquete de la distribución (OpenCV 4.6 en Raspberry Pi OS Bookworm), que se instala en minutos en lugar de horas:
```bash
sudo apt update
sudo apt install -y build-essential cmake libopencv-dev
```
Después, pase directamente a la [Prueba de Verificación](#prueba-de-verificación-en-c).

# Instalación de OpenCV 4.9.0 en Raspberry Pi 5 (C++)
Este tutorial describe paso a paso cómo instalar y compilar OpenCV 4.9.0 en una Raspberry Pi 5, así como ejecutar una prueba en C++ para verificar su correcta instalación.

## Requisitos Previos
- Raspberry Pi 5 con Raspberry Pi OS actualizado.

- Conexión a Internet.

- Acceso como superusuario (sudo).

## 1. Actualizar el Sistema y Dependencias
Ejecute los siguientes comandos para actualizar el sistema e instalar las dependencias necesarias para compilar OpenCV:
```bash
sudo apt update && sudo apt upgrade -y

sudo apt install -y build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb-dev libdc1394-dev libopenblas-dev libeigen3-dev
```

## 2. Descargar OpenCV y OpenCV Contrib
Nos moveremos al directorio /home y descargaremos OpenCV versión 4.9.0 junto con los módulos extra (contrib):
```bash
cd /home/

sudo wget -O opencv.zip https://github.com/opencv/opencv/archive/4.9.0.zip
sudo wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.9.0.zip

sudo unzip opencv.zip
sudo unzip opencv_contrib.zip
```

## 3. Preparar el Entorno de Compilación
```bash
cd /home/opencv-4.9.0/

sudo mkdir build

cd build
```

## 4. Configurar CMake
Ejecute el siguiente comando para configurar la compilación con CMake. Asegúrese de ajustar rutas si modificó el directorio de descarga:
```bash
sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=/home/opencv_contrib-4.9.0/modules \
    -D ENABLE_NEON=ON -D ENABLE_VFPV3=OFF -D CPU_BASELINE=NEON \
    -D BUILD_TESTS=OFF -D OPENCV_ENABLE_NONFREE=ON \
    -D BUILD_opencv_python3=ON -D PYTHON3_EXECUTABLE=/usr/bin/python3 \
    -D BUILD_EXAMPLES=OFF -D WITH_OPENMP=ON -D WITH_OPENGL=ON ..
```
Si todo está correcto, el sistema mostrará un mensaje como:
```
-- Configuring done
-- Generating done
-- Build files have been written to: /home/opencv-4.9.0/build
```

## 5. Compilar e Instalar
Compilar utilizando todos los núcleos disponibles:

```bash
sudo make -j$(nproc)
```
Instalar OpenCV:

```bash
sudo make install
sudo ldconfig
```

Continúe con la [Prueba de Verificación](#prueba-de-verificación-en-c).

# Instalación de OpenCV 4.7.0 en Raspberry Pi Zero 2W (C++)
Este tutorial describe paso a paso cómo instalar y compilar OpenCV 4.7.0 en una Raspberry Pi Zero 2W, así como ejecutar una prueba en C++ para verificar su correcta instalación.

## Requisitos Previos
- Raspberry Pi Zero 2W con Raspberry Pi OS actualizado.

- Conexión a Internet.

- Acceso como superusuario (sudo).

- Al menos 6 GB libres en la microSD (código fuente, compilación y swap).

## 1. Actualizar el Sistema y Dependencias
Ejecute los siguientes comandos para actualizar el sistema e instalar las dependencias necesarias para compilar OpenCV:
```bash
sudo apt update && sudo apt upgrade -y

sudo apt install -y build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb-dev libdc1394-dev libopenblas-dev libeigen3-dev
```

## 2. Ampliar la Swap
La Zero 2W solo tiene 512 MB de RAM y la compilación de OpenCV se queda sin memoria con la swap por defecto (entre 100 y 512 MB). Amplíela a 2 GB mientras dure la compilación:
```bash
sudo dphys-swapfile swapoff
sudo cp /etc/dphys-swapfile /etc/dphys-swapfile.bak
sudo sed -i 's/^#\?CONF_SWAPSIZE=.*/CONF_SWAPSIZE=2048/' /etc/dphys-swapfile
sudo dphys-swapfile setup
sudo dphys-swapfile swapon
free -h    # la fila "Swap" debe mostrar unos 2.0Gi
```

## 3. Descargar OpenCV y OpenCV Contrib
Nos moveremos al directorio /home y descargaremos OpenCV versión 4.7.0 junto con los módulos extra (contrib):
```bash
cd /home/

sudo wget -O opencv.zip https://github.com/opencv/opencv/archive/4.7.0.zip
sudo wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.7.0.zip

sudo unzip opencv.zip
sudo unzip opencv_contrib.zip
```

## 4. Preparar el Entorno de Compilación
```bash
cd /home/opencv-4.7.0/

sudo mkdir build

cd build
```

## 5. Configurar CMake
Ejecute el siguiente comando para configurar la compilación con CMake. Asegúrese de ajustar rutas si modificó el directorio de descarga:

```bash
sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=/home/opencv_contrib-4.7.0/modules \
    -D ENABLE_NEON=ON -D ENABLE_VFPV3=OFF -D CPU_BASELINE=NEON \
    -D BUILD_TESTS=OFF -D OPENCV_ENABLE_NONFREE=ON \
    -D BUILD_opencv_python3=ON -D PYTHON3_EXECUTABLE=/usr/bin/python3 \
    -D BUILD_EXAMPLES=OFF -D WITH_OPENMP=ON -D WITH_OPENGL=ON ..
```
Si todo está correcto, el sistema mostrará un mensaje como:
```
-- Configuring done
-- Generating done
-- Build files have been written to: /home/opencv-4.7.0/build
```

## 6. Compilar e Instalar
Compilar con un solo núcleo (con más, la RAM no alcanza aunque se haya ampliado la swap):

```bash
sudo make -j1
```

> [!NOTE]
> La conexión ssh se puede cerrar varias veces durante la compilación. Basta con volver a conectarse, regresar al directorio `/home/opencv-4.7.0/build` y volver a ejecutar el comando anterior: `make` continúa donde se quedó.

Instalar OpenCV:

```bash
sudo make install
sudo ldconfig
```

## 7. Restaurar la Swap
Una swap grande desgasta la microSD. Al terminar, vuelva a la configuración original:
```bash
sudo dphys-swapfile swapoff
sudo mv /etc/dphys-swapfile.bak /etc/dphys-swapfile
sudo dphys-swapfile setup
sudo dphys-swapfile swapon
```

# Prueba de Verificación en C++
El ejemplo [HelloWorld](HelloWorld/) solo imprime la versión de OpenCV, así que sirve para comprobar la instalación. Clone el repositorio (si aún no lo tiene) y ejecute su `build.sh`, que compila en `HelloWorld/build/` y lanza el programa:
```bash
git clone https://github.com/Darcko123/Raspberry.git
cd Raspberry/OpenCV/HelloWorld
./build.sh
```

Si la instalación fue exitosa, verá en consola algo como:
```
-- OpenCV version: 4.9.0
...
OpenCV version: 4.9.0
```
La primera línea la imprime CMake al configurar y la última el programa. Ambas deben mostrar la versión instalada (4.9.0, 4.7.0 o 4.6.0 si se instaló desde apt).
