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
<<<<<<< HEAD
	//max 
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
	//Max you forgot to bring cake today.. :(
=======
>>>>>>> 21e8d31015497b7b6f5d53b59aff8af2f78d4e91
	// Simon
>>>>>>> af2a2f1e73a74d1b6b5770f084d2f7616204453b
=======
	// gustav

>>>>>>> 2b66d0417e8b173e4c85499a16a6d46502a9c617
>>>>>>> af2a2f1e73a74d1b6b5770f084d2f7616204453b

>>>>>>> 2b66d0417e8b173e4c85499a16a6d46502a9c617
    waitKey(0);
}