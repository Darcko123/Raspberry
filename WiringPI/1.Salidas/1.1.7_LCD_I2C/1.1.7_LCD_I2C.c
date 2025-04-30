#include <stdio.h>
#include <stdlib.h>
#include "liquidcrystal_i2c.h"

int main() {
    // Inicializar WiringPi
    if (wiringPiSetup() == -1) {
        printf("Error al inicializar WiringPi\n");
        return 1;
    }

    // Inicializar LCD (0x27 es la direcciï¿½n comï¿½n para estos displays)
    int fd = LCD_Init(0x27, 4, LCD_5x8DOTS);
    if (fd == -1) {
        printf("Error al inicializar LCD\n");
        return 1;
    }

    // Mostrar mensaje
    LCD_SetCursor(fd, 0, 0);
    LCD_PrintStr(fd, "Hola Raspberry!");
    LCD_SetCursor(fd, 0, 1);
    LCD_PrintStr(fd, "LCD I2C Funciona!");
    LCD_SetCursor(fd, 0, 2);    
    LCD_PrintStr(fd, "Raspberry Zero 2W");

    // Esperar 5 segundos
    delay(5000);

    // Demostrar caracteres personalizados
    uint8_t heart[8] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};
    LCD_CreateChar(fd, 0, heart);
    
    LCD_Clear(fd);
    LCD_PrintStr(fd, "Corazon: ");
    LCD_PrintCustomChar(fd, 0);

    delay(3000);

    // Apagar backlight y cerrar
    LCD_NoBacklight(fd);
    LCD_Close(fd);

    return 0;
}
