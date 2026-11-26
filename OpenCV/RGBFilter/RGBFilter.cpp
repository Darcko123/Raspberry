/**
 * @file RGBFilter.cpp
 *
 * @brief Separa los píxeles de avatares.png según su canal dominante (rojo, verde o azul).
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat, Vec3b
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
    // Clasificación por canal dominante
    // =============================================
    // Crear matrices vacías (inicializadas en negro) para cada componente de color
    Mat red   = Mat::zeros(image.size(), image.type());
    Mat green = Mat::zeros(image.size(), image.type());
    Mat blue  = Mat::zeros(image.size(), image.type());

    // Recorrer cada píxel de la imagen original
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {

            // Obtener el valor del píxel actual
            Vec3b pixel = image.at<Vec3b>(y, x);

            uchar b = pixel[0]; // Componente azul
            uchar g = pixel[1]; // Componente verde
            uchar r = pixel[2]; // Componente rojo

            // Comparar componentes para encontrar el dominante
            if (r > g && r > b) {
                // Si rojo es dominante, guardar solo el valor del canal rojo
                red.at<Vec3b>(y, x) = Vec3b(0, 0, r);
            }
            else if (g > r && g > b) {
                // Si verde es dominante, guardar solo el valor del canal verde
                green.at<Vec3b>(y, x) = Vec3b(0, g, 0);
            }
            else if (b > r && b > g) {
                // Si azul es dominante, guardar solo el valor del canal azul
                blue.at<Vec3b>(y, x) = Vec3b(b, 0, 0);
            }
            else {
                // Si no hay un color claramente dominante (ej. gris), se deja en negro
                // Esto evita saturar la imagen con falsos positivos
                red.at<Vec3b>(y, x)   = Vec3b(0, 0, 0);
                green.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                blue.at<Vec3b>(y, x)  = Vec3b(0, 0, 0);
            }
        }
    }

    // =============================================
    // Visualización
    // =============================================
    imshow("Imagen original", image);
    imshow("Canal rojo", red);
    imshow("Canal verde", green);
    imshow("Canal azul", blue);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
