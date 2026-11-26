/**
 * @file FaceDetect.cpp
 *
 * @brief Detecta rostros de frente en el vídeo de la cámara con un clasificador Haar en cascada.
 *
 * @author Daniel Ruiz
 * @date Junio 16, 2025
 */

#include <iostream>              // cout, cerr
#include <vector>                // vector
#include <opencv2/core.hpp>      // Mat, Rect
#include <opencv2/imgproc.hpp>   // cvtColor, equalizeHist, rectangle
#include <opencv2/highgui.hpp>   // namedWindow, imshow, waitKey
#include <opencv2/objdetect.hpp> // CascadeClassifier
#include <opencv2/videoio.hpp>   // VideoCapture

using namespace std;
using namespace cv;

// =============================================
// Detección y dibujo de rostros
// =============================================
static void detectAndDraw(Mat& image, CascadeClassifier& classifier)
{
    vector<Rect> faces;
    Mat gray;

    // Convertir a escala de grises y mejorar contraste
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    // Detección de rostros
    classifier.detectMultiScale(gray, faces, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // Dibujar los rostros detectados
    for (const Rect& face : faces) {
        rectangle(image, face, Scalar(255, 0, 0), 2);
    }

    // Mostrar la imagen con los rostros
    imshow("Rostros detectados", image);
}

int main()
{
    // =============================================
    // Carga y verificación del clasificador
    // =============================================
    CascadeClassifier classifier;  // Clasificador en cascada para detección de rostros
    if (!classifier.load("haarcascade_frontalface_alt.xml")) {
        cerr << "Error: No se pudo cargar el clasificador Haar." << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Configuración de la cámara
    // =============================================
    VideoCapture capture(0);  // Inicializar la cámara (0 = cámara por defecto)
    if (!capture.isOpened()) {
        cerr << "Error: No se pudo abrir la cámara." << endl;
        return EXIT_FAILURE;
    }

    capture.set(CAP_PROP_FRAME_WIDTH, 640);   // Ancho del frame
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);  // Alto del frame

    // =============================================
    // Configuración de la interfaz
    // =============================================
    cout << "Detección de rostros iniciada. Presiona 'q' o 'Esc' para salir." << endl;
    namedWindow("Rostros detectados", WINDOW_AUTOSIZE);

    // =============================================
    // Bucle principal
    // =============================================
    Mat frame;  // Imagen capturada de la cámara
    while (true) {
        capture >> frame;  // Capturar un frame de la cámara
        if (frame.empty()) {
            cerr << "Error: Frame vacío capturado." << endl;
            return EXIT_FAILURE;
        }

        detectAndDraw(frame, classifier);

        // Esperar 1 ms por una tecla; 'q' o ESC (código 27) sale del bucle
        int key = waitKey(1);
        if (key == 27 || key == 'q') {
            cout << "Saliendo del programa..." << endl;
            waitKey(1000);  // Espera 1 segundo para que se vea el mensaje antes de cerrar
            break;
        }
    }

    return EXIT_SUCCESS;
}
