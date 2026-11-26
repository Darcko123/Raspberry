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
    // (sin puntero de valor, que está obsoleto: la posición se fija y se lee con las funciones *TrackbarPos)
    createTrackbar("Umbral bajo Canny", "Config", nullptr, 255);
    createTrackbar("Umbral alto Canny", "Config", nullptr, 255);
    setTrackbarPos("Umbral bajo Canny", "Config", cannyLow);
    setTrackbarPos("Umbral alto Canny", "Config", cannyHigh);

    // Las tres primeras imágenes no cambian: se muestran una sola vez
    imshow("Imagen original", image);
    imshow("Imagen en escala de grises", gray);
    imshow("Filtro Gaussiano", blurred);

    // =============================================
    // Bucle principal
    // =============================================
    Mat edges;
    int lastLow = -1;   // Umbrales del último resultado mostrado (-1: aún no hay ninguno)
    int lastHigh = -1;
    while (true) {
        // Leer los valores actuales de los trackbars
        cannyLow = getTrackbarPos("Umbral bajo Canny", "Config");
        cannyHigh = getTrackbarPos("Umbral alto Canny", "Config");

        // Recalcular Canny solo si cambió algún umbral
        if (cannyLow != lastLow || cannyHigh != lastHigh) {
            Canny(blurred, edges,
                  cannyLow,   // Umbral bajo para la detección de bordes
                  cannyHigh,  // Umbral alto para la detección de bordes
                  3);         // Apertura del operador de Sobel
            imshow("Filtro Canny", edges);
            lastLow = cannyLow;
            lastHigh = cannyHigh;
        }

        // Esperar 30 ms por una tecla; ESC (código 27) sale del bucle
        int key = waitKey(30);
        if (key == 27) {
            break;
        }
    }

    return EXIT_SUCCESS;
}
