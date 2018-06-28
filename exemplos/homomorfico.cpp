#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define RADIUS 20

// Esse código foi escrito em cima do original (dft.cpp). Caso queira apenas os trechos modificados,
// aperte CRTL+F e busque por "código novo".

// Procedimento do filtro:
// 1 - Aplica logaritmo na imagem;
// 2 - Aplica transformada de fourier;
// 3 - Inverte os quadrantes
// 4 - Aplica o filtro homomórfico
// 5 - Inverte os quadrantes novamente
// 6 - Aplica transformada inversa
// 7 - Aplica exponencial
// 8 - Mostra imagem

using namespace cv;
using namespace std;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;
  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));
  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);
  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}


int main(int , char**){
  char *window_name = "janela"; // Nome da janela de exibição
  VideoCapture cap;
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  /** CÓDIGO NOVO **/

	  // Prepara a janela para exibição
	  namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	  // Valores iniciais dos parâmetros do filtro homomórfico (usados nas trackbars)
	  float d; int y_H, y_H_max=20, y_L, y_L_max=20, d0=5, d0_max=50, c, c_max=20;

	  // Cria os trackbars para o ajuste manual do filtro homomórfico
	  createTrackbar("Gama H: ",window_name, &y_H, y_H_max); createTrackbar("Gama L: ",window_name, &y_L, y_L_max);
	  createTrackbar("D0: ",window_name, &d0, d0_max); createTrackbar("c: ",window_name, &c, c_max);

  /** CÓDIGO NOVO **/

  // habilita/desabilita ruido
  int noise=0;
  // frequencia do ruido
  int freq=10;
  // ganho inicial do ruido
  float gain=1;
  // valor do ruido
  float mean;
  // guarda tecla capturada
  char key;
  // valores ideais dos tamanhos da imagem para calculo da DFT
  int dft_M, dft_N;
  // abre a câmera default
  cap.open(0);
  if(!cap.isOpened())
    return -1;
  // captura uma imagem para recuperar as informacoes de gravação
  cap >> image;
  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);
  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));
  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());
  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));
  // a função de transferência (filtro frequencial) deve ter o mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();
  // cria uma matriz temporária para criar as componentes real e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);
  // cria a matriz com as componentes do filtro e junta ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  for(;;){
    cap >> image;

    /** CÓDIGO NOVO **/

	    // Calcula o filtro homomórfico
	    for(int i=0; i<dft_M; i++){
	       for(int j=0; j<dft_N; j++){
			d = pow(pow(i - dft_M/2, 2) + pow(j - dft_N/2, 2), 0.5);
			tmp.at<float>(i,j)  = (y_H - y_L)*(1 - exp(-1*c*(pow(d,2)/pow(d0,2)))) + y_L; // Fórmula
	       }
	    }

	    // Salva o filtro homomórfico
	    comps[0] = tmp; comps[1] = comps[0]; merge(comps, 2, filter);

    /** CÓDIGO NOVO **/

    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("Vídeo Original", imagegray);
    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));
    // limpa o array de matrizes que vao compor a imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);
    // combina o array de matrizes em uma unica componente complexa
    merge(planos, complexImage);

    /** CÓDIGO NOVO **/

	    // Aplicação do logaritmo antes da transformada
	    for (int i = 0; i < dft_M; i++){
	      for (int j = 0; j  < dft_N;j++){
		complexImage.at<char>(i,j) = log(complexImage.at<char>(i,j)+1); // +1 pq log(0) não existe!
	      }
	    }

    /** CÓDIGO NOVO **/

    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);
    // aplica o filtro homomórfico
    mulSpectrums(complexImage,filter,complexImage,0);
    // limpa o array de planos
    planos.clear();
    // separa as partes real e imaginaria para modifica-las
    split(complexImage, planos);
    // usa o valor medio do espectro para dosar o ruido
    mean = abs(planos[0].at<float> (dft_M/2,dft_N/2));
    // insere ruido coerente, se habilitado
    if(noise){
      // F(u,v) recebe ganho proporcional a F(0,0)
      planos[0].at<float>(dft_M/2 +freq, dft_N/2 +freq) +=
        gain*mean;
      planos[1].at<float>(dft_M/2 +freq, dft_N/2 +freq) +=
        gain*mean;
      // F*(-u,-v) = F(u,v)
      planos[0].at<float>(dft_M/2 -freq, dft_N/2 -freq) =
        planos[0].at<float>(dft_M/2 +freq, dft_N/2 +freq);
      planos[1].at<float>(dft_M/2 -freq, dft_N/2 -freq) =
        -planos[1].at<float>(dft_M/2 +freq, dft_N/2 +freq);
    }
    // recompoe os planos em uma unica matriz complexa
    merge(planos, complexImage);
    // troca novamente os quadrantes
    deslocaDFT(complexImage);
    // calcula a DFT inversa
    idft(complexImage, complexImage);

    /** CÓDIGO NOVO **/

	    // Aplicação da exponencial após a transformada inversa
	    for (int i = 0; i < dft_M; i++){
		  for (int j = 0; j  < dft_N;j++){
		    complexImage.at<char>(i,j) = exp((complexImage.at<char>(i,j)));
		  }
	    }

    /** CÓDIGO NOVO **/

    // limpa o array de planos
    planos.clear();
    // separa as partes real e imaginaria da imagem filtrada
    split(complexImage, planos);
    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow(window_name, planos[0]);
    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
    switch(key){
      // aumenta a frequencia do ruido
    case 'q':
      freq=freq+1;
      if(freq > dft_M/2-1)
        freq = dft_M/2-1;
      break;
      // diminui a frequencia do ruido
    case 'a':
      freq=freq-1;
      if(freq < 1)
        freq = 1;
      break;
      // amplifica o ruido
    case 'x':
      gain += 0.1;
      break;
      // atenua o ruido
    case 'z':
      gain -= 0.1;
      if(gain < 0)
        gain=0;
      break;
      // insere/remove ruido
    case 'e':
      noise=!noise;
      break;
    }
  }
  return 0;
}
