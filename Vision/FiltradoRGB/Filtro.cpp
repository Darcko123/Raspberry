#include <opencv2/opencv.hpp>      // Biblioteca principal de OpenCV
#include <opencv2/imgcodecs.hpp>   // Funciones para manejo de imágenes
#include <opencv2/highgui.hpp>     // Funciones para interfaz gráfica (ventanas)
#include <iostream>                // Entrada y salida estándar
#include <string>                  // Manejo de cadenas

using namespace std;              // Espacio de nombres estándar
using namespace cv;               // Espacio de nombres de OpenCV

int main(int argc, char** argv)
{
    // Cargar imagen desde archivo (modificar la ruta si es necesario)
    Mat image = imread("avatares.jpg", IMREAD_COLOR);

    // Verificar que la imagen se haya cargado correctamente
    if (image.empty()) {
        cerr << "Error: No se pudo abrir o encontrar la imagen." << endl;
        return -1;
    }

    // Crear matrices vacías (inicializadas en negro) para cada componente de color
    Mat rojo  = Mat::zeros(image.size(), image.type());
    Mat verde = Mat::zeros(image.size(), image.type());
    Mat azul  = Mat::zeros(image.size(), image.type());

    // Recorrer cada píxel de la imagen original
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {

            // Obtener el valor del píxel actual
            Vec3b pixel = image.at<Vec3b>(y, x);

            uchar b = pixel[0]; // Componente azul
            uchar g = pixel[1]; // Componente verde
            uchar r = pixel[2]; // Componente rojo

            // Comparar componentes para encontrar el dominante
            if (r > g && r > b) {
                // Si rojo es dominante, guardar solo el valor del canal rojo
                rojo.at<Vec3b>(y, x) = Vec3b(0, 0, r);
            }
            else if (g > r && g > b) {
                // Si verde es dominante, guardar solo el valor del canal verde
                verde.at<Vec3b>(y, x) = Vec3b(0, g, 0);
            }
            else if (b > r && b > g) {
                // Si azul es dominante, guardar solo el valor del canal azul
                azul.at<Vec3b>(y, x) = Vec3b(b, 0, 0);
            }
            else {
                // Si no hay un color claramente dominante (ej. gris), se deja en negro
                // Esto evita saturar la imagen con falsos positivos
                rojo.at<Vec3b>(y, x)  = Vec3b(0, 0, 0);
                verde.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
                azul.at<Vec3b>(y, x)  = Vec3b(0, 0, 0);
            }
        }
    }

    // Mostrar las imágenes generadas
    imshow("Imagen original", image);
    imshow("Canal Rojo", rojo);
    imshow("Canal Verde", verde);
    imshow("Canal Azul", azul);

    // Esperar indefinidamente hasta que se presione una tecla
    waitKey(0);

    return 0;
}
