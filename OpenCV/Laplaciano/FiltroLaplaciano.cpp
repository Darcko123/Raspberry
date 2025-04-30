#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    // Declare the variables we are going to use
    Mat imagenOriginal, GRAYImage, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    const char* window_name = "Laplace Demo";
	
    //Cargar imagen
    const char* imageName = argc >=2 ? argv[1] : "avatares.jpg";
    imagenOriginal = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Load an image
    // Check if image is loaded fine
    if(imagenOriginal.empty())
    {
        return -1;
    }

    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur( imagenOriginal, imagenOriginal, Size(3, 3), 0, 0, BORDER_DEFAULT );
    cvtColor( imagenOriginal, GRAYImage, COLOR_BGR2GRAY ); // Convert the image to grayscale
    Mat imageLaplaciano;
    Laplacian( GRAYImage, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
    
    // converting back to CV_8U
    convertScaleAbs( dst, imageLaplaciano );
    imshow( window_name, imageLaplaciano );
    waitKey(0);
    return 0;
}