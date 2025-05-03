# Tutorial para instalación y uso de OpenCV en C++ para Raspberry
El siguiente documento describe el proceso para la desacarga y compilación de OpenCV tanto para *Raspberry Pi 5* cómo para *Raspberry Zero 2W*

- [Tutorial para instalación y uso de OpenCV en C++ para Raspberry](#tutorial-para-instalación-y-uso-de-opencv-en-c-para-raspberry)
- [📸 Instalación de OpenCV 4.9.0 en Raspberry Pi 5 (C++)](#-instalación-de-opencv-490-en-raspberry-pi-5-c)
  - [🔧 Requisitos Previos](#-requisitos-previos)
  - [1️⃣ Actualizar el Sistema y Dependencias](#1️⃣-actualizar-el-sistema-y-dependencias)
  - [2️⃣ Descargar OpenCV y OpenCV Contrib](#2️⃣-descargar-opencv-y-opencv-contrib)
  - [3️⃣ Preparar el Entorno de Compilación](#3️⃣-preparar-el-entorno-de-compilación)
  - [4️⃣ Configurar CMake](#4️⃣-configurar-cmake)
  - [5️⃣ Compilar e Instalar](#5️⃣-compilar-e-instalar)
  - [✅ Prueba de Verificación en C++](#-prueba-de-verificación-en-c)
  - [🧱 Crear Archivo CMakeLists.txtCMakeLists.txt](#-crear-archivo-cmakeliststxtcmakeliststxt)
  - [⚙️ Compilar y Ejecutar](#️-compilar-y-ejecutar)
- [📸 Instalación de OpenCV 4.5.0 en Raspberry Zero 2W (C++)](#-instalación-de-opencv-450-en-raspberry-zero-2w-c)
  - [🔧 Requisitos Previos](#-requisitos-previos-1)
  - [1️⃣ Actualizar el Sistema y Dependencias](#1️⃣-actualizar-el-sistema-y-dependencias-1)
  - [2️⃣ Descargar OpenCV y OpenCV Contrib](#2️⃣-descargar-opencv-y-opencv-contrib-1)
  - [3️⃣ Preparar el Entorno de Compilación](#3️⃣-preparar-el-entorno-de-compilación-1)
  - [4️⃣ Configurar CMake](#4️⃣-configurar-cmake-1)
  - [5️⃣ Compilar e Instalar](#5️⃣-compilar-e-instalar-1)
  - [✅ Prueba de Verificación en C++](#-prueba-de-verificación-en-c-1)
  - [🧱 Crear Archivo CMakeLists.txt](#-crear-archivo-cmakeliststxt)
  - [🧱 Crear Archivo build.sh](#-crear-archivo-buildsh)
  - [⚙️ Compilar y Ejecutar](#️-compilar-y-ejecutar-1)

# 📸 Instalación de OpenCV 4.9.0 en Raspberry Pi 5 (C++)
Este tutorial describe paso a paso cómo instalar y compilar OpenCV 4.9.0 en una Raspberry Pi 5, así como ejecutar una prueba en C++ para verificar su correcta instalación.

## 🔧 Requisitos Previos
- Raspberry Pi 5 con Raspberry Pi OS actualizado.

- Conexión a Internet.

- Acceso como superusuario (sudo).

## 1️⃣ Actualizar el Sistema y Dependencias
Ejecute los siguientes comandos para actualizar el sistema e instalar las dependencias necesarias para compilar OpenCV:
``` bash
sudo apt update && sudo apt upgrade -y 

sudo apt install -y build-essential cmake git pkg-config libgtk-3-dev \ libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \ libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \ gfortran openexr libatlas-base-dev python3-dev python3-numpy \ libtbb-dev libdc1394-dev libopenblas-dev libeigen3-dev –y 
```
 

## 2️⃣ Descargar OpenCV y OpenCV Contrib
Nos moveremos al directorio /home y descargaremos OpenCV versión 4.9.0 junto con los módulos extra (contrib):
```bash
cd /home/

sudo wget -O opencv.zip https://github.com/opencv/opencv/archive/4.9.0.zip
sudo wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.9.0.zip

sudo unzip opencv.zip
sudo unzip opencv_contrib.zip
``` 

## 3️⃣ Preparar el Entorno de Compilación
```bash
cd /home/opencv-4.9.0/ 

sudo mkdir build 

cd build 
```
 

## 4️⃣ Configurar CMake
Ejecute el siguiente comando para configurar la compilación con CMake. Asegúrese de ajustar rutas si modificó el directorio de descarga:
```bash
sudo  cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/opencv_contrib-4.9.0/modules -D ENABLE_NEON=ON -D ENABLE_VFPV3=OFF -D CPU_BASELINE=NEON -D BUILD_TESTS=OFF -D OPENCV_ENABLE_NONFREE=ON -D BUILD_opencv_python3=ON -D PYTHON3_EXECUTABLE=/usr/bin/python3 -D BUILD_EXAMPLES=OFF -D WITH_OPENMP=ON -D WITH_OPENGL=ON .. 
```
Si todo está correcto, el sistema mostrará un mensaje como:
```bash
-- Configuring done -- Generating done -- Build files have been written to: /home/opencv-4.9.0/build 
```

## 5️⃣ Compilar e Instalar
Compilar utilizando todos los núcleos disponibles:

``` bash
sudo make –j$(nproc) 
```
Instalar OpenCV:

```bash
sudo make install 
```
 
## ✅ Prueba de Verificación en C++
Cree un archivo de prueba llamado Prueba.cpp en un directorio diferente:
```bash
nano Prueba.cpp 
```
Agregue el siguiente código:

```c++
#include <opencv2/core.hpp> 
#include <iostream> 

using namespace std; 
using namespace cv; 

int main() 
{ 
    cout << "OpenCV version: " << CV_VERSION << endl;
    return 0; 
} 
```

## 🧱 Crear Archivo CMakeLists.txtCMakeLists.txt 
En el mismo directorio, cree el archivo CMakeLists.txt:
```bash
nano CMakeLists.txt 
```
Agregue lo siguiente:
``` txt
cmake_minimum_required(VERSION 3.12) project(Prueba LANGUAGES CXX) 

--- Configuración para OpenCV --- 

find_package(OpenCV 4.9.0 REQUIRED COMPONENTS core highgui imgproc) message(STATUS "OpenCV version: ${OpenCV_VERSION}") 

--- Configuración para WiringPi --- 

find_library(WIRINGPI_LIB wiringPi) # Busca la librería WiringPi if(NOT WIRINGPI_LIB) message(FATAL_ERROR "WiringPi not found! Install with: sudo apt install wiringpi") else() message(STATUS "WiringPi library found: ${WIRINGPI_LIB}") endif() 

--- Configura el ejecutable --- 

add_executable(Prueba Prueba.cpp) 

--- Incluye directorios y enlaza librerías --- 

target_include_directories(Prueba PRIVATE ${OpenCV_INCLUDE_DIRS}) target_link_libraries(Prueba PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB}) 

--- Habilita C++11 --- 

target_compile_features(Prueba PRIVATE cxx_std_11) 
```

## ⚙️ Compilar y Ejecutar
Compile y ejecute la prueba:
``` bash
cmake . 
make 
./Prueba 
```
Si la instalación fue exitosa, verá en consola algo como:
``` yaml
OpenCV version: 4.9.0
```

# 📸 Instalación de OpenCV 4.5.0 en Raspberry Zero 2W (C++)
Este tutorial describe paso a paso cómo instalar y compilar OpenCV 4.7.0 en una Raspberry Zero 2W, así como ejecutar una prueba en C++ para verificar su correcta instalación.

## 🔧 Requisitos Previos
- Raspberry Pi con Raspberry Pi OS actualizado.

- Conexión a Internet.

- Acceso como superusuario (sudo).

## 1️⃣ Actualizar el Sistema y Dependencias
Ejecute los siguientes comandos para actualizar el sistema e instalar las dependencias necesarias para compilar OpenCV:
``` bash
sudo apt update && sudo apt upgrade -y 

sudo apt install -y build-essential cmake git pkg-config libgtk-3-dev \ libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \ libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \ gfortran openexr libatlas-base-dev python3-dev python3-numpy \ libtbb-dev libdc1394-dev libopenblas-dev libeigen3-dev –y 
```
## 2️⃣ Descargar OpenCV y OpenCV Contrib
Nos moveremos al directorio /home y descargaremos OpenCV versión 4.7.0 junto con los módulos extra (contrib):
```bash
cd /home/

sudo wget -O opencv.zip https://github.com/opencv/opencv/archive/4.7.0.zip
sudo wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.7.0.zip

sudo unzip opencv.zip
sudo unzip opencv_contrib.zip
``` 

## 3️⃣ Preparar el Entorno de Compilación
```bash
cd /home/opencv-4.7.0/ 

sudo mkdir build 

cd build 
```

## 4️⃣ Configurar CMake
Ejecute el siguiente comando para configurar la compilación con CMake. Asegúrese de ajustar rutas si modificó el directorio de descarga:

```bash
sudo  cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/opencv_contrib-4.9.0/modules -D ENABLE_NEON=ON -D ENABLE_VFPV3=OFF -D CPU_BASELINE=NEON -D BUILD_TESTS=OFF -D OPENCV_ENABLE_NONFREE=ON -D BUILD_opencv_python3=ON -D PYTHON3_EXECUTABLE=/usr/bin/python3 -D BUILD_EXAMPLES=OFF -D WITH_OPENMP=ON -D WITH_OPENGL=ON .. 
```
Si todo está correcto, el sistema mostrará un mensaje como:
```bash
-- Configuring done -- Generating done -- Build files have been written to: /home/opencv-4.7.0/build 
```

## 5️⃣ Compilar e Instalar
Compilar utilizando todos los núcleos disponibles:

``` bash
sudo make –j1
```

> [!NOTE]
> NOTA: La conexión ssh se puede cerrar múltiples veces, solo hay que desconectar el dispositivos, volver a conectar, reanudar la conexión, regresar al directorio "/home/OpenCV-4.5.0/build" y volver a ejecutar el comando anterior.

Instalar OpenCV:

```bash
sudo make install 
```
 
## ✅ Prueba de Verificación en C++
Cree un archivo de prueba llamado Prueba.cpp en un directorio diferente:
```bash
nano Prueba.cpp 
```
Agregue el siguiente código:

```c++
#include <opencv2/core.hpp> 
#include <iostream> 

using namespace std; 
using namespace cv; 

int main() 
{ 
    cout << "OpenCV version: " << CV_VERSION << endl;
    return 0; 
} 
```

## 🧱 Crear Archivo CMakeLists.txt
En el mismo directorio, cree el archivo CMakeLists.txt:
```bash
nano CMakeLists.txt 
```
Agregue lo siguiente:

```txt
cmake_minimum_required(VERSION 3.12)
project(Filtro LANGUAGES CXX)

# Establecer flags del compilador (puede ser útil en la Raspberry Pi Zero 2W)
set(CMAKE_CXX_FLAGS "" CACHE STRING "Flags for C++ compiler" FORCE)

# Configuración de OpenCV
find_package(OpenCV 4.7.0 REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# Buscar wiringPi
find_library(WIRINGPI_LIB wiringPi)
if(WIRINGPI_LIB)
    message(STATUS "Found wiringPi library: ${WIRINGPI_LIB}")
else()
    message(WARNING "wiringPi library not found. Proceeding without it.")
endif()

# Crear ejecutable
add_executable(Filtro Filtro.cpp)
target_include_directories(Filtro PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Filtro PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
```

## 🧱 Crear Archivo build.sh
En el mismo directorio, cree el archivo build.sh:
```bash
nano build.sh
```
Agregue lo siguiente:

```sh
#!/bin/bash
rm -rf CMakeFiles CMakeCache.txt
cmake -DCMAKE_CXX_FLAGS="" .
make
./Prueba
```

Ejecutar el comando:

```bash
chmod +x build.sh
```

## ⚙️ Compilar y Ejecutar

Ejecutar:

```bash
./build
```

Si la instalación fue exitosa, verá en consola algo como:
``` yaml
OpenCV version: 4.7.0
```