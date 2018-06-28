#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv){
  //Imagem Recebida
  Mat image;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  imshow("image", image);
  waitKey();
  // Nova imagem
  Mat newimage;
  newimage = image.clone();
  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  newimage = newimage(Rect(0, 0, image.cols & -2, image.rows & -2));
  int width = newimage.size().width/2;
  int height = newimage.size().height/2;
  // Pegando os Quadrantes Separadamente
  Mat A, B, C, D;
  A = image(Rect(0,0,height,width));
  B = image(Rect(height,0,height,width));
  C = image(Rect(0,width,height,width));
  D = image(Rect(height,width,height,width));

  A.copyTo(newimage(Rect(height,width,height,width))); // Quadrante 1
  B.copyTo(newimage(Rect(0,width,height,width))); // Quadrante 2
  C.copyTo(newimage(Rect(height,0,height,width))); // Quadrante 3
  D.copyTo(newimage(Rect(0,0,height,width))); // Quadrante 4
  imshow("trocaregioes", newimage);
  waitKey();
  cvDestroyAllWindows();
  return 0;
}
