/**
 * @file HistogramBrightnessContrast.cpp
 *
 * @brief Ajusta el brillo y el contraste de avatares.png con trackbars y dibuja su histograma.
 *
 * @author Daniel Ruiz
 * @date Mayo 3, 2025
 */

#include <iostream>              // cerr
#include <opencv2/core.hpp>      // Mat, normalize
#include <opencv2/imgproc.hpp>   // calcHist, line
#include <opencv2/imgcodecs.hpp> // imread
#include <opencv2/highgui.hpp>   // namedWindow, createTrackbar, imshow, waitKey

using namespace std;
using namespace cv;

// =============================================
// Variables globales
// =============================================
// El callback de los trackbars no recibe datos propios, así que el estado es global.

// Posición de los trackbars (rango 0-200, 100 = valor neutral)
int brightnessTrackbar = 100;  // Brillo
int contrastTrackbar = 100;    // Contraste

Mat image;  // Imagen original en escala de grises
Mat hist;   // Matriz para almacenar el histograma

// =============================================
// Callback de los trackbars
// =============================================
/*
 * Esta función se ejecuta cada vez que se mueven los trackbars.
 * Actualiza la imagen con los nuevos valores de brillo/contraste
 * y recalcula el histograma.
 */
static void updateBrightnessContrast(int, void*)
{
    // Ajustamos los valores para que 100 sea neutral (0 en la fórmula)
    int brightness = brightnessTrackbar - 100;  // Rango: -100 a +100
    int contrast = contrastTrackbar - 100;      // Rango: -100 a +100

    // Cálculo de parámetros para la transformación
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

    // Aplicar transformación de brillo/contraste
    Mat dst;
    image.convertTo(dst, CV_8U, a, b);  // dst = a*image + b
    imshow("Imagen original", dst);

    // Configuración del histograma
    const int histSize = 256;        // Número de bins (256 para imágenes de 8 bits)
    const float range[] = {0, 256};  // Rango de valores de píxeles
    const float* histRange = {range};
    const int histHeight = 400;      // Altura de la imagen del histograma
    const int histWidth = 512;       // Ancho de la imagen del histograma

    // Calcular histograma
    calcHist(&dst,       // Imagen de entrada
             1,          // Número de imágenes
             0,          // Canales a procesar (0 para escala de grises)
             Mat(),      // Máscara (ninguna)
             hist,       // Matriz de salida para el histograma
             1,          // Dimensión del histograma
             &histSize,  // Tamaño del histograma
             &histRange, // Rango de valores
             true,       // Histograma uniforme
             false);     // No acumular

    // Normalizar histograma para que quepa en la imagen de visualización
    normalize(hist, hist, 0, histHeight, NORM_MINMAX, -1, Mat());

    // Crear imagen para visualizar el histograma
    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));  // Imagen negra

    // Dibujar el histograma como líneas conectadas
    for (int i = 1; i < histSize; i++) {
        line(histImage,
             Point((i - 1) * histWidth / histSize, histHeight - cvRound(hist.at<float>(i - 1))),
             Point(i * histWidth / histSize, histHeight - cvRound(hist.at<float>(i))),
             Scalar(255, 0, 0),  // Color azul (BGR)
             2,                  // Grosor de línea
             8,                  // Tipo de línea
             0);                 // Desplazamiento
    }

    imshow("Histograma", histImage);
}

int main()
{
    // =============================================
    // Carga y verificación de la imagen
    // =============================================
    image = imread("avatares.png", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Error: No se pudo cargar la imagen 'avatares.png'" << endl;
        return EXIT_FAILURE;
    }

    // =============================================
    // Configuración de la interfaz
    // =============================================
    namedWindow("Imagen original", WINDOW_AUTOSIZE);
    namedWindow("Histograma", WINDOW_AUTOSIZE);

    createTrackbar("Brillo", "Imagen original", &brightnessTrackbar, 200, updateBrightnessContrast);
    createTrackbar("Contraste", "Imagen original", &contrastTrackbar, 200, updateBrightnessContrast);

    // =============================================
    // Visualización
    // =============================================
    updateBrightnessContrast(0, nullptr);  // Primer dibujo, antes de mover los trackbars
    waitKey(0);  // Cualquier tecla cierra el programa

    return EXIT_SUCCESS;
}
