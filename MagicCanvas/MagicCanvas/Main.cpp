#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
Mat BackgroundSubstracted(Mat currentFrame, Mat background);

int main()
{
	// Fields
	Mat background;
	Mat currentFrame;
	Mat substraction;
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
		if ((char)waitKey(1) == 's')
			GetBackground(capture, background);

		// Removing background from output
		substraction = BackgroundSubstracted(currentFrame, imread("background.png"));


		imshow("Background", background);
		imshow("Video", currentFrame);
		imshow("Background substraction", substraction);
	}
	
	return 0;
}

void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo)
{
	// Grab 1 frame and return it as the background
	Mat tempBackground;
	
	capture >> tempBackground;
	cvtColor(tempBackground, tempBackground, CV_RGB2GRAY);
	imwrite("background.png", tempBackground);
	backgroundToWriteTo = imread("background.png");
}

	// Function to substract background 			   
Mat BackgroundSubstracted(Mat currentFrame, Mat background)
{
	Mat substraction = (background - currentFrame);
	Mat GrayScale_substraction;
	
	return substraction;
}
