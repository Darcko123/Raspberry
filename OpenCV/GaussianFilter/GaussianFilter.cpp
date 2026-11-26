/**
 * @file GaussianFilter.cpp
 *
 * @brief Suaviza avatares.png con un filtro Gaussiano de 3×3.
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cout, cerr
#include <opencv2/core.hpp>      // Mat
#include <opencv2/imgproc.hpp>   // GaussianBlur
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Configuración
    // =============================================
    const int kernelSize = 3;  // Tamaño del kernel (3x3)

    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Aplicación del filtro Gaussiano
    // =============================================
    /*
     * El filtro Gaussiano es un suavizado (blur) que:
     * - Reduce ruido y detalles pequeños
     * - Preserva mejor los bordes que un blur promedio
     * - Usa una función de peso Gaussiana (campana de Gauss)
     *
     * Parámetros:
     * 1. Size(3,3): Tamaño del kernel (ancho, alto) - debe ser impar
     * 2. 0: Sigma en X (0 = calculado automáticamente desde kernel size)
     * 3. 0: Sigma en Y (si es 0, usa el mismo valor que sigma X)
     * 4. BORDER_DEFAULT: Método para manejar píxeles en bordes
     */
    Mat blurred;
    GaussianBlur(image,                         // Imagen de entrada
                 blurred,                       // Imagen de salida
                 Size(kernelSize, kernelSize),  // Tamaño del kernel
                 0,                             // Sigma X (0 = auto-calculado)
                 0,                             // Sigma Y
                 BORDER_DEFAULT);               // Tratamiento de bordes

    // =============================================
    // Visualización
    // =============================================
    imshow("Imagen original", image);
    imshow("Filtro Gaussiano - Suavizado de imagen", blurred);

    cout << "Presione cualquier tecla para salir..." << endl;
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
