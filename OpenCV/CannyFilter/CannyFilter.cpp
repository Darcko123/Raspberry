/**
 * @file CannyFilter.cpp
 *
 * @brief Detecta los bordes de avatares.png con Canny, con umbrales ajustables por trackbars.
 *
 * @author Daniel Ruiz
 * @date Junio 10, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat
#include <opencv2/imgproc.hpp>   // cvtColor, GaussianBlur, Canny
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // namedWindow, createTrackbar, imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Configuración
    // =============================================
    int cannyLow = 50;    // Umbral bajo para el filtro Canny
    int cannyHigh = 150;  // Umbral alto para el filtro Canny

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
    // Convertir a escala de grises
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Filtro Gaussiano para suavizado de imagen
    Mat blurred;
    GaussianBlur(gray, blurred,
                 Size(3, 3),       // Tamaño del kernel Gaussiano
                 3,                // Desviación estándar en X
                 3,                // Desviación estándar en Y
                 BORDER_DEFAULT);  // Método de bordes

    // =============================================
    // Configuración de la interfaz
    // =============================================
    namedWindow("Config", WINDOW_AUTOSIZE);        // Ventana para los controles
    namedWindow("Filtro Canny", WINDOW_AUTOSIZE);  // Ventana para el resultado Canny

    // Crear trackbars para ajustar los umbrales del filtro Canny
    createTrackbar("Umbral bajo Canny", "Config", &cannyLow, 255);
    createTrackbar("Umbral alto Canny", "Config", &cannyHigh, 255);

    // =============================================
    // Bucle principal
    // =============================================
    Mat edges;
    while (true) {
        // Aplicar el filtro Canny con los valores actuales de los trackbars
        Canny(blurred, edges,
              cannyLow,   // Umbral bajo para la detección de bordes
              cannyHigh,  // Umbral alto para la detección de bordes
              3);         // Apertura del operador de Sobel

        // Mostrar las imágenes
        imshow("Imagen original", image);
        imshow("Imagen en escala de grises", gray);
        imshow("Filtro Gaussiano", blurred);
        imshow("Filtro Canny", edges);

        // Esperar 30 ms por una tecla; ESC (código 27) sale del bucle
        int key = waitKey(30);
        if (key == 27) {
            break;
        }
    }

    return EXIT_SUCCESS;
}
