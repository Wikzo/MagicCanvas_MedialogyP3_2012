#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Fighting Mongooses
// Marco, Simon, Gustav

Mat GetBackground();

int main()
{
	// Fields
	Mat background;
	Mat currentFrame;
	VideoCapture capture;
	bool isMac = 1;
	
	capture.open(isMac);

	//safety check
	if (!capture.isOpened())
	{
		cout << "ERROR - video capture not working";
		return -1;
	}

	// Get background
	background = GetBackground();
	
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

	// TODO:
	// Grab background
	// Grab current video
	// Subtract current video from background
	// Use local threshold and make binary
	// Remove noise with erosion
	// PROFIT!!!
}

Mat GetBackground(bool isMac)
{
	// PC = 0; Mac = 1

	Mat baggrund;

	VideoCapture backgroundCapture;
	backgroundCapture.open(false);

	// Grab 1 frame and return it
	backgroundCapture >> baggrund;

	return baggrund;
}
