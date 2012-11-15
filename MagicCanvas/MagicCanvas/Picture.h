#ifndef PICTURE_H // if products not defined, define it here
#define PICTURE_H

#pragma once // only include things one time (Visual Studio specific)

#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
using namespace cv;

struct color
{
	int r;
	int g;
	int b;
};

struct point
{
	int x;
	int y;
	point():x(0),y(0){};
	point(int x, int y):x(x),y(y){};
};

class Picture
{
public:
	int** pixelR; //Pointers to the value of each color pixel on the image
	int** pixelG;
	int** pixelB;
	int height;
	int width;
	//functions that work on all kinds of pictures
	void openFile(string name);
	void openCamera(VideoCapture captureToStoreCamra);
	void binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold);
	void drawPictureAt(point lowerLeftCorner, int newidth, Picture pictureToDraw);
	void makeBlack();
	void output(string windowName);
	void reset();
	//functions that just work on binary pictures:
	void findFirstRow(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine);
	void erode(int radius, Picture &tmpPicture);
	void dilate(int radius, Picture &tmpPicture);
	void startFire(point startingPoint, color objColor, Picture &tmpPicture);
	void findAllBLOBs(Picture &tmpPicture);
};

#endif