#include <iostream>                // Para entrada/salida est�ndar y mensajes de error
#include <opencv2/opencv.hpp>      // Incluye todos los m�dulos principales de OpenCV

using namespace std;              // Espacio de nombres est�ndar
using namespace cv;               // Espacio de nombres de OpenCV

int main(int argc, char** argv)
{
    // =============================================
    // Declaraci�n de variables y configuraci�n inicial
    // =============================================
    Mat imageOriginal, gaussianResult;  // Matrices para imagen original y resultado
    
    // Configuraci�n de la ventana de visualizaci�n
    const char* window_name = "Filtro Gaussiano - Suavizado de Imagen";
    const int kernelSize = 3;           // Tama�o del kernel (3x3)

    // =============================================
    // Carga y verificaci�n de la imagen
    // =============================================
    imageOriginal = imread("avatares.jpg", IMREAD_COLOR);  // Cargar imagen a color

    if(imageOriginal.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.jpg'" << endl;
        cerr << "Verifique que:" << endl;
        cerr << "1. La imagen existe en el directorio actual" << endl;
        cerr << "2. El nombre y extensi�n son correctos" << endl;
        cerr << "3. Tiene permisos de lectura" << endl;
        return EXIT_FAILURE;  // C�digo de error est�ndar
    }

    // =============================================
    // Aplicaci�n del Filtro Gaussiano
    // =============================================
    /*
     * El filtro Gaussiano es un suavizado (blur) que:
     * - Reduce ruido y detalles peque�os
     * - Preserva mejor los bordes que un blur promedio
     * - Usa una funci�n de peso Gaussiana (campana de Gauss)
     * 
     * Par�metros:
     * 1. Size(3,3): Tama�o del kernel (ancho, alto) - debe ser impar
     * 2. 0: Sigma en X (0 = calculado autom�ticamente desde kernel size)
     * 3. 0: Sigma en Y (si es 0, usa el mismo valor que sigma X)
     * 4. BORDER_DEFAULT: M�todo para manejar p�xeles en bordes
     */
    GaussianBlur(imageOriginal,       // Imagen de entrada
                gaussianResult,       // Imagen de salida
                Size(kernelSize, kernelSize), // Tama�o del kernel
                0,                    // Sigma X (0 = auto-calculado)
                0,                    // Sigma Y
                BORDER_DEFAULT);      // Tratamiento de bordes

    // =============================================
    // Visualizaci�n y guardado de resultados
    // =============================================
    // Mostrar imagen original y resultado
    imshow("Imagen Original", imageOriginal);
    imshow(window_name, gaussianResult);

    // Opci�n para guardar el resultado (descomentar para usar)
    // imwrite("imagen_suavizada_gaussiano.jpg", gaussianResult);

    // Mensaje de ayuda para el usuario
    cout << "Presione cualquier tecla para salir..." << endl;

    // Esperar entrada del usuario
    waitKey(0);

    return EXIT_SUCCESS;  // Indica terminaci�n exitosa
}
