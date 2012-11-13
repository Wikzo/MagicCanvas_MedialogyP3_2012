#ifndef PICTURE_H // if products not defined, define it here
#define PICTURE_H

#pragma once // only include things one time (Visual Studio specific)

#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
using namespace cv;

struct point
{
	int x;
	int y;
};

class Picture
{
public:
	int** pixelR; //Pointers to the value of each color pixel on the image
	int** pixelG;
	int** pixelB;
	void openFile(string name);
	void openCamera(VideoCapture captureToStoreCamra);
	void binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold);
	void output(string windowName);
	void reset();
	void findFirstRow(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine);
	void drawPictureAt(point lowerLeftCorner, int newidth, Picture pictureToDraw);
	void erode(int radius, Picture &tmpPicture);
	void dilate(int radius, Picture &tmpPicture);
	void makeBlack();
	bool isBW;
	int height;
	int width;
};

#endif