#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // Cargar imagen original
    Mat image = imread("avatares.jpg", IMREAD_COLOR);

    if (image.empty()) {
        cerr << "Error: No se pudo abrir o encontrar la imagen." << endl;
        return -1;
    }

    // Convertir imagen de BGR a HSV
    Mat imageHSV;
    cvtColor(image, imageHSV, COLOR_BGR2HSV);

    // Crear máscaras para cada color
    Mat maskRojo1, maskRojo2, maskVerde, maskAzul;

    // Rango de tono para ROJO (hay dos rangos por el ciclo del tono en HSV)
    inRange(imageHSV, Scalar(0, 100, 100), Scalar(10, 255, 255), maskRojo1);     // Rojo bajo
    inRange(imageHSV, Scalar(160, 100, 100), Scalar(179, 255, 255), maskRojo2);  // Rojo alto
    Mat maskRojo = maskRojo1 | maskRojo2;

    // Rango de tono para VERDE
    inRange(imageHSV, Scalar(35, 100, 100), Scalar(85, 255, 255), maskVerde);

    // Rango de tono para AZUL
    inRange(imageHSV, Scalar(100, 100, 100), Scalar(130, 255, 255), maskAzul);

    // Crear imágenes de salida aplicando las máscaras
    Mat rojo, verde, azul;
    bitwise_and(image, image, rojo, maskRojo);
    bitwise_and(image, image, verde, maskVerde);
    bitwise_and(image, image, azul, maskAzul);

    // Mostrar resultados
    imshow("Imagen original", image);
    imshow("Rojo (HSV)", rojo);
    imshow("Verde (HSV)", verde);
    imshow("Azul (HSV)", azul);

    waitKey(0);
    return 0;
}
