# Control de Pantalla LCD via I2C con Raspberry Pi usando WiringPi

## Descripción

Esta librería facilita el control de pantallas LCD compatibles con el controlador HD44780 usando comunicación I2C en una Raspberry Pi.
Permite inicializar, escribir texto, controlar la retroiluminación, desplazar el contenido y crear caracteres personalizados.

La librería está escrita en C utilizando WiringPi, una biblioteca popular para la manipulación de GPIO y periféricos en Raspberry Pi.

## Características

- Inicialización de LCDs de 1, 2 o 4 filas.

- Escritura de texto y caracteres individuales.

- Control de cursor y parpadeo.

- Desplazamiento del contenido del display.

- Cambio de dirección de escritura (izquierda/derecha).

- Encendido y apagado de retroiluminación.

- Creación e impresión de caracteres personalizados.

## Requisitos

- Raspberry Pi con WiringPi instalado.
  
- Módulo adaptador I2C para LCD basado en PCF8574.

## Instalación
1. Copia los archivos liquidcrystal_i2c.h y liquidcrystal_i2c.c a tu proyecto.
2. Incluye el archivo de cabecera en tu programa:
``` c++
    #include "liquidcrystal_i2c.h
```
3. Asegurate de linkear contra WiringPI al compilar:
   ``` bash
    gcc -o lcd_example main.c liquidcrystal_i2c.c -lwiringPi
   ``` 
# Ejemplo
``` c++
#include "liquidcrystal_i2c.h"

int main()
{
    int fd = LCD_Init(0x27, 2, LCD_5x8DOTS);
    if (fd == -1)
        return -1;

    LCD_Clear(fd);
    LCD_PrintStr(fd, "Hola Mundo!");
    LCD_SetCursor(fd, 0, 1);
    LCD_PrintStr(fd, "LCD I2C!");

    sleep(2);
    LCD_ScrollDisplayLeft(fd);

    LCD_Close(fd);
    return 0;
}
```

## Funciones principales
- LCD_Init(addr, rows, dotsize): Inicializa el LCD.

- LCD_Clear(fd): Limpia el display.

- LCD_SetCursor(fd, col, row): Posiciona el cursor.

- LCD_PrintStr(fd, str): Imprime una cadena.

- LCD_Backlight(fd) / LCD_NoBacklight(fd): Controla la retroiluminación.

- LCD_ScrollDisplayLeft(fd) / LCD_ScrollDisplayRight(fd): Desplaza el contenido.

- LCD_CreateChar(fd, location, charmap[]): Crea un carácter personalizado.

- LCD_PrintCustomChar(fd, index): Imprime un carácter creado.~~
