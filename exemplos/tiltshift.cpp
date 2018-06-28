#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int altura = 0;
int altura_max = 100;

int decaimento = 0;
int decaimento_max = 100;

int vertical = 0;
int vertical_max = 100;

double alpha

Mat image, media_image, image_saida;

char TrackbarName[50];

void show(){
	imshow("tiltshift", image_saida);
	}

void tiltshift(int, void*){
	for (double i = 0; i < image.rows ; i++)
	{
		 alpha = 0.5*(tanh((i-altura)/decaimento) - tanh((i-vertical)/decaimento));
		addWeighted(altura.row(i),alpha, media_image.row(i),1-alpha,0.0,image_saida.row(i));
	}
	show();
	}
int main(int argvc, char** argv){
  Mat mask, mask1;
  float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};
  image = imread("ponte.jpg");
  image.copyTo(image_saida);
  mask = Mat(3, 3, CV_32F, media); 
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  swap(mask, mask1);
  filter2D(image, media_image, image.depth(), mask, Point(1,1), 0);
  namedWindow("tiltshift", 1);
  
  sprintf( TrackbarName, "Altura", altura_max );
  createTrackbar( TrackbarName, "tiltshift",
				  &decaimento,
				  decaimento_max,
				  tiltshift );
  
  sprintf( TrackbarName, "Decaimento", decaimento_max );
  createTrackbar( TrackbarName, "tiltshift",
				  &decaimento,
				  decaimento_max,
				  tiltshift );
  
  sprintf( TrackbarName, "vertical", vertical_max );
  createTrackbar( TrackbarName, "tiltshift",
				  &vertical,
				  vertical_max,
				  tiltshift );
  show();
  waitKey(0);
  return 0;
}
