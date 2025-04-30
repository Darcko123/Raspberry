#include <iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std; 
using namespace cv; 

int main() 
{ 
    //imread puede leer “BMP, DIB, JPEG, JPE, PNG, PBM, PGM, PPM, SR, RAS, and TIFF”
    Mat img = imread("avatares.jpg", IMREAD_COLOR);

    //Verificamos existencia de imagen
    if(img.empty())
    {
        return -1;
    }
    // Muestra imagen
    imshow("Imagen avatares", img);

    //Presiona cualquier tecla para salir
    waitKey(0);

    return 0;
} 