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
Mat PerformImageSubstraction(Mat currentFrame, Mat background, int threshold);
Mat Threshold(Mat image, int threshold);
Mat MeanFilter(Mat input);

int main()
{
	// Fields
	Mat background;
	Mat currentFrame;
	Mat substraction;
<<<<<<< HEAD
	Mat MedianValue;
=======

	/*// Convert to grayscale - DOES NOT WORK
	cvtColor(background, background, CV_RGB2GRAY);
	cvtColor(currentFrame, currentFrame, CV_RGB2GRAY);
	cvtColor(substraction, substraction, CV_RGB2GRAY);*/

>>>>>>> 8cc480287a6941f09653d0cff116b505ab5cb27e
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

		// Convert current frame to grayscale
		cvtColor(currentFrame, currentFrame, CV_RGB2GRAY);

		// Substract background from current frame
		substraction = PerformImageSubstraction(currentFrame, background, 100);

		// Mean filter (should be replaced with MEAN filter later)
		//substraction = MeanFilter(substraction);
		
		// Median blur (built-in function)
		cv::medianBlur(substraction, substraction, 3);


		// -------- DEBUG FEATURES --------------
		// Exit
		if ((char)waitKey(30) == 'q')
			break;

		// Grab new background screenshot
		if ((char)waitKey(1) == 's')
			GetBackground(capture, background);

		// -------- DEBUG FEATURES --------------

		imshow("Background", background);
		imshow("Video", currentFrame);
		imshow("Background substraction", substraction);
	}
	
	return 0;
}

void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo)
{
	// Grab 1 frame and return it as the background
	// Maybe optimize so not neccessary to save image as png

	// TODO: Image differencing (taking the last frame and calculate average, "learning") [book p. 125]
	// Example: http://stackoverflow.com/questions/7765877/background-subtraction-in-opencvc

	Mat tempBackground;
	
	capture >> tempBackground;
	
	cvtColor(tempBackground, tempBackground, CV_RGB2GRAY);
	imwrite("background.png", tempBackground);
	backgroundToWriteTo = imread("background.png");

	// Make grayscale
	cvtColor(backgroundToWriteTo, backgroundToWriteTo, CV_RGB2GRAY);
}

// Function to substract background 			   
Mat PerformImageSubstraction(Mat currentFrame, Mat background, int threshold)
{
	// Old way
	/*Mat substraction = (background - currentFrame);
	return substraction;*/

	// Don't get overflow
	Mat output = currentFrame.clone();

	// Loop through all pixels and substract background from current frame
	for (int y = 0; y < output.rows; y++)
	{
		for (int x = 0; x < output.cols; x++)
		{
			int difference = background.at<uchar>(y, x) - currentFrame.at<uchar>(y, x);
			
			// Absolute value
			if (difference < 0)
				difference *= -1;

			// Threshold
			// TODO: USE LOCAL THRESHOLD INSTEAD
			if (difference <= threshold)
				difference = 0;
			else
				difference = 255;

			// New image with difference
			output.at<uchar>(y, x) = difference;
		}
	}

	return output;
	
}

Mat Threshold(Mat image, int threshold)
{
	// OLD - we don't use this anymore
	// Thresholding is part of PerformImageSubstraction() now


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

			// Make binary
			if (value <= threshold)
				image.at<uchar>(y, x) = 0;
			else
				image.at<uchar>(y, x) = 255;

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

//	return image;
}
Mat MedianFilter(Mat input)
	{		
	// 3x3 kernel
		
	// Make a temporary clone of the input image
	Mat MedianValue = input.clone();
		
	// Loop through all pixels
	for (int y = 0; y < input.rows-2; y++)
	{
		for (int x = 0; x < input.cols-2; x++)
		{
			if (x - 2 < 0 || y - 2 < 0) // don't go out of bounds
				continue;
				
					// Apply the kernel
					MedianValue.at<uchar>(y, x) = (
					input.at<uchar>(y-1, x-1) + input.at<uchar>(y-1, x)
					+ input.at<uchar>(y-1, x+1) + input.at<uchar>(y, x-1)
					+ input.at<uchar>(y, x) + input.at<uchar>(y, x+1)
					+ input.at<uchar>(y+1, x-1) + input.at<uchar>(y+1, x)
					+ input.at<uchar>(y+1, x+1)
					) /9;

			}
		}
		
		return MedianValue;
	}
