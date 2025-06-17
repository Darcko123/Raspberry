#include <iostream>               // Para salida de mensajes de error
#include <opencv2/opencv.hpp>     // Incluye todos los módulos principales de OpenCV
#include "opencv2/highgui.hpp"    // Funciones para interfaz gráfica (ventanas)
#include "opencv2/imgproc.hpp"    // Operaciones de procesamiento de imagen
#include "opencv2/imgcodecs.hpp"  // Funciones para lectura/escritura de imágenes
#include "opencv2/objdetect.hpp"  // Para detección de objetos (clasificadores en cascada)
#include "opencv2/features2d.hpp" // Para detección de características (opcional)

using namespace std;              // Espacio de nombres estándar
using namespace cv;               // Espacio de nombres de OpenCV

// =============================================
// Funciones
// =============================================
void detectarYMostrar(Mat& imagen, CascadeClassifier& clasificador);
// =============================================

// =============================================
// Función principal
// =============================================
int main()
{
    // =============================================
    // Declaración de variables y configuración inicial
    // =============================================
    Mat imagenOriginal; // Imagen capturada de la cámara
    
    CascadeClassifier clasificador; // Clasificador en cascada para detección de rostros

    double scale = 1; // Factor de escala para la imagen

    // =============================================
    if (!clasificador.load("haarcascade_frontalface_alt.xml"))  // Cargar el clasificador Haar para detección de rostros
    {
        cerr << "Error: No se pudo cargar el clasificador Haar." << endl;   // Mensaje de error si no se carga el clasificador
        return EXIT_FAILURE;  // Código de error si no se carga el clasificador
    }

    // =============================================
    // Configuraciones de la cámara
    // =============================================
    VideoCapture capture(0); // Inicializar la cámara (0 = cámara por defecto)
    if (!capture.isOpened()) // Verificar si la cámara se abrió correctamente
    {
        cerr << "Error: No se pudo abrir la cámara." << endl;
        return EXIT_FAILURE; // Código de error estándar si no se abre la cámara
    }

    capture.set(CAP_PROP_FRAME_WIDTH, 640);     // Ancho del frame
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);    // Alto del frame

    // =============================================
    // Iniciar la captura de video y detección de rostros
    cout << "Detección de rostros iniciada. Presiona 'q' o 'Esc' para salir." << endl;
    namedWindow("Rostros Detectados", WINDOW_AUTOSIZE);
    // =============================================

    while(1)
    {
        capture >> imagenOriginal; // Capturar un frame de la cámara
        if(imagenOriginal.empty()) // Verificar si el frame está vacío
        {
            cerr << "Error: Frame vacío capturado." << endl;
            return EXIT_FAILURE; // Código de error estándar si no se captura la imagen
        }

        // =============================================
        detectarYMostrar(imagenOriginal, clasificador);  // Llamar a la función para detectar y mostrar rostros

        // =============================================
        // Esperar por una tecla para continuar o salir
        // =============================================
        char key = waitKey(1); // Esperar 1 ms para detectar teclas
        if(key == 27 || key == 'q')
        {
            cout << "Saliendo del programa..." << endl; // Mensaje de salida
            // Esperar un momento para que se muestre el mensaje antes de cerrar
            waitKey(1000); // Espera 1 segundo
            // Salir del bucle
            break;
        }
        // =============================================
    }
}

// =============================================
// Función para detectar y dibujar rostros en la imagen
// =============================================
void detectarYMostrar(Mat& imagen, CascadeClassifier& clasificador)
{
    vector<Rect> rostros;
    Mat imagenGris;

    // Convertir a escala de grises y mejorar contraste
    cvtColor(imagen, imagenGris, COLOR_BGR2GRAY);
    equalizeHist(imagenGris, imagenGris);

    // Detección de rostros
    clasificador.detectMultiScale(imagenGris, rostros, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // Dibujar los rostros detectados
    for (const Rect& rostro : rostros)
    {
        rectangle(imagen, rostro, Scalar(255, 0, 0), 2);
    }

    // Mostrar la imagen con los rostros
    imshow("Rostros Detectados", imagen);
}