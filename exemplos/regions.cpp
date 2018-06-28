#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main(int argc, char** argv){
  Mat image,newimage;
  int xi,yi,xf,yf;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  xi = strtol(argv[2], NULL, 10);
  yi = strtol(argv[3], NULL, 10);
  xf = strtol(argv[4], NULL, 10);
  yf = strtol(argv[5], NULL, 10);
  for(int i=xi;i<xf;i++){
    for(int j=yi;j<yf;j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }
  imshow("image", image);
  waitKey();
  return 0;
}
