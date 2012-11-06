#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Fighting Mongooses
// Marco, Simon, Gustav

int main()
{
	VideoCapture capture;
	capture.open(1);

	if (!capture.isOpened())
	{
		cout << "I'm sorry Dave, I can't do that.";
		return -1;
	}

	Mat frame;
	while(true)
	{
		capture >> frame;
		if (frame.empty())
			break;

		if ((char)waitKey(30) == 'q')
			break;

		imshow("Video", frame);
	}

	return 0;

	// TODO:
	// Grab background
	// Grab current video
	// Subtract current video from background
	// Use local threshold and make binary
	// Remove noise with erosion
	// PROFIT!!!
}