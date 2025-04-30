/*!
 * @file liquidcrystal_i2c.h
 * @brief Implementación de una librería para el control de pantallas LCD usando I2C en Raspberry Pi con WiringPi.
 *
 * Esta librería permite controlar una pantalla LCD basada en el controlador HD44780 usando la comunicación I2C
 * y proporciona funciones para inicializar, escribir y manipular la pantalla de manera sencilla.
 * @author Daniel Ruiz
 * @date 26 de abril de 2025
 * @version 1.0
 */
#ifndef LIQUIDCRYSTAL_I2C_H_
#define LIQUIDCRYSTAL_I2C_H_

#include <wiringPi.h>
#include <wiringPiI2C.h>

/* Command */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* Entry Mode */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Display On/Off */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/* Cursor Shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/* Function Set */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/* Backlight */
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* Enable Bit */
#define ENABLE 0x04

/* Register Select Bit */
#define RS 0x01

// Helper definiciones
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/**
 * @brief Inicializa la pantalla LCD.
 * @param addr Dirección I2C del dispositivo LCD.
 * @param rows Número de filas de la pantalla.
 * @param dotsize Tamaño de punto de los caracteres.
 * @return Descriptor de archivo o -1 en caso de error.
 */
int LCD_Init(int addr, uint8_t rows, uint8_t dotsize);

/**
 * @brief Cierra la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Close(int fd);


/**
 * @brief Limpia la pantalla.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Clear(int fd);

/**
 * @brief Mueve el cursor a la posición inicial.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Home(int fd);

/**
 * @brief Posiciona el cursor en una coordenada específica.
 * @param fd Descriptor de archivo I2C.
 * @param col Columna destino.
 * @param row Fila destino.
 */
void LCD_SetCursor(int fd, uint8_t col, uint8_t row);

/**
 * @brief Apaga la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoDisplay(int fd);

/**
 * @brief Enciende la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Display(int fd);

/**
 * @brief Apaga el cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoCursor(int fd);

/**
 * @brief Enciende el cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Cursor(int fd);

/**
 * @brief Apaga el parpadeo del cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoBlink(int fd);

/**
 * @brief Enciende el parpadeo del cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Blink(int fd);

/**
 * @brief Desplaza el contenido del display hacia la izquierda.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_ScrollDisplayLeft(int fd);

/**
 * @brief Desplaza el contenido del display hacia la derecha.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_ScrollDisplayRight(int fd);

/**
 * @brief Configura la escritura de izquierda a derecha.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_LeftToRight(int fd);

/**
 * @brief Configura la escritura de derecha a izquierda.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_RightToLeft(int fd);

/**
 * @brief Apaga la retroiluminación del LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoBacklight(int fd);

/**
 * @brief Enciende la retroiluminación del LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Backlight(int fd);

/**
 * @brief Ajusta el estado de la retroiluminación.
 * @param fd Descriptor de archivo I2C.
 * @param new_val Valor para encender (1) o apagar (0) la retroiluminación.
 */
void LCD_SetBacklight(int fd, uint8_t new_val);

/**
 * @brief Imprime un carácter en la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param c Carácter a imprimir.
 */
void LCD_PrintChar(int fd, char c);

/**
 * @brief Imprime una cadena de caracteres en la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param c Cadena de caracteres a imprimir.
 */
void LCD_PrintStr(int fd, const char c[]);

/**
 * @brief Crea un carácter personalizado.
 * @param fd Descriptor de archivo I2C.
 * @param location Ubicación de la memoria CGRAM.
 * @param charmap Mapa de bits del nuevo carácter.
 */
void LCD_CreateChar(int fd, uint8_t location, uint8_t charmap[]);

/**
 * @brief Imprime un carácter personalizado en la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param index Índice del carácter personalizado a imprimir.
 */
void LCD_PrintCustomChar(int fd, uint8_t index);

#endif /* LIQUIDCRYSTAL_I2C_H_ */