#include <opencv2/opencv.hpp>

using namespace cv;

void imfilter( const cv::Mat & image ) {

  // Creando las ventanas
  namedWindow( "Imagen Original", WINDOW_AUTOSIZE );
  namedWindow( "Imagen Filtrada", WINDOW_AUTOSIZE );

  // Muestra imagen original//
  imshow( "Imagen Original", image );

  // Variable para imagen de salida
  Mat out;

  // Filtro Gaussiano 
  GaussianBlur( image, out, Size(3,3), 3, 3);
  GaussianBlur(   out, out, Size(3,3), 3, 3);

  // Muestra imagen filtrada
  imshow( "Imagen Filtrada", out );

  // Presiona una tecla para salir
  waitKey( 0 );

  destroyWindow( "Imagen Original" );
  destroyWindow( "Imagen Filtrada" );

}

int main( int argc, char** argv ){
  //imread puede leer “BMP, DIB, JPEG, JPE, PNG, PBM, PGM, PPM, SR, RAS, and TIFF”
  Mat img = imread("avatares.jpg", IMREAD_COLOR); 

  if( img.empty() ) 
    return -1;
  
 
  imfilter( img );


  return 0;
}