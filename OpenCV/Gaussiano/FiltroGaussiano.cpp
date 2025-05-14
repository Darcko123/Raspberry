#include <iostream>                // Para entrada/salida estándar y mensajes de error
#include <opencv2/opencv.hpp>      // Incluye todos los módulos principales de OpenCV

using namespace std;              // Espacio de nombres estándar
using namespace cv;               // Espacio de nombres de OpenCV

int main(int argc, char** argv)
{
    // =============================================
    // Declaración de variables y configuración inicial
    // =============================================
    Mat imageOriginal, gaussianResult;  // Matrices para imagen original y resultado
    
    // Configuración de la ventana de visualización
    const char* window_name = "Filtro Gaussiano - Suavizado de Imagen";
    const int kernelSize = 3;           // Tamaño del kernel (3x3)

    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    imageOriginal = imread("avatares.jpg", IMREAD_COLOR);  // Cargar imagen a color

    if(imageOriginal.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.jpg'" << endl;
        cerr << "Verifique que:" << endl;
        cerr << "1. La imagen existe en el directorio actual" << endl;
        cerr << "2. El nombre y extensión son correctos" << endl;
        cerr << "3. Tiene permisos de lectura" << endl;
        return EXIT_FAILURE;  // Código de error estándar
    }

    // =============================================
    // Aplicación del Filtro Gaussiano
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
    GaussianBlur(imageOriginal,       // Imagen de entrada
                gaussianResult,       // Imagen de salida
                Size(kernelSize, kernelSize), // Tamaño del kernel
                0,                    // Sigma X (0 = auto-calculado)
                0,                    // Sigma Y
                BORDER_DEFAULT);      // Tratamiento de bordes

    // =============================================
    // Visualización y guardado de resultados
    // =============================================
    // Mostrar imagen original y resultado
    imshow("Imagen Original", imageOriginal);
    imshow(window_name, gaussianResult);

    // Opción para guardar el resultado (descomentar para usar)
    // imwrite("imagen_suavizada_gaussiano.jpg", gaussianResult);

    // Mensaje de ayuda para el usuario
    cout << "Presione cualquier tecla para salir..." << endl;

    // Esperar entrada del usuario
    waitKey(0);

    return EXIT_SUCCESS;  // Indica terminación exitosa
}
