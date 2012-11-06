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
    waitKey(0);

	// Fighting FTW!
	// Simons comment!
<<<<<<< HEAD

	// Now I have invaded your branch... muhahahaha!!!
=======
	// Hej Gustav
	// Hej Simon
>>>>>>> 0c866ed8024339ed38a6af9dd84f6d8561d72248
}