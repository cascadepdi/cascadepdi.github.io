/**
  @file videowriter_basic.cpp
  @brief A very basic sample for using VideoWriter and VideoCapture
  @author PkLab.net
  @date Aug 24, 2016
*/
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat detectAndDisplay( Mat frame );

int main(int, char**)
{
    Mat src;
    // use default camera as video source
    VideoCapture cap(0);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    // get one frame from camera to know frame size and type
    cap >> src;
    // check if we succeeded
    if (src.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }
    bool isColor = (src.type() == CV_8UC3);

    //--- INITIALIZE VIDEOWRITER
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = 5.0;                          // framerate of the created video stream
    string filename = "./live3.avi";             // name of the output video file
    writer.open(filename, codec, fps, src.size(), isColor);
    // check if we succeeded
    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return -1;
    }

    //--- GRAB AND WRITE LOOP
    cout << "Writing videofile: " << filename << endl
         << "Press any key to terminate" << endl;
    for (;;)
    {
        // check if we succeeded
        if (!cap.read(src)) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        src = detectAndDisplay(src);
        // encode the frame into the videofile stream
        writer.write(src);
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", src);
        if (waitKey(5) >= 0)
            break;
    }
    // the videofile will be closed and released automatically in VideoWriter destructor
    return 0;
}

Mat detectAndDisplay( Mat frame )
{
	CascadeClassifier coke_cascade;
	CascadeClassifier hand_cascade;
    coke_cascade.load("cascade.xml");
    hand_cascade.load("hand2cascade.xml");
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    std::vector<Rect> cokes;
    std::vector<Rect> hands;
    coke_cascade.detectMultiScale( frame_gray, cokes, 1.030, 3);
    hand_cascade.detectMultiScale( frame_gray, hands, 9.3, 16);
    for ( size_t i = 0; i < cokes.size(); i++ )
    {
		Rect coke_i = cokes[i];
        Point center( cokes[i].x + cokes[i].width/2, cokes[i].y + cokes[i].height/2 );
        rectangle(frame, coke_i, CV_RGB(0, 255,0), 1);
        //ellipse( frame, center, Size( cokes[i].width/2, cokes[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_gray( cokes[i] );    
        //imwrite( "Saida_Webcam.jpg", frame );
    }
    for ( size_t i = 0; i < hands.size(); i++ )
    {
		Rect hand_i = hands[i];
        Point center( hands[i].x + hands[i].width/2, hands[i].y + hands[i].height/2 );
        rectangle(frame, hand_i, CV_RGB(0, 255,0), 1);
        //ellipse( frame, center, Size( cokes[i].width/2, cokes[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_gray( hands[i] );    
        //imwrite( "Saida_Webcam.jpg", frame );
    }
    return frame;
}
