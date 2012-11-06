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

	// Team1
<<<<<<< HEAD
	// Marta's comment
=======
	//Max 
	
	
	
	// HAHAHAH NUUUUBS! :D
>>>>>>> 26e11a39b4edef65751b1e2ccc32d1d5f58e025e
}