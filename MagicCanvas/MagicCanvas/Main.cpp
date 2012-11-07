#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Fighting Mongooses
// Marco, Simon, Gustav

// Step 1: Save reference image (background)
// Step 2: Capture current image (video)
// Step 3: Perform image subtraction
// Step 4: Threshold (local)
// Step 5: Noise reduction - Mean filter or morphological opening/closing

// Function prototypes
void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo);
Mat PerformImageSubstraction(Mat currentFrame, Mat background);
Mat Threshold(int threshold);
Mat MeanFilter(Mat image, int threshold);

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

		// Convert to grayscale
		cvtColor(currentFrame, currentFrame, CV_RGB2GRAY);

		// Exit
		if ((char)waitKey(30) == 'q')
			break;

		// Grab new background screenshot
		if ((char)waitKey(1) == 's')
			GetBackground(capture, background);

		// Removing background from output
		substraction = PerformImageSubstraction(currentFrame, imread("background.png"));


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
Mat PerformImageSubstraction(Mat currentFrame, Mat background)
{
	Mat substraction = (background - currentFrame);
	Mat GrayScale_substraction;
	
	return substraction;
}

Mat Threshold(Mat image, int threshold)
{
	// TODO: Make local threshold (p. 125 in book)
	// Use on binary image only

	// Loop through all pixels and set them to either black (0) or white (255) using a threshold
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			int value = 0;
			if (image.at<uchar>(y, x) < 0)
				value = image.at<uchar>(y, x) * -1; // Take absolute value
			else
				value = image.at<uchar>(y, x);

			if (value >= threshold)
				image.at<uchar>(y, x) = 255;
			else
				image.at<uchar>(y, x) = 0;

		}
	}

	return image;
}

Mat MeanFilter(Mat input)
{
	// 5x5 kernel

	// Make a temporary clone of the input image
	Mat mean = input.clone();

	// Loop through all pixels
	for (int y = 0; y < input.rows-2; y++)
	{
		for (int x = 0; x < input.cols-2; x++)
		{
			if (x - 2 < 0 || y - 2 < 0) // don't go out of bounds
				continue;

			// Apply the kernel
			mean.at<uchar>(y, x) = (
				input.at<uchar>(y-2, x-2) + input.at<uchar>(y-2, x-1)
				+ input.at<uchar>(y-2, x) + input.at<uchar>(y-2, x+1)
				+ input.at<uchar>(y-2, x+2) + input.at<uchar>(y-1, x-2)
				+ input.at<uchar>(y-1, x-1) + input.at<uchar>(y-1, x)
				+ input.at<uchar>(y-1, x+1) + input.at<uchar>(y-1, x+2)
				+ input.at<uchar>(y, x-2) + input.at<uchar>(y, x-1)
				+ input.at<uchar>(y, x) + input.at<uchar>(y, x+1)
				+ input.at<uchar>(y, x+2) + input.at<uchar>(y+1, x-2)
				+ input.at<uchar>(y+1, x-1) + input.at<uchar>(y+1, x)
				+ input.at<uchar>(y+1, x+1) + input.at<uchar>(y+1, x+2)
				+ input.at<uchar>(y+2, x-2)	+ input.at<uchar>(y+2, x-1)
				+ input.at<uchar>(y+2, x) + input.at<uchar>(y+2, x+1)
				+ input.at<uchar>(y+2, x+2)
				) / 25;
		}
	}

	return mean;
}
