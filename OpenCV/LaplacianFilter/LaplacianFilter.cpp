/**
 * @file LaplacianFilter.cpp
 *
 * @brief Detecta los bordes de avatares.png con el operador Laplaciano.
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat, convertScaleAbs
#include <opencv2/imgproc.hpp>   // GaussianBlur, cvtColor, Laplacian
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Configuración
    // =============================================
    const int kernelSize = 3;   // Tamaño del kernel (3x3 es común para Laplaciano)
    const int scale = 1;        // Factor de escala para los valores del derivado
    const int delta = 0;        // Valor delta añadido a los resultados
    const int ddepth = CV_16S;  // Profundidad de salida (16-bit signed para preservar bordes negativos)

    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Pre-procesamiento de la imagen
    // =============================================
    /*
     * Aplicamos un suavizado Gaussiano para reducir el ruido antes de la detección de bordes.
     * Esto es importante porque el Laplaciano es muy sensible al ruido.
     * Kernel size = 3x3, sigmaX e Y = 0 (calculado automáticamente).
     * Se aplica sobre la misma matriz para no reservar otra imagen BGR.
     */
    GaussianBlur(image, image,
                 Size(3, 3),       // Tamaño del kernel Gaussiano
                 0,                // Desviación estándar en X (0 = calculada automáticamente)
                 0,                // Desviación estándar en Y
                 BORDER_DEFAULT);  // Método de bordes

    // Convertir a escala de grises (el Laplaciano trabaja sobre imágenes de un solo canal)
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // =============================================
    // Aplicación del operador Laplaciano
    // =============================================
    /*
     * El operador Laplaciano calcula las segundas derivadas de la imagen,
     * resaltando las regiones de cambio rápido en intensidad (bordes).
     *
     * Parámetros:
     * - ddepth: CV_16S para preservar bordes negativos
     * - kernelSize: Tamaño del kernel (1, 3, 5 o 7)
     * - scale: Factor de escalamiento opcional
     * - delta: Valor a añadir a los resultados
     * - BORDER_DEFAULT: Método para manejar bordes
     */
    Mat laplacian16S;  // Imagen temporal para el resultado en 16S
    Laplacian(gray, laplacian16S,
              ddepth,
              kernelSize,
              scale,
              delta,
              BORDER_DEFAULT);

    // =============================================
    // Post-procesamiento
    // =============================================
    /*
     * Convertimos de vuelta a 8-bit sin signo (0-255) usando valores absolutos,
     * ya que el Laplaciano produce valores positivos y negativos.
     */
    Mat laplacian;
    convertScaleAbs(laplacian16S, laplacian);

    // =============================================
    // Visualización
    // =============================================
    imshow("Filtro Laplaciano - Detección de bordes", laplacian);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
