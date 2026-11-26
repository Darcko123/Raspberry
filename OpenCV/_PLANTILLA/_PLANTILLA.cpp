/**
 * @file _PLANTILLA.cpp
 *
 * @brief Plantilla de los ejemplos de OpenCV: carga avatares.png y la muestra.
 *
 * Para crear un ejemplo nuevo, copie la carpeta _PLANTILLA/ y renombre la carpeta,
 * este archivo y el project() del CMakeLists.txt con el mismo nombre en PascalCase.
 *
 * @author Daniel Ruiz
 * @date Noviembre 25, 2026
 */

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    // CMake copia avatares.png a build/, que es el directorio de trabajo
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    imshow("Plantilla", image);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
