#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Testing GIT
// Fighting Mongooses
// Marco, Simon, Gustav

// Step 1: Save reference image (background)
// Step 2: Capture current image (video)
// Step 3: Perform image subtraction
// Step 4: Threshold (local)
// Step 5: Noise reduction - Mean filter or morphological opening/closing

// Function prototypes
void GetBackground(VideoCapture capture, Mat &backgroundToWriteTo);
Mat PerformImagesubtraction(Mat currentFrame, Mat background, int threshold);
Mat Threshold(Mat image, int threshold);
Mat MeanFilter(Mat input);
uchar BubbleSort(uchar a[], int size);
Mat MedianFilter(Mat input);

Mat Erosion(Mat input, int radius);
Mat Dilation(Mat input, int radius);

int main()
{
	// Fields
	Mat background;
	Mat currentFrame;
	Mat subtraction;

	VideoCapture capture;
	bool isMac = 0;
	
	capture.open(isMac);
	
	//safety check
	if (!capture.isOpened())
	{
		cout << "ERROR - camera not working";
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
		subtraction = PerformImagesubtraction(currentFrame, background, 100);
		
		// Median blur (built-in function)
		//cv::medianBlur(subtraction, subtraction, 5);

		// Manual subtraction
		subtraction = MedianFilter(subtraction);

		/*// Morphology (VERY SLOW)
		// Closing --> opening
		// closing = Dilation + erosion
		// Opening = Erosion + dilation
		subtraction = Dilation(subtraction, 1);
		subtraction = Erosion(subtraction, 1);
		subtraction = Erosion(subtraction, 1);
		subtraction = Dilation(subtraction, 1);*/


		// -------- DEBUG FEATURES START --------------
		// Exit
		if ((char)waitKey(30) == 'q')
			break;

		// Grab new background screenshot
		if ((char)waitKey(1) == 's')
			GetBackground(capture, background);

		// -------- DEBUG FEATURES END --------------

		// -------- SHOW OUTPUT START --------------
		imshow("Background", background);
		imshow("Video", currentFrame);
		imshow("Background subtraction", subtraction);
		// -------- SHOW OUTPUT END --------------
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
Mat PerformImagesubtraction(Mat currentFrame, Mat background, int threshold)
{

	// Don't get overflow
	Mat output = currentFrame.clone();

	// Loop through all pixels and substract background from current frame
	for (int y = 0; y < output.rows; y++)
	{
		for (int x = 0; x < output.cols; x++)
		{
			int difference = currentFrame.at<uchar>(y, x) - background.at<uchar>(y, x);
			
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
	// Thresholding is part of PerformImagesubtraction() now

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

	return mean;
}

Mat Erosion(Mat input, int radius)
{
	Mat output = input.clone();

	for(int x = radius; x < input.cols-radius; x++)
	{
		for(int y = radius; y < input.rows-radius; y++)
		{
			bool pixelIsaccepted = true;
			for(int filterX = x - radius; pixelIsaccepted && filterX <= x + radius; filterX++)
			{
				for(int filterY = y - radius; pixelIsaccepted && filterY <= y + radius; filterY++)
				{
					if (input.at<uchar>(filterY,filterX) == 0)
					{
						pixelIsaccepted = false;
					}
				}
			}
			if (pixelIsaccepted == true)
				output.at<uchar>(y,x) = 255;
			else
				output.at<uchar>(y,x) = 0;
		}
	}

	return output;
}

Mat Dilation(Mat input, int radius)
{
	Mat output = input.clone();

	for(int x = radius; x < input.cols-radius; x++)
		{
			for(int y = radius; y < input.rows-radius; y++)
			{
				bool pixelIsaccepted = false;
				for(int filterX = x - radius; !pixelIsaccepted && filterX <= x + radius; filterX++)
				{
					for(int filterY = y - radius; !pixelIsaccepted && filterY <= y + radius; filterY++)
					{
						if (input.at<uchar>(filterY,filterX) == 255)
						{
							pixelIsaccepted = true;
						}
					}
				}
				if (pixelIsaccepted == true)
					output.at<uchar>(y,x) = 255;
				else
					output.at<uchar>(y,x) = 0;
			}
		}
	return output;
	
}

Mat MedianFilter(Mat input)
	{		
	// 3x3 kernel
		
	// Make a temporary clone of the input image
	Mat MedianValue = input.clone();
	uchar arrayOfMedians[9];
		
	// Loop through all pixels
	for (int y = 0; y < input.rows-1; y++)
	{
		for (int x = 0; x < input.cols-1; x++)
		{
			if (x - 1 < 0 || y - 1 < 0) // don't go out of bounds
				continue;
				
			// Apply the kernel
			int index = 0;
			arrayOfMedians[index] = MedianValue.at<uchar>(y-1, x-1);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y-1, x);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y-1, x+1);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y, x-1);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y, x);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y, x+1);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y+1, x-1);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y+1, x);
			arrayOfMedians[++index] = MedianValue.at<uchar>(y+1, x+1);
			
			MedianValue.at<uchar>(y,x) = BubbleSort(arrayOfMedians, 9);
	
			}
		}
		return MedianValue;
}

uchar BubbleSort(uchar a[], int size)
{
	
	//	for (int i = 0; i < size; i++)
	//		cout << a[i] << endl;
	
	// Bubble list
	int index = 0;
	bool swapped = true;
	
	
	while (swapped)
	{
		swapped = false;
		for (int i = 1; i < size; i++)
		{
			if (a[i-1] > a[i])
			{
				// Sort numbers
				uchar temp = a[i-1];
				a[i-1] = a[i];
                a[i] = temp;
				
                swapped = true;
				
				
			}
		}
	}
	
	return a[4];
}
