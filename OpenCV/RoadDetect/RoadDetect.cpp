/**
 * @file RoadDetect.cpp
 *
 * @brief Detecta las líneas del carril con Canny y Hough; si no hay cámara, usa una imagen de assets/.
 *
 * @author Daniel Ruiz
 * @date Noviembre 26, 2026
 */

#include <cmath>                 // atan, abs
#include <iostream>              // cout, cerr
#include <vector>                // vector
#include <opencv2/core.hpp>      // Mat, Point, Vec4i
#include <opencv2/imgproc.hpp>   // cvtColor, GaussianBlur, Canny, fillConvexPoly, HoughLinesP, polylines, line, putText
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // imshow, waitKey
#include <opencv2/videoio.hpp>   // VideoCapture, CAP_PROP_*

using namespace std;
using namespace cv;

// =============================================
// Entrada alternativa cuando no hay cámara
// =============================================
// CMake copia estas imágenes a build/ desde ../assets/. Se prueba en orden y
// se devuelve la primera que se puede cargar.
static bool loadFallbackImage(Mat& frame, const char*& imagePath)
{
    const char* imagePaths[] = {
        "carretera2_1.jpg",
        "carretera3_1.jpg",
        "carretera4_1.jpg"
    };

    for (const char* path : imagePaths) {
        frame = imread(path, IMREAD_COLOR);
        if (!frame.empty()) {
            imagePath = path;
            return true;
        }
    }
    return false;
}

// =============================================
// Filtrado y clasificación de líneas
// =============================================
// Se conservan solo las líneas con un ángulo entre 20 y 70 grados en valor
// absoluto: se descartan las casi horizontales y las casi verticales.
static vector<Vec4i> filterByAngle(const vector<Vec4i>& lines)
{
    vector<Vec4i> filtered;

    for (const Vec4i& l : lines) {
        int x1 = l[0], y1 = l[1];
        int x2 = l[2], y2 = l[3];

        // Pequeño valor en el denominador para evitar la división por cero
        double slope = static_cast<double>(y2 - y1) / (x2 - x1 + 1e-5);
        double angle = std::atan(slope) * 180.0 / CV_PI;

        if (std::abs(angle) > 20 && std::abs(angle) < 70) {
            filtered.push_back(l);
        }
    }

    return filtered;
}

// Una línea es de la izquierda si su pendiente es negativa y ambos extremos
// están en la mitad izquierda; de la derecha si es positiva y ambos están a la derecha.
static void classifyLines(const vector<Vec4i>& lines, int cols,
                          vector<Vec4i>& left, vector<Vec4i>& right)
{
    for (const Vec4i& l : lines) {
        int x1 = l[0], y1 = l[1];
        int x2 = l[2], y2 = l[3];
        double slope = static_cast<double>(y2 - y1) / (x2 - x1 + 1e-5);

        if (slope < 0 && x1 < cols / 2 && x2 < cols / 2) {
            left.push_back(l);
        } else if (slope > 0 && x1 > cols / 2 && x2 > cols / 2) {
            right.push_back(l);
        }
    }
}

// Promedia los extremos de todas las líneas para obtener una representativa.
static Vec4i averageLines(const vector<Vec4i>& lines)
{
    if (lines.empty()) {
        return Vec4i(0, 0, 0, 0);
    }

    int avgX1 = 0, avgY1 = 0;
    int avgX2 = 0, avgY2 = 0;
    for (const Vec4i& l : lines) {
        avgX1 += l[0];
        avgY1 += l[1];
        avgX2 += l[2];
        avgY2 += l[3];
    }

    int n = static_cast<int>(lines.size());
    return Vec4i(avgX1 / n, avgY1 / n, avgX2 / n, avgY2 / n);
}

// Extiende la línea desde el 60 % de la altura hasta el borde inferior.
static void extendLine(Vec4i& line, int imgRows)
{
    int x1 = line[0], y1 = line[1];
    int x2 = line[2], y2 = line[3];
    double slope = static_cast<double>(y2 - y1) / (x2 - x1 + 1e-5);

    int yBottom = imgRows;
    int xBottom = x1 + static_cast<int>((yBottom - y1) / slope);
    int yTop = static_cast<int>(imgRows * 0.6);
    int xTop = x1 + static_cast<int>((yTop - y1) / slope);

    line = Vec4i(xTop, yTop, xBottom, yBottom);
}

// Detecta, filtra, clasifica y dibuja las líneas del carril.
static void processLines(const Mat& frame, const Mat& roiImage, Mat& houghResult,
                         const vector<Point>& roiVertices)
{
    // Transformada de Hough probabilística sobre la ROI ya enmascarada
    vector<Vec4i> lines;
    HoughLinesP(roiImage, lines,
                1,            // Resolución en píxeles
                CV_PI / 180,  // Resolución angular
                50,           // Umbral de votos
                50,           // Longitud mínima de segmento
                10);          // Separación máxima entre segmentos

    houghResult = frame.clone();  // Copia para dibujar el resultado

    if (lines.empty()) {
        putText(houghResult, "No se detectaron lineas", Point(50, 50),
                FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        return;
    }

    vector<Vec4i> filteredLines = filterByAngle(lines);
    vector<Vec4i> leftLines, rightLines;
    classifyLines(filteredLines, frame.cols, leftLines, rightLines);

    // La ROI se dibuja en magenta sobre la imagen de resultado
    polylines(houghResult, roiVertices, true, Scalar(255, 0, 255), 2);

    if (!leftLines.empty()) {
        Vec4i avgLeft = averageLines(leftLines);
        extendLine(avgLeft, frame.rows);
        line(houghResult,
             Point(avgLeft[0], avgLeft[1]),
             Point(avgLeft[2], avgLeft[3]),
             Scalar(255, 0, 0), 3);
    } else {
        putText(houghResult, "No hay linea izquierda", Point(50, 100),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 2);
    }

    if (!rightLines.empty()) {
        Vec4i avgRight = averageLines(rightLines);
        extendLine(avgRight, frame.rows);
        line(houghResult,
             Point(avgRight[0], avgRight[1]),
             Point(avgRight[2], avgRight[3]),
             Scalar(0, 0, 255), 3);
    } else {
        putText(houghResult, "No hay linea derecha", Point(50, 140),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
    }
}

int main()
{
    // =============================================
    // Configuración de la cámara y entrada alternativa
    // =============================================
    VideoCapture capture(0);  // Inicializar la cámara (0 = cámara por defecto)
    Mat frame;
    const char* imagePath = nullptr;
    bool useImage = false;

    if (capture.isOpened()) {
        capture.set(CAP_PROP_FRAME_WIDTH, 640);   // Ancho del frame
        capture.set(CAP_PROP_FRAME_HEIGHT, 480);  // Alto del frame
        capture.set(CAP_PROP_FPS, 60);            // Frames por segundo
        capture.set(CAP_PROP_BRIGHTNESS, 128);    // Brillo (0-255)
        capture.set(CAP_PROP_CONTRAST, 128);      // Contraste (0-255)
        capture >> frame;  // Primer frame: algunas cámaras abren pero no entregan imagen
    } else {
        cerr << "Aviso: No se pudo abrir la cámara; se prueba con una imagen de assets/." << endl;
    }

    if (!capture.isOpened() || frame.empty()) {
        if (!loadFallbackImage(frame, imagePath)) {
            cerr << "Error: No se pudo abrir la cámara ni cargar una imagen de assets/." << endl;
            return EXIT_FAILURE;
        }
        useImage = true;
        cerr << "Aviso: Usando '" << imagePath << "' en lugar de la cámara." << endl;
    }

    // =============================================
    // Bucle principal
    // =============================================
    Mat gray, blurred, edges;
    Mat laneMask, roiImage, houghResult;

    while (true) {
        if (!useImage) {
            capture >> frame;  // Capturar el siguiente frame de la cámara
            if (frame.empty()) {
                // La cámara puede dejar de entregar frames en cualquier momento
                if (!loadFallbackImage(frame, imagePath)) {
                    cerr << "Error: No se pudo capturar la imagen de la cámara ni cargar una imagen de assets/." << endl;
                    return EXIT_FAILURE;
                }
                useImage = true;
                cerr << "Aviso: Usando '" << imagePath << "' en lugar de la cámara." << endl;
            }
        }

        // Las máscaras se crean con el tamaño del primer frame. Antes se
        // inicializaban con un Mat vacío y la ROI no se aplicaba.
        if (laneMask.size() != frame.size()) {
            laneMask = Mat::zeros(frame.size(), CV_8UC1);
            roiImage = Mat::zeros(frame.size(), CV_8UC1);
        }

        // =============================================
        // Pre-procesamiento: grises y suavizado
        // =============================================
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred,
                     Size(3, 3),       // Tamaño del kernel Gaussiano
                     3,                // Desviación estándar en X
                     3,                // Desviación estándar en Y
                     BORDER_DEFAULT);  // Método de bordes

        // =============================================
        // Detección de bordes con Canny
        // =============================================
        Canny(blurred, edges,
              50,   // Umbral bajo para la detección de bordes
              150,  // Umbral alto para la detección de bordes
              3);   // Apertura del operador de Sobel

        // =============================================
        // Región de interés (ROI)
        // =============================================
        vector<Point> roiVertices = {
            Point(frame.cols * 0.45, frame.rows * 0.6),  // Superior izquierdo
            Point(frame.cols * 0.55, frame.rows * 0.6),  // Superior derecho
            Point(frame.cols,        frame.rows),        // Inferior derecho
            Point(0,                 frame.rows)         // Inferior izquierdo
        };

        fillConvexPoly(laneMask, roiVertices.data(),
                       static_cast<int>(roiVertices.size()), Scalar(255));
        edges.copyTo(roiImage, laneMask);

        // =============================================
        // Transformada de Hough y resultado
        // =============================================
        processLines(frame, roiImage, houghResult, roiVertices);

        // Títulos sin acentos: con el backend Qt de OpenCV 4.6 una ventana con acentos queda negra
        // y sin el tamaño de la imagen.
        imshow("Imagen original", frame);
        imshow("Region de interes (ROI)", roiImage);
        imshow("Lineas detectadas", houghResult);

        // =============================================
        // Salida
        // =============================================
        // Con imagen solo se procesa una vez: cualquier tecla cierra el programa.
        int key = waitKey(useImage ? 0 : 1);
        if (useImage || key == 27 || key == 'q') {
            cout << "Saliendo del programa..." << endl;
            waitKey(1000);  // Espera 1 segundo para que se vea el mensaje
            break;
        }
    }

    return EXIT_SUCCESS;
}
