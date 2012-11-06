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
void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo);

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
	GetBackground(capture, background);

	// Grab current frame
	while(true)
	{
		capture >> currentFrame;
		if (currentFrame.empty())
			break;

		// Exit
		if ((char)waitKey(30) == 'q')
			break;

		// Grab new background screenshot
		if ((char)waitKey(30) == 's')
			GetBackground(capture, background);


		imshow("Background", background);
		imshow("Video", currentFrame);
	}

	return 0;
}

void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo)
{
	// Grab 1 frame and return it as the background
	Mat tempBackground;
	
	capture >> tempBackground;
	imwrite("background.png", tempBackground);
	backgroundToWriteTo = imread("background.png");
}
