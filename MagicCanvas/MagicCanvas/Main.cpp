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
	
<<<<<<< HEAD
	// Mac or PC
	if (isMac==true)
	{
		capture.open(1);
	}
	else
	{
		capture.open(0);
	}
=======
	capture.open(isMac);	
>>>>>>> 9ab99b49f65781be89cc111b58813eaaa812db8a

	//safety check
	if (!capture.isOpened())
	{
		cout << "I'm sorry Dave, I'm afraid I can't do that.";
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
	Mat background;

	VideoCapture backgroundCapture;

	if (isMac)
		backgroundCapture.open(1);
	else
		backgroundCapture.open(0);

	// Grab 1 frame and return it
	backgroundCapture >> background;

	return background;
}