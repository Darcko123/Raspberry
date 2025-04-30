/*!
 * @file liquidcrystal_i2c.c
 * @brief Implementación de una librería para el control de pantallas LCD usando I2C en Raspberry Pi con WiringPi.
 *
 * Esta librería permite controlar una pantalla LCD basada en el controlador HD44780 usando la comunicación I2C
 * y proporciona funciones para inicializar, escribir y manipular la pantalla de manera sencilla.
 * @author Daniel Ruiz
 * @date 26 de abril de 2025
 * @version 1.0
 */

 #include "liquidcrystal_i2c.h"
 #include <unistd.h>
 #include <string.h>


// Variables globales privadas al módulo
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _backlightval;
static uint8_t _numlines;


// Prototipos de funciones privadas
static void write4bits(int fd, uint8_t value);
static void expanderWrite(int fd, uint8_t data);
static void pulseEnable(int fd, uint8_t data);
static void send(int fd, uint8_t value, uint8_t mode);
static void sendCommand(int fd, uint8_t cmd);
static void sendData(int fd, uint8_t data);

/**
 * @brief Envía 8 bits divididos en dos transferencias de 4 bits.
 * @param fd Descriptor de archivo I2C.
 * @param value Valor a enviar.
 * @param mode Modo de envío (comando o datos).
 */
static void send(int fd, uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    write4bits(fd, highnib | mode);
    write4bits(fd, lownib | mode);
}

/**
 * @brief Envía un comando a la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param cmd Comando a enviar.
 */
static void sendCommand(int fd, uint8_t cmd)
{
    send(fd, cmd, 0);
}

/**
 * @brief Envía datos para ser mostrados en pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param data Dato a mostrar.
 */
static void sendData(int fd, uint8_t data)
{
    send(fd, data, RS);
}

/**
 * @brief Escribe 4 bits de datos en el LCD.
 * @param fd Descriptor de archivo I2C.
 * @param value Valor de 4 bits.
 */
static void write4bits(int fd, uint8_t value)
{
    expanderWrite(fd, value);
    pulseEnable(fd, value);
}

/**
 * @brief Envía datos al expansor I2C junto con el control de retroiluminación.
 * @param fd Descriptor de archivo I2C.
 * @param data Datos a escribir.
 */
static void expanderWrite(int fd, uint8_t _data)
{
    uint8_t data = _data | _backlightval;
    wiringPiI2CWrite(fd, data);
}

/**
 * @brief Genera un pulso de habilitación para que el LCD procese los datos.
 * @param fd Descriptor de archivo I2C.
 * @param data Datos para generar el pulso.
 */
static void pulseEnable(int fd, uint8_t _data)
{
    expanderWrite(fd, _data | ENABLE);
    delayMicroseconds(1);    // enable pulse debe ser >450ns
    
    expanderWrite(fd, _data & ~ENABLE);
    delayMicroseconds(50);   // comando necesita >37us para ejecutarse
}

// ==============================================
// Funciones públicas para el usuario
// ==============================================
/**
 * @brief Inicializa la pantalla LCD.
 * @param addr Dirección I2C del dispositivo LCD.
 * @param rows Número de filas de la pantalla.
 * @param dotsize Tamaño de punto de los caracteres.
 * @return Descriptor de archivo o -1 en caso de error.
 */
int LCD_Init(int addr, uint8_t rows, uint8_t dotsize)
{
    int fd = wiringPiI2CSetup(addr);
    if (fd == -1) {
        return -1;
    }

    _backlightval = LCD_BACKLIGHT;
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _numlines = rows;

    // Determinar tamaño de fuente
    uint8_t displayfunction = LCD_4BITMODE | LCD_1LINE;
    
    if (rows > 1) {
        displayfunction |= LCD_2LINE;
    }
    
    // Para pantallas de 1 línea, podemos usar tamaño de fuente 5x10
    if ((dotsize == LCD_5x10DOTS) && (rows == 1)) {
        displayfunction |= LCD_5x10DOTS;
    }

    // Inicialización según el datasheet HD44780
    delay(50); // Espera más de 40ms después de que VCC sube a 4.5V

    // Secuencia de inicialización (igual que antes)
    write4bits(fd, 0x03 << 4);
    delay(5);
    write4bits(fd, 0x03 << 4);
    delay(5);
    write4bits(fd, 0x03 << 4);
    delay(1);
    write4bits(fd, 0x02 << 4);

    // Configurar número de líneas y tamaño de fuente
    sendCommand(fd, LCD_FUNCTIONSET | displayfunction);

    // Resto de la inicialización (igual que antes)
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
    LCD_Clear(fd);
    sendCommand(fd, LCD_ENTRYMODESET | _displaymode);
    delay(100);

    return fd;
}

/**
 * @brief Cierra la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Close(int fd) 
{
    LCD_NoBacklight(fd);
    close(fd);
}

/**
 * @brief Limpia la pantalla.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Clear(int fd)
{
    sendCommand(fd, LCD_CLEARDISPLAY);
    delay(2); // este comando tarda 1.52ms
}

/**
 * @brief Regresa el cursor a la posición inicial.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Home(int fd)
{
    sendCommand(fd, LCD_RETURNHOME);
    delay(2); // este comando tarda 1.52ms
}

/**
 * @brief Posiciona el cursor en una coordenada específica.
 * @param fd Descriptor de archivo I2C.
 * @param col Columna destino.
 * @param row Fila destino.
 */
void LCD_SetCursor(int fd, uint8_t col, uint8_t row)
{
    const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54}; // Soporte para hasta 4 filas
    
    if (row >= _numlines) {
        row = _numlines - 1; // Nos aseguramos de no exceder el número de filas
    }
    
    sendCommand(fd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/**
 * @brief Apaga la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoDisplay(int fd) {
    _displaycontrol &= ~LCD_DISPLAYON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Enciende la pantalla LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Display(int fd) {
    _displaycontrol |= LCD_DISPLAYON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Desactiva el cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoCursor(int fd)
{
    _displaycontrol &= ~LCD_CURSORON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Activa el cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Cursor(int fd)
{
    _displaycontrol |= LCD_CURSORON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Desactiva el parpadeo del cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoBlink(int fd)
{
    _displaycontrol &= ~LCD_BLINKON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Activa el parpadeo del cursor.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Blink(int fd)
{
    _displaycontrol |= LCD_BLINKON;
    sendCommand(fd, LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * @brief Desplaza el contenido del display hacia la izquierda.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_ScrollDisplayLeft(int fd) 
{
    sendCommand(fd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/**
 * @brief Desplaza el contenido del display hacia la derecha.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_ScrollDisplayRight(int fd) 
{
    sendCommand(fd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/**
 * @brief Configura la escritura de izquierda a derecha.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_LeftToRight(int fd)
{
    _displaymode |= LCD_ENTRYLEFT;
    sendCommand(fd, LCD_ENTRYMODESET | _displaymode);
}

/**
 * @brief Configura la escritura de derecha a izquierda.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_RightToLeft(int fd) 
{
    _displaymode &= ~LCD_ENTRYLEFT;
    sendCommand(fd, LCD_ENTRYMODESET | _displaymode);
}

/**
 * @brief Apaga la retroiluminación del LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_NoBacklight(int fd) {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(fd, 0);
}

/**
 * @brief Enciende la retroiluminación del LCD.
 * @param fd Descriptor de archivo I2C.
 */
void LCD_Backlight(int fd) {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(fd, 0);
}

/**
 * @brief Ajusta el estado de la retroiluminación.
 * @param fd Descriptor de archivo I2C.
 * @param new_val Valor para encender (1) o apagar (0) la retroiluminación.
 */
void LCD_SetBacklight(int fd, uint8_t new_val)
{
    if (new_val) {
        LCD_Backlight(fd);
    } else {
        LCD_NoBacklight(fd);
    }
}

/**
 * @brief Imprime un carácter en la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param c Carácter a imprimir.
 */
void LCD_PrintChar(int fd, char c) 
{
    sendData(fd, c);
}

/**
 * @brief Imprime una cadena de caracteres en la pantalla.
 * @param fd Descriptor de archivo I2C.
 * @param c Cadena de caracteres a imprimir.
 */
void LCD_PrintStr(int fd, const char c[])
{
    while (*c) {
        LCD_PrintChar(fd, *c++);
    }
}

/**
 * @brief Crea un carácter personalizado.
 * @param fd Descriptor de archivo I2C.
 * @param location Ubicación de la memoria CGRAM.
 * @param charmap Mapa de bits del nuevo carácter.
 */
void LCD_CreateChar(int fd, uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // Solo tenemos 8 ubicaciones (0-7)
    sendCommand(fd, LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        sendData(fd, charmap[i]);
    }
}

/**
 * @brief Imprime un carácter personalizado ya creado.
 * @param fd Descriptor de archivo I2C.
 * @param index Índice del carácter.
 */
void LCD_PrintCustomChar(int fd, uint8_t index) {
    sendData(fd, index);
}