#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


// Fighting Mongooses
// Marco, Simon, Gustav

int main()
{
    Mat im = imread("nisse.jpg");
    if (im.empty()) 
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }
    imshow("Image", im);
    waitKey(0);

}