#include <iostream>               // Para salida de mensajes de error
#include <opencv2/opencv.hpp>     // Incluye todos los módulos principales de OpenCV
#include "opencv2/highgui.hpp"    // Funciones para interfaz gráfica (ventanas)
#include "opencv2/imgproc.hpp"    // Operaciones de procesamiento de imagen
#include "opencv2/imgcodecs.hpp"  // Funciones para lectura/escritura de imágenes

using namespace std;              // Espacio de nombres estándar
using namespace cv;               // Espacio de nombres de OpenCV

int main(int argc, char** argv)
{
    // =============================================
    // Declaración de variables y configuración inicial
    // =============================================
    Mat imagenOriginal; // Matriz para la imagen original
    Mat imagenGris;     // Matriz para la imagen en escala de grises
    Mat filtroGausiano; // Matriz para el resultado del filtro Gaussiano
    Mat filtroCanny;    // Matriz para el resultado del filtro Canny

    int cannyLow = 50;   // Umbral bajo para el filtro Canny
    int cannyHigh = 150; // Umbral alto para el filtro Canny
    // =============================================

    // =============================================
    // Carga y verificación de la imagen/video
    // =============================================
    imagenOriginal = imread("Lenna.png"); // Cargar imagen desde archivo
    if(imagenOriginal.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'Lenna.png'" << endl;
        cerr << "Verifique que:" << endl;
        cerr << "1. La imagen existe en el directorio actual" << endl;
        cerr << "2. El nombre y extensión son correctos" << endl;
        cerr << "3. Tiene permisos de lectura" << endl;
        return EXIT_FAILURE;  // Código de error estándar
    }
    // =============================================

    // =============================================
    // Pre-procesamiento de la imagen
    // =============================================
    // Convertir a escala de grises
    cvtColor(imagenOriginal, imagenGris, COLOR_BGR2GRAY);
    // =============================================

    // =============================================
    // Filtro Gaussiano para suavizado de imagen
    // =============================================
    GaussianBlur(imagenGris, filtroGausiano, 
        Size(3, 3),  // Tamaño del kernel Gaussiano
        3,           // Desviación estándar en X
        3,           // Desviación estándar en Y
        BORDER_DEFAULT); // Método de bordes
    // =============================================

    // =============================================
    // Configuración de la interfaz
    // =============================================
    namedWindow("Config", WINDOW_AUTOSIZE);         // Crear ventana para los controles
    namedWindow("Filtro Canny", WINDOW_AUTOSIZE);   // Ventana para el resultado Canny

    // Crear trackbars para ajustar los umbrales del filtro Canny
    createTrackbar("Umbral Bajo Canny", "Config", &cannyLow, 255); 
    createTrackbar("Umbral Alto Canny", "Config", &cannyHigh, 255);
    // =============================================

    // =============================================
    // Bucle principal
    // =============================================
    while (true)
    {
        // Aplicar el filtro Canny con los valores actuales de los trackbars
        Canny(filtroGausiano, filtroCanny, 
            cannyLow,     // Umbral bajo para la detección de bordes
            cannyHigh,    // Umbral alto para la detección de bordes
            3);           // Tamaño del kernel para suavizado previo

        // Mostrar las imágenes
        imshow("Imagen Original", imagenOriginal);
        imshow("Imagen en Escala de Grises", imagenGris);
        imshow("Filtro Gaussiano", filtroGausiano);
        imshow("Filtro Canny", filtroCanny);

        // Esperar 30ms por una tecla, si se presiona ESC, salir
        int key = waitKey(30);
        if (key == 27) { // 27 es el código ASCII para ESC
            break;
        }
    }
    // =============================================

    return EXIT_SUCCESS; // Código de éxito estándar
}