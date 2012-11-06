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

		imshow("video", frame);
	}

	return 0;
}