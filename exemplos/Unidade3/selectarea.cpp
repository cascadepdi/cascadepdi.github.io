#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv; 
int main (int argc, char **argv)
{
    // Read image
    Mat im = imread(argv[1],CV_LOAD_IMAGE_COLOR);    
    // Select ROI
    Rect2d r = selectROI(im);
    cout << r << endl;
    // Crop image
    Mat imCrop = im(r);
    // Display Cropped Image
    imshow("Image", imCrop);
    imwrite(argv[2],imCrop);
    waitKey(0);     
    return 0;
}
