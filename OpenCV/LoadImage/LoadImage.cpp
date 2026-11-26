/**
 * @file LoadImage.cpp
 *
 * @brief Carga avatares.png desde disco y la muestra en una ventana.
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    // imread puede leer BMP, DIB, JPEG, JPE, PNG, PBM, PGM, PPM, SR, RAS y TIFF
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Visualización
    // =============================================
    imshow("Imagen avatares", image);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
