#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat im = imread("nisse.jpg");
    if (im.empty()) 
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }
    imshow("Image", im);

<<<<<<< HEAD
	// Simon
=======
	// gustav

>>>>>>> 2b66d0417e8b173e4c85499a16a6d46502a9c617

    waitKey(0);
}