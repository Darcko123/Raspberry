# Raspberry Pi Zero 2W - Recursos y Ejemplos

Este repositorio proporciona una colección de recursos, ejemplos y tutoriales prácticos para trabajar con la Raspberry Pi Zero 2W. El objetivo es facilitar el desarrollo de proyectos básicos e intermedios utilizando C, C++ con OpenCV y HTML/CSS.

## 📚 **Descripción General**

Este repositorio está dividido en tres secciones principales:

- **WiringPi/**: Ejemplos de control de GPIOs utilizando la librería WiringPi en lenguaje C.
- **OpenCV/**: Programas de procesamiento de imágenes y visión por computadora utilizando OpenCV en C++, incluyendo un tutorial detallado para su instalación.
- **DiseñoWeb/**: Diseños básicos de páginas web listas para ser servidas mediante un servidor Apache.

---

## 📁 **Estructura del Repositorio**

```
Raspberry/
│
├── WiringPi/       #Ejemplos de control de GPIOs utilizando la librería WiringPi en lenguaje C.
│   ├── 1.Salidas/
│   │   ├── 1.1.1_BlinkingLed/
│   │   │   ├── README.md
│   │   │   ├── 1.1.1_BlinkingLed.c
│   │   │   ├── BlinkingLed
│   │   │
│   │   ├── 1.1_2_7Segments/
│   │   │   ├── README.md
│   │   │   ├── 1.1.2_7Segments.c
│   │   │   ├── 7Segments
│   │   │
│   │   ├── 1.1_7_LCD_I2C/
│   │   │   ├── README.md
│   │   │   ├── 1.1.7_LCD_I2C.c
│   │   │   ├── liquidcrystal_i2c.c
│   │   │   ├── liquidcrystal_i2c.h
│   │   │   ├── LCD
│   │   
│   ├── 2.Entradas/
│
├── OpenCV/        # Programas de procesamiento de imágenes y visión por computadora
│   ├── OpenCVDownloadTutorial.md
│   ├── Prueba/
│   ├── CargaDeImagen/
│   ├── FiltroRGB/
│   ├── FiltroHSV/
│   ├── FiltroLaplaciano/
│   ├── FiltroGaussiano/
│   ├── CMakeLists.txt
│   ├── build.sh
│
├── DiseñoWeb
│   ├── README.md
│   ├── AnalogClock/
│   ├── PresentationCard
│
├── LICENSE          # Licencia del proyecto
├── README.md        # Este archivo
└── .gitignore

```

---

## 🖥️Requerimientos de Hardware
- Raspberry Pi Zero 2W

- MicroSD (mínimo 16 GB Clase 10 recomendada)

- Fuente de alimentación de 5V/2A

- Conexión a Internet (Ethernet o Wi-Fi)

- Opcional: cámara compatible con la Raspberry Pi

---
## 🛠️ Requerimientos de Software
- Sistema operativo Raspberry Pi OS (recomendado Lite o Desktop)

- Librería WiringPi instalada

- Apache HTTP Server instalado

- OpenCV 4.x instalado

- Compilador GCC para C/C++

- Herramientas básicas: git, make, cmake

---

## 🚀 **Cómo Empezar**

1. **Clona el repositorio:**
   ```bash
   git clone https://github.com/Darcko123/Raspberry.git
   ```
2. **Navega a la carpeta deseada:**
   ```bash
   cd Raspberry/<ruta>/<subcarpeta>
   ```
3. **Abre el ejemplo correspondiente en tu entorno de desarrollo.**
4. **Compila y ejecuta el código**

---

---

## 📄 **Licencia**

Este proyecto está bajo la licencia **MIT**. Consulta el archivo [LICENSE](./LICENSE.md) para más detalles.

---

## 📞 **Contacto**

Si tienes alguna duda o sugerencia, no dudes en ponerte en contacto:
- **Autor:** Daniel Ruiz
- **Correo:** daniel18052002@yahoo.com


