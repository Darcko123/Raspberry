#include "opencv2/imgproc.hpp"    // Operaciones de procesamiento de imagen
#include "opencv2/imgcodecs.hpp"  // Funciones para lectura/escritura de imágenes
#include "opencv2/highgui.hpp"    // Funciones para interfaz gráfica (ventanas)
#include <iostream>               // Para salida de mensajes de error

using namespace std;              // Espacio de nombres estándar
using namespace cv;               // Espacio de nombres de OpenCV

int main(int argc, char** argv)
{
    // =============================================
    // Declaración de variables y configuración inicial
    // =============================================
    Mat imagenOriginal, grayImage, laplacianResult;
    
    // Parámetros para el operador Laplaciano
    const int kernel_size = 3;    // Tamaño del kernel (3x3 es común para Laplaciano)
    const int scale = 1;          // Factor de escala para los valores del derivado
    const int delta = 0;          // Valor delta añadido a los resultados
    const int ddepth = CV_16S;    // Profundidad de salida (16-bit signed para preservar bordes negativos)
    
    const char* window_name = "Filtro Laplaciano - Detección de Bordes";

    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    imagenOriginal = imread("avatares.jpg", IMREAD_COLOR);
    
    if(imagenOriginal.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.jpg'" << endl;
        cerr << "Verifique que la imagen existe en el directorio correcto" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Pre-procesamiento de la imagen
    // =============================================
    /*
     * Aplicamos un suavizado Gaussiano para reducir el ruido antes de la detección de bordes.
     * Esto es importante porque el Laplaciano es muy sensible al ruido.
     * Kernel size = 3x3, sigmaX e Y = 0 (calculado automáticamente)
     */
    GaussianBlur(imagenOriginal, imagenOriginal, 
                Size(3, 3),  // Tamaño del kernel Gaussiano
                0,           // Desviación estándar en X (0 = calculada automáticamente)
                0,           // Desviación estándar en Y
                BORDER_DEFAULT); // Método de bordes

    // Convertir a escala de grises (el Laplaciano trabaja sobre imágenes de un solo canal)
    cvtColor(imagenOriginal, grayImage, COLOR_BGR2GRAY);

    // =============================================
    // Aplicación del operador Laplaciano
    // =============================================
    /*
     * El operador Laplaciano calcula las segundas derivadas de la imagen,
     * resaltando las regiones de cambio rápido en intensidad (bordes).
     * 
     * Parámetros:
     * - ddepth: CV_16S para preservar bordes negativos
     * - kernel_size: Tamaño del kernel (1, 3, 5 o 7)
     * - scale: Factor de escalamiento opcional
     * - delta: Valor a añadir a los resultados
     * - BORDER_DEFAULT: Método para manejar bordes
     */
    Mat laplacian16S;  // Imagen temporal para el resultado en 16S
    Laplacian(grayImage, laplacian16S, 
              ddepth, 
              kernel_size, 
              scale, 
              delta, 
              BORDER_DEFAULT);

    // =============================================
    // Post-procesamiento y visualización
    // =============================================
    /*
     * Convertimos de vuelta a 8-bit sin signo (0-255) usando valores absolutos,
     * ya que el Laplaciano produce valores positivos y negativos.
     */
    convertScaleAbs(laplacian16S, laplacianResult);

    // Mostrar el resultado
    imshow(window_name, laplacianResult);
    
    // Opcional: Guardar el resultado
    // imwrite("bordes_laplaciano.jpg", laplacianResult);

    // Esperar hasta que el usuario presione una tecla
    waitKey(0);

    return EXIT_SUCCESS;
}
