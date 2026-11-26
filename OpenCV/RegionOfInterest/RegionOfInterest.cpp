/**
 * @file RegionOfInterest.cpp
 *
 * @brief Extrae regiones de interés de avatares.png: una manual con selectROI y tres con máscaras.
 *
 * @author Daniel Ruiz
 * @date Mayo 3, 2025
 */

#include <iostream>              // cerr
#include <vector>                // vector
#include <opencv2/core.hpp>      // Mat, Rect, Point
#include <opencv2/imgproc.hpp>   // fillConvexPoly, rectangle, circle
#include <opencv2/imgcodecs.hpp> // imread, imwrite
#include <opencv2/highgui.hpp>   // selectROI, imshow, waitKey

using namespace std;
using namespace cv;

int main()
{
    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    Mat image = imread("avatares.png", IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // Obtenemos las dimensiones de la imagen para usarlas en las ROI
    int cols = image.cols;  // Ancho de la imagen
    int rows = image.rows;  // Alto de la imagen

    // =============================================
    // 1. Selección manual de ROI usando selectROI
    // =============================================
    /*
     * Esta sección permite al usuario seleccionar manualmente una región de interés
     * rectangular en la imagen usando el ratón.
     */
    Rect2d roi = selectROI(image);  // Permite al usuario seleccionar un rectángulo

    if (roi.empty()) {
        // selectROI devuelve un rectángulo vacío si se cancela (ESC o tecla c)
        cerr << "Aviso: selección cancelada, se omite la ROI manual" << endl;
    } else {
        // Recortamos la imagen usando la ROI seleccionada (vista, sin copia)
        Mat crop = image(roi);

        // Mostramos y guardamos la imagen recortada
        imshow("ROI manual", crop);
        imwrite("roi_image_target.jpg", crop);
    }

    // =============================================
    // 2. ROI poligonal (rombo)
    // =============================================
    /*
     * Aquí definimos una ROI en forma de rombo usando un polígono de 4 vértices.
     * La máscara se crea con fillConvexPoly y luego se aplica a la imagen original.
     */

    // Definimos los vértices del rombo (coordenadas relativas al tamaño de la imagen)
    vector<Point> vertices;
    vertices.push_back(Point(cols * 0.5, rows * 0.25));   // Vértice superior central
    vertices.push_back(Point(cols * 0.75, rows * 0.5));   // Vértice derecho central
    vertices.push_back(Point(cols * 0.5, rows * 0.75));   // Vértice inferior central
    vertices.push_back(Point(cols * 0.25, rows * 0.5));   // Vértice izquierdo central

    // Creamos una máscara en blanco del mismo tamaño que la imagen
    Mat polygonMask = Mat::zeros(image.size(), CV_8UC1);
    Mat polygonResult = Mat::zeros(image.size(), image.type());

    // Rellenamos el polígono definido en la máscara
    fillConvexPoly(polygonMask, vertices.data(), vertices.size(), Scalar(255));

    // Aplicamos la máscara a la imagen original
    image.copyTo(polygonResult, polygonMask);

    // Mostramos y guardamos el resultado
    imshow("ROI poligonal", polygonResult);
    imwrite("roi_image_Polygon.jpg", polygonResult);

    // =============================================
    // 3. ROI rectangular
    // =============================================
    /*
     * Definimos una ROI rectangular usando coordenadas y dimensiones relativas.
     * La máscara se crea con la función rectangle.
     */
    Mat rectResult = Mat::zeros(image.size(), image.type());
    // Definimos el rectángulo (x_inicio, y_inicio, ancho, alto)
    Rect roiRect(cols * 0.25, rows * 0.25, cols * 0.5, rows * 0.5);

    // Creamos una máscara temporal para el rectángulo
    Mat rectMask = Mat::zeros(image.size(), CV_8UC1);
    rectangle(rectMask, roiRect, Scalar(255), -1);  // -1 rellena el rectángulo

    // Aplicamos la máscara
    image.copyTo(rectResult, rectMask);

    // Mostramos y guardamos el resultado
    imshow("ROI rectangular", rectResult);
    imwrite("roi_image_Rectangular.jpg", rectResult);

    // =============================================
    // 4. ROI circular
    // =============================================
    /*
     * Definimos una ROI circular centrada en la imagen con radio relativo al tamaño.
     * La máscara se crea con la función circle.
     */
    Mat circleResult = Mat::zeros(image.size(), image.type());
    Point center(cols * 0.5, rows * 0.5);  // Centro de la imagen
    int radius = min(cols, rows) * 0.25;   // Radio como 25% de la dimensión más pequeña

    // Creamos una máscara temporal para el círculo
    Mat circleMask = Mat::zeros(image.size(), CV_8UC1);
    circle(circleMask, center, radius, Scalar(255), -1);  // -1 rellena el círculo

    // Aplicamos la máscara
    image.copyTo(circleResult, circleMask);

    // Mostramos y guardamos el resultado
    imshow("ROI circular", circleResult);
    imwrite("roi_image_Circular.jpg", circleResult);

    // =============================================
    // Espera
    // =============================================
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
