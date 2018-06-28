#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argvc, char** argv){
  Mat image_all, image_part,result;
  image_all = imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  image_part = imread("areabiel.png",CV_LOAD_IMAGE_GRAYSCALE);
  matchTemplate(image_all,image_part,result,CV_TM_CCORR);
  int menor = 1000;
  CvPoint p;
  CvPoint p2;
  for(int i=0;i<result.size().width;i++){
    for(int j=10;j<result.size().width;j++){
      if(result.at<uchar>(i,j)< menor);
      menor = result.at<uchar>(i,j);
      p.x = i;
      p.y = j;
    }
  }
  p.x = p.x-10;
  p.y = p.x-10;
  p2.x = p.x+20;
  p2.y = p.x+20;
  //matchTemplate(image_all,image_part,result,CV_TM_CCORR_NORMED);
  //int width = image_all.size().width - image_part.size().width + 1;
  //int height = image_all.size().height - image_part.size().height + 1;
  //result = cvCreateImage(cvSize(width,height),32,1);
  normalize(result,result,1,0,CV_MINMAX);
  imshow("Resultado",result);
  waitKey(0);
  return 0;
}
