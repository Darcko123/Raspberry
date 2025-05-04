#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// =============================================
// Variables globales y configuración
// =============================================

// Valores iniciales para brillo y contraste (rango 0-200, 100 = valor neutral)
int _brightness = 100;  // Brillo (100 = neutral)
int _contrast = 100;    // Contraste (100 = neutral)

// Almacenamiento para la imagen original y el histograma
Mat image;    // Imagen original en escala de grises
Mat hist;     // Matriz para almacenar el histograma

// =============================================
// Función de callback para actualizar la imagen
// =============================================
/*
 * Esta función se ejecuta cada vez que se mueven los trackbars
 * Actualiza la imagen con los nuevos valores de brillo/contraste
 * y recalcula el histograma
 */
static void updateBrightnessContrast(int, void*) {
    // Ajustamos los valores para que 100 sea neutral (0 en la fórmula)
    int brightness = _brightness - 100;  // Rango: -100 a +100
    int contrast = _contrast - 100;      // Rango: -100 a +100

    // =============================================
    // Cálculo de parámetros para la transformación
    // =============================================
    /*
     * Fórmula de transformación lineal: dst = a*src + b
     * Basada en el trabajo de Werner D. Streidt
     * 
     * Para contraste positivo (>100):
     *   - Comprime el rango alrededor del centro (128)
     * Para contraste negativo (<100):
     *   - Expande el rango desde el centro
     */
    double a, b;  // Parámetros de la transformación lineal
    
    if (contrast > 0) {
        // Caso de contraste aumentado
        double delta = 127. * contrast / 100;
        a = 255. / (255. - delta * 2);
        b = a * (brightness - delta);
    } else {
        // Caso de contraste reducido
        double delta = -128. * contrast / 100;
        a = (256. - delta * 2) / 255.;
        b = a * brightness + delta;
    }

    // =============================================
    // Aplicar transformación de brillo/contraste
    // =============================================
    Mat dst;
    image.convertTo(dst, CV_8U, a, b);  // dst = a*image + b
    imshow("Imagen Original", dst);

    // =============================================
    // Cálculo y visualización del histograma
    // =============================================
    
    // Configuración del histograma
    const int histSize = 256;      // Número de bins (256 para imágenes de 8 bits)
    const float range[] = {0, 256}; // Rango de valores de píxeles
    const float* histRange = {range};
    const int hist_h = 400;        // Altura de la imagen del histograma
    const int hist_w = 512;        // Ancho de la imagen del histograma

    // Calcular histograma
    calcHist(&dst,           // Imagen de entrada
             1,              // Número de imágenes
             0,              // Canales a procesar (0 para escala de grises)
             Mat(),          // Máscara (ninguna)
             hist,           // Matriz de salida para el histograma
             1,             // Dimensión del histograma
             &histSize,     // Tamaño del histograma
             &histRange,    // Rango de valores
             true,          // Histograma uniforme
             false);        // No acumular

    // Normalizar histograma para que quepa en la imagen de visualización
    normalize(hist, hist, 0, hist_h, NORM_MINMAX, -1, Mat());

    // Crear imagen para visualizar el histograma
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));  // Imagen negra

    // Dibujar el histograma como líneas conectadas
    for (int i = 1; i < histSize; i++) {
        line(histImage, 
             Point((i-1) * hist_w / histSize, hist_h - cvRound(hist.at<float>(i-1))),
             Point(i * hist_w / histSize, hist_h - cvRound(hist.at<float>(i))),
             Scalar(255, 0, 0),  // Color azul (BGR)
             2,                  // Grosor de línea
             8,                  // Tipo de línea
             0);                 // Desplazamiento
    }

    // Mostrar el histograma
    imshow("Histograma", histImage);
}

// =============================================
// Función principal
// =============================================
int main() {
    // Cargar imagen en escala de grises
    image = imread("avatares.jpg", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen." << endl;
        return EXIT_FAILURE;
    }

    // Crear ventanas para la imagen y el histograma
    namedWindow("Imagen Original", WINDOW_AUTOSIZE);
    namedWindow("Histograma", WINDOW_AUTOSIZE);

    // Crear trackbars para ajustar brillo y contraste
    createTrackbar("Brillo", "Imagen Original", &_brightness, 200, updateBrightnessContrast);
    createTrackbar("Contraste", "Imagen Original", &_contrast, 200, updateBrightnessContrast);

    // Inicializar la visualización
    updateBrightnessContrast(0, 0);

    // Esperar a que el usuario presione una tecla para salir
    waitKey(0);
    
    return EXIT_SUCCESS;
}