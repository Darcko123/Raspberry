/**
 * @file _PLANTILLA.cpp
 *
 * @brief Plantilla de los ejemplos de OpenCV: carga avatares.png y la muestra.
 *
 * Para crear un ejemplo nuevo, copie la carpeta _PLANTILLA/ y renombre la carpeta,
 * este archivo y el project() del CMakeLists.txt con el mismo nombre en PascalCase.
 *
 * Convenciones que siguen todos los ejemplos:
 *  - Solo se incluyen los módulos de OpenCV que pide find_package() en el CMakeLists.txt.
 *  - Identificadores en inglés y en camelCase; comentarios y títulos de ventana en español.
 *  - main() sin argumentos, errores por cerr y salida con EXIT_SUCCESS o EXIT_FAILURE.
 *  - Funciones privadas (static): prototipos antes de main() y definiciones después.
 *  - Cada sección empieza con un bloque de comentarios // ====.
 *
 * @author Daniel Ruiz
 * @date Noviembre 25, 2026
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
    // CMake copia avatares.png a build/, que es el directorio de trabajo
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Visualización
    // =============================================
    imshow("Plantilla", image);
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
