/**
 * @file HelloWorld.cpp
 *
 * @brief Comprueba la instalación de OpenCV imprimiendo su versión.
 *
 * @author Daniel Ruiz
 * @date Abril 29, 2025
 */

#include <iostream>              // cout
#include <opencv2/core.hpp>      // CV_VERSION

using namespace std;
using namespace cv;

int main()
{
    cout << "OpenCV version: " << CV_VERSION << endl;
    return EXIT_SUCCESS;
}
