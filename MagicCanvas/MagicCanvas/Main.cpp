#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Fighting Mongooses
// Marco, Simon, Gustav

// TODO:
// Grab background
// Grab current video
// Subtract current video from background
// Use local threshold and make binary
// Remove noise with erosion
// PROFIT!!!

// Function prototypes
Mat GetBackground(VideoCapture capture);

int main()
{
	// Fields
	Mat background;
	Mat currentFrame;
	VideoCapture capture;
	bool isMac = 0;
	
	capture.open(isMac);

	//safety check
	if (!capture.isOpened())
	{
		cout << "ERROR - video capture not working";
		return -1;
	}

	// Get background
	//background = GetBackground(isMac);

	// Grab current frame
	while(true)
	{
		capture >> currentFrame;
		if (currentFrame.empty())
			break;

		if ((char)waitKey(30) == 'q')
			break;

		imshow("Video", currentFrame);
	}

	return 0;
}

Mat GetBackground(VideoCapture capture)
{
	Mat background;
	// Grab 1 frame and return it as the background
	capture >> background;

	return background;
}
