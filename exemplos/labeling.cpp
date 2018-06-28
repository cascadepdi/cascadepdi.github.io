#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects,nobjectswithholes;
  
  CvPoint p;
  p.x=0;
  p.y=0;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;
  // Retirando objetos ligados a borda
  for(int i=0; i<height; i++) { 
	if(image.at<uchar>(i,0)==255){ 
		p.x = 0, p.y = i;
        }
	if(image.at<uchar>(i,width-1)==255){
		p.x = width - 1; p.y = i;
	}

	floodFill(image,p,0);
  }
  for(int j=0; j<width; j++) { 
	if(image.at<uchar>(0,j)==255){ 
		p.x = j, p.y = 0;
        }
	if(image.at<uchar>(height-1,j)==255){
		p.x = j; p.y = height-1;
	}
	floodFill(image,p,0);
  }
  //Atribuindo um tom de cinza para a cor de fundo
  p.x = 0; p.y = 0;
  floodFill(image,p,127);
   //
   imshow("Bordas Eliminadas", image);  

  // busca de objetos 
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		nobjects++;
		p.x = j; p.y = i;
		floodFill(image,p,254);
	  }
	}
  }
  //Busca de Objetos com Buracos
  nobjectswithholes=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if((image.at<uchar>(i,j) == 254) and (image.at<uchar>(i,j+1) == 0)){
		nobjectswithholes++;
		p.x = j+1; p.y = i;
		floodFill(image,p,253);
	  }
	}
  }
  //Filtrando os buracos
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if((image.at<uchar>(i,j) == 254)){
		p.x = j; p.y = i;
		floodFill(image,p,127);
	  }
	}
  }
  std::cout << "Numero de Objetos: " << nobjects << "\n";
  std::cout << "Objetos com Buracos: " << nobjectswithholes << "\n";
  imshow("Buracos", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
