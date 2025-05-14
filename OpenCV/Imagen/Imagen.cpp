#include <iostream>               // Para salida de mensajes de error
#include <opencv2/opencv.hpp>     // Librer�a principal de OpenCV
#include "opencv2/highgui.hpp"    // Funciones para interfaz gr�fica (ventanas)
#include "opencv2/imgproc.hpp"    // Operaciones de procesamiento de imagen
#include "opencv2/imgcodecs.hpp"  // Funciones para lectura/escritura de im�genes

using namespace std;              // Espacio de nombres est�ndar
using namespace cv;               // Espacio de nombres de OpenCV

int main() 
{ 
    // =============================================
    // Carga de la imagen
    // =============================================
    //imread puede leer �BMP, DIB, JPEG, JPE, PNG, PBM, PGM, PPM, SR, RAS, and TIFF�
    Mat img = imread("avatares.jpg", IMREAD_COLOR);

    // =============================================
    // Verificaci�n de carga de imagen
    // =============================================
    if(img.empty())
    {
        cerr << "Error: No se pudo cargar la imagen 'avatares.jpg'" << endl;
        cerr << "Verifique que la imagen existe en el directorio correcto" << endl;
        return EXIT_FAILURE;
    }
    // =============================================
    // Muestra imagen
    // =============================================
    imshow("Imagen avatares", img);

    //Presiona cualquier tecla para salir
    waitKey(0);

    return 0;
} 
