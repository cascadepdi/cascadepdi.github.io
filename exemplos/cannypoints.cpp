#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 3

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points;

  int width, height, gray;
  int x, y;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));

  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }
  imshow("pontos.jpg", points);
  
  // Todo o codigo acima foi retirado do algoritmo pontilhismo.cpp
  Mat canny_image;
  int raio;
  int min,max;
  String Etapas[5] = {"Canny - 0","Canny - 1","Canny - 2","Canny - 3","Canny - 4"};
  String Pontos[5] = {"Points - 0","Points - 1","Points - 2","Points - 3","Points - 4"};  
  Mat canny_vectors[5];
  Mat Pontos_passo[5];
  int n_passo = 5;
  for(int i=0; i<n_passo; i++){
     raio = 5-i;
     min = 10*i;
     max = 2*min;
	 Canny(image, canny_image, min, max);
     for(int j=0; j<height; j++ ){
        for(int k=0; k<width; k++){
           if(canny_image.at<uchar>(j,k)>0){
              gray = image.at<uchar>(j,k);
              circle(points, cv::Point(k,j), raio, CV_RGB(gray, gray, gray), -1, CV_AA);
              
           }
        }
     }
     canny_vectors[i] = canny_image.clone();
     Pontos_passo[i] = points.clone();
     imshow(Pontos[i],Pontos_passo[i]);
     imshow(Etapas[i],canny_vectors[i]);
  }
  
 
  waitKey(0);
  return 0;
}
