#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image,grayimage,equalizedgrayimage;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes,planesgray;
  Mat histgray,histequalizegrayimage,hist1,hist2;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgGray(histh, histw, CV_8UC1, Scalar(0));
  Mat histImgEquaGray(histh, histw, CV_8UC1, Scalar(0));

  while(1){
	// Capturando a imagem
    cap >> image;
    // Convertendo a imagem em Cinza
    cvtColor(image,grayimage,CV_BGR2GRAY);
    // Equalizando imagem
    equalizeHist(grayimage,equalizedgrayimage);
    // Calculando os histogramas 
    calcHist(&grayimage, 1, 0, Mat(), hist1, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&equalizedgrayimage, 1, 0, Mat(), hist2, 1,
             &nbins, &histrange,
             uniform, acummulate);
    // Normalizando
    normalize(hist1, hist1, 0, histImgGray.rows, NORM_MINMAX, -1, Mat());
    normalize(hist2, hist2, 0, histImgEquaGray.rows, NORM_MINMAX, -1, Mat());
    histImgGray.setTo(Scalar(0));
    histImgEquaGray.setTo(Scalar(0));
    //
    for(int i=0; i<nbins; i++){
      line(histImgGray,
           Point(i, histh),
           Point(i, histh-cvRound(hist1.at<float>(i))),
           Scalar(255, 255, 255), 1, 8, 0);
      line(histImgEquaGray,
           Point(i, histh),
           Point(i, histh-cvRound(hist2.at<float>(i))),
           Scalar(255, 255, 255), 1, 8, 0);
    }
    histImgGray.copyTo(grayimage(Rect(0, 0, nbins, histh)));
    histImgGray.copyTo(grayimage(Rect(0, 0, nbins, histh)));
    histImgEquaGray.copyTo(equalizedgrayimage(Rect(0, histh, nbins, histh)));

    //Mostrando Imagens
    imshow("Imagem Cinza", grayimage);
    imshow("Imagem Cinza Equalizada", equalizedgrayimage);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
