/**
 * @file HSVFilter.cpp
 *
 * @brief Segmenta los píxeles rojos, verdes y azules de avatares.png con rangos en el espacio HSV.
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat, inRange, bitwise_and
#include <opencv2/imgproc.hpp>   // cvtColor
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Conversión de BGR a HSV
    // =============================================
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // =============================================
    // Máscaras por rango de tono
    // =============================================
    Mat maskRed1, maskRed2, maskGreen, maskBlue;

    // Rango de tono para ROJO (hay dos rangos por el ciclo del tono en HSV)
    inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), maskRed1);     // Rojo bajo
    inRange(hsv, Scalar(160, 100, 100), Scalar(179, 255, 255), maskRed2);  // Rojo alto
    Mat maskRed = maskRed1 | maskRed2;

    // Rango de tono para VERDE
    inRange(hsv, Scalar(35, 100, 100), Scalar(85, 255, 255), maskGreen);

    // Rango de tono para AZUL
    inRange(hsv, Scalar(100, 100, 100), Scalar(130, 255, 255), maskBlue);

    // =============================================
    // Aplicación de las máscaras
    // =============================================
    Mat red, green, blue;
    bitwise_and(image, image, red, maskRed);
    bitwise_and(image, image, green, maskGreen);
    bitwise_and(image, image, blue, maskBlue);

    // =============================================
    // Visualización
    // =============================================
    imshow("Imagen original", image);
    imshow("Rojo (HSV)", red);
    imshow("Verde (HSV)", green);
    imshow("Azul (HSV)", blue);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
