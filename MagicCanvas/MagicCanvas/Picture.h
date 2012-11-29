#ifndef PICTURE_H // if products not defined, define it here
#define PICTURE_H

#pragma once // only include things one time (Visual Studio specific)

#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
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

struct Person
{
	float posOnX;
};

class Picture
{
public:
	
	class person
	{
	public:

		float posX;
		float moveVector;
		int heightOfROI;
		double id;
		int pId;

		//static Picture& parent;
		int** pixel;

		//person(const Picture& parentPicture);

		bool refind(Picture& parent);
		bool refindOccluded(Picture& parent);

	}p[50];
	int currentPersonId;
	double personCount;


	int** pixelR; //Pointers to the value of each color pixel on the image
	int** pixelG;
	int** pixelB;


	Mat tmp;

	int height;
	int width;
	int minPixelToBeAPerson;
	int maxAmountToMove;
	int radiusForMorfology;
	int numberOfPersons; //TODO - Remove
	
	//TODO: find a smart way to determine that!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	float initialMoveVector;
	//functions that work on all kinds of pictures
	void initialize(string fileName);
	void initialize(VideoCapture captureToStoreCamra);
	void refresh(string fileName);
	void refresh(VideoCapture captureToStoreCamra);
	void binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold);
	//This function compares the input from the camera to the bg. When the input is bright with the chosen threshold the output pixel will be true.
	void refreshBGSubtractAndThreshholdForBnW(VideoCapture captureToStoreCamra, Picture refPicture, int threshhold);
	void drawPictureAt(point lowerLeftCorner, int newidth, Picture pictureToDraw);
	void makeBlack();
	void output(string windowName);
	void reset();
	//functions that just work on binary pictures:
	void findFirstRow(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine);
	void erode(int radius, Picture &tmpPicture);
	void dilate(int radius, Picture &tmpPicture);
	void startFire(point startingPoint, color objColor, Picture &tmpPicture);
	void startFireLoggingData(point startingPoint, color objColor, Picture &tmpPicture, Person persons[], int maxNumberOfPersons);
	void findAllBLOBs(Picture &tmpPicture, Person persons[], int maxNumberOfPersons);
	void placeHats(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine, Picture hat);

	void lookForNewPersons(int procentOfScreenUsedForEnterAndExit, int heightOfUpperFOI);
	void handleFoundPersons();

	void startFireLoggingPersons(point startingPoint);
	void startFireLoggingOccludedPersons(point startingPoint);

	void resetChannel(char RorGorB);
	void resetChannelsExcept(char RorGorB);

	void coutPersons();
	void clipPersons();
	
};

#endif
