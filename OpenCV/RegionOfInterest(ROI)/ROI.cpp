#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    Mat image = imread("avatares.jpg");

    if(image.empty()) {
        cout << "Error: No se pudo cargar la imagen." << endl;
        return -1;
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
    Rect2d r = selectROI(image);  // Permite al usuario seleccionar un rectángulo
    
    // Recortamos la imagen usando la ROI seleccionada
    Mat imCrop = image(r);
    
    // Mostramos y guardamos la imagen recortada
    imshow("ROI Manual", imCrop);
    imwrite("roi_image_target.jpg", imCrop);

    // =============================================
    // 2. ROI Poligonal (Trapecio)
    // =============================================
    /*
     * Aquí definimos una ROI en forma de trapecio usando un polígono de 4 vértices.
     * La máscara se crea con fillConvexPoly y luego se aplica a la imagen original.
     */
    
    // Definimos los vértices del trapecio (coordenadas relativas al tamaño de la imagen)
    vector<Point> roi_vertices;
    roi_vertices.push_back(Point(cols * 0.5, rows * 0.25));  // Vértice superior central
    roi_vertices.push_back(Point(cols * 0.75, rows * 0.5));  // Vértice derecho central
    roi_vertices.push_back(Point(cols * 0.5, rows * 0.75));  // Vértice inferior central
    roi_vertices.push_back(Point(cols * 0.25, rows * 0.5));  // Vértice izquierdo central

    // Creamos una máscara en blanco del mismo tamaño que la imagen
    Mat mask = Mat::zeros(image.size(), CV_8UC1);
    Mat poligonalResult = Mat::zeros(image.size(), image.type());
    
    // Rellenamos el polígono definido en la máscara
    fillConvexPoly(mask, roi_vertices.data(), roi_vertices.size(), Scalar(255));

    // Aplicamos la máscara a la imagen original
    image.copyTo(poligonalResult, mask);

    // Mostramos y guardamos el resultado
    imshow("ROI Poligonal", poligonalResult);
    imwrite("roi_image_Polygon.jpg", poligonalResult);

    // =============================================
    // 3. ROI Rectangular
    // =============================================
    /*
     * Definimos una ROI rectangular usando coordenadas y dimensiones relativas.
     * La máscara se crea con la función rectangle.
     */
    
    Mat rectangularResult = Mat::zeros(image.size(), image.type());
    // Definimos el rectángulo (x_inicio, y_inicio, ancho, alto)
    Rect roi_rect(cols * 0.25, rows * 0.25, cols * 0.5, rows * 0.5);
    
    // Creamos una máscara temporal para el rectángulo
    Mat rectMask = Mat::zeros(image.size(), CV_8UC1);
    rectangle(rectMask, roi_rect, Scalar(255), -1);  // -1 rellena el rectángulo
    
    // Aplicamos la máscara
    image.copyTo(rectangularResult, rectMask);

    // Mostramos y guardamos el resultado
    imshow("ROI Rectangular", rectangularResult);
    imwrite("roi_image_Rectangular.jpg", rectangularResult);

    // =============================================
    // 4. ROI Circular
    // =============================================
    /*
     * Definimos una ROI circular centrada en la imagen con radio relativo al tamaño.
     * La máscara se crea con la función circle.
     */
    
    Mat circularResult = Mat::zeros(image.size(), image.type());
    Point center(cols * 0.5, rows * 0.5);  // Centro de la imagen
    int radius = min(cols, rows) * 0.25;   // Radio como 25% de la dimensión más pequeña
    
    // Creamos una máscara temporal para el círculo
    Mat circleMask = Mat::zeros(image.size(), CV_8UC1);
    circle(circleMask, center, radius, Scalar(255), -1);  // -1 rellena el círculo
    
    // Aplicamos la máscara
    image.copyTo(circularResult, circleMask);

    // Mostramos y guardamos el resultado
    imshow("ROI Circular", circularResult);
    imwrite("roi_image_Circular.jpg", circularResult);

    // Esperamos a que el usuario presione una tecla antes de terminar
    waitKey(0);

    return 0;
}