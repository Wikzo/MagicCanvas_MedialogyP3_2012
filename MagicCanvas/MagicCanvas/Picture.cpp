#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

void Picture::initialize(string fileName)
{
	tmp = imread(fileName);
	width = tmp.cols;
	height = tmp.rows;

	pixelR = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelR[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelR[x][y] = tmp.at<Vec3b>(y,x)[2];
	}

	pixelG = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelG[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelG[x][y] = tmp.at<Vec3b>(y,x)[1];
	}

	pixelB = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelB[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelB[x][y] = tmp.at<Vec3b>(y,x)[0];
	}
}
void Picture::initialize(VideoCapture captureToStoreCamra)
{
	captureToStoreCamra >> tmp;
	width = tmp.cols;
	height = tmp.rows;

	pixelR = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelR[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelR[x][y] = tmp.at<Vec3b>(y,x)[2];
	}

	pixelG = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelG[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelG[x][y] = tmp.at<Vec3b>(y,x)[1];
	}

	pixelB = new int*[width];
	for(int x = 0; x < width; x++)
	{
		pixelB[x] = new int[height];
		for(int y = 0; y < height; y++)
			pixelB[x][y] = tmp.at<Vec3b>(y,x)[0];
	}
}

void Picture::refresh(string fileName)
{
	tmp = imread(fileName);
	width = tmp.cols;
	height = tmp.rows;

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelR[x][y] = tmp.at<Vec3b>(y,x)[2];
	}

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelG[x][y] = tmp.at<Vec3b>(y,x)[1];
	}

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelB[x][y] = tmp.at<Vec3b>(y,x)[0];
	}
}
void Picture::refresh(VideoCapture captureToStoreCamra)
{
	captureToStoreCamra >> tmp;
	width = tmp.cols;
	height = tmp.rows;

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelR[x][y] = tmp.at<Vec3b>(y,x)[2];
	}

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelG[x][y] = tmp.at<Vec3b>(y,x)[1];
	}

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
			pixelB[x][y] = tmp.at<Vec3b>(y,x)[0];
	}
}


void Picture::output(string windowName)
{
	if(height == 0||width == 0){
		cout << "No picture is loaded";
		waitKey(0);
		exit(0);
	}
	Mat out(height, width, CV_8UC3);

	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
			 out.at<Vec3b>(y,x)[2] = pixelR[x][y];
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
			out.at<Vec3b>(y,x)[1] = pixelG[x][y];
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
			out.at<Vec3b>(y,x)[0] = pixelB[x][y];			
	imshow(windowName, out);
}
void Picture::reset()
{
	for(int i = 0; i < width; i++)
	{
		delete [] pixelR[i];
		delete [] pixelG[i];
		delete [] pixelB[i];
	}
	delete [] pixelR;
	delete [] pixelG;
	delete [] pixelB;

	width = 0;
	height = 0;

}

void Picture::binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold)
{
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if((pixelB[x][y] - refPicture.pixelB[x][y] > threshhold || pixelB[x][y] - refPicture.pixelB[x][y] < -1*threshhold)
				|| (pixelR[x][y] - refPicture.pixelR[x][y] > threshhold || pixelR[x][y] - refPicture.pixelR[x][y] < -1*threshhold)
				|| (pixelG[x][y] - refPicture.pixelG[x][y] > threshhold || pixelG[x][y] - refPicture.pixelG[x][y] < -1*threshhold))
			{
				pixelR[x][y] = 255;
				pixelG[x][y] = 255;
				pixelB[x][y] = 255;
			}
			else
			{
				pixelR[x][y] = 0;
				pixelG[x][y] = 0;
				pixelB[x][y] = 0;
			}
		}
	}
	
}
void Picture::findFirstRow(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine)
{
	bool found = false;
	for(int y = 0; y < height && !found; y++)
	{
		for(int x = 0; x < width-minRowLength && !found; x++)
		{
			found = true;
			for(int i = minRowLength; i > 0; i--)
			{
				for(int j = 0; j < minRowWidth; j++){
					if(pixelR[x+i][y+j] == 0)
						found = false;
				}
			}

			// Found left corner
			if(found)
			{
				
				//cout<< "x: " << x << "y: " << y;
				startOfTheLine.x = x;
				startOfTheLine.y = y;
				/*for(int x = 0; x < width; x++)
				{
					pixelR[x][y] = 255;
					pixelG[x][y] = 0;
					pixelB[x][y] = 0;
				}
				*/
				
				// Strech
				// RGB are all the same, so it doesn't matter what color channel is used
				while((pixelG[x+1][y] == 255 || pixelG[x+2][y] == 255 || pixelG[x+3][y] == 255) && x < width-4)
				{
					
					//pixelR[x][y] = 255;
					//pixelG[x][y] = 0;
					//pixelB[x][y] = 0;
					x++;
						
				}
				lengthOfTheLine = x - startOfTheLine.x;
			}
		}
	}
}
void Picture::drawPictureAt(point lowerLeftCorner, int newwidth, Picture pictureToDraw)
{
	//r 0, g 255, b 0
	float sf = (float)pictureToDraw.width/newwidth;

	int newheight = pictureToDraw.height/sf;


	//cout << "x: " << lowerLeftCorner.x << " y: " << lowerLeftCorner.y << " width: " << newwidth << " height: " << newheight << " sf: " << sf <<"\n";
	
	// Draw
	for(int x = 0; x < newwidth; x++){
		for(int y = 0; y < newheight; y++){
			if((lowerLeftCorner.y - newheight + y) >= 0){ 

				if(pictureToDraw.pixelR[(int)(x*sf)][(int)(y*sf)] != 0 && pictureToDraw.pixelG[(int)(x*sf)][(int)(y*sf)] != 255 && pictureToDraw.pixelB[(int)(x*sf)][(int)(y*sf)] != 0){
					pixelR[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelR[(int)(x*sf)][(int)(y*sf)];
					pixelG[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelG[(int)(x*sf)][(int)(y*sf)];
					pixelB[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelB[(int)(x*sf)][(int)(y*sf)];
				}
			}
		}
	}
}
void Picture::erode(int radius, Picture &tmpPicture)
{
	for(int x = radius; x < width-radius; x++)
	{
		for(int y = radius; y < height-radius; y++)
		{
			bool pixelIsaccepted = true;
			for(int filterX = x - radius; pixelIsaccepted && filterX <= x + radius; filterX++)
			{
				for(int filterY = y - radius; pixelIsaccepted && filterY <= y + radius; filterY++)
				{
					if (pixelR[filterX][filterY] == 0)
					{
						pixelIsaccepted = false;
					}
				}
			}
			if (pixelIsaccepted == true)
				tmpPicture.pixelR[x][y] = 255;
			else
				tmpPicture.pixelR[x][y] = 0;
		}
	}
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++){
			pixelR[x][y] = tmpPicture.pixelR[x][y];
			pixelG[x][y] = tmpPicture.pixelR[x][y];
			pixelB[x][y] = tmpPicture.pixelR[x][y];
		}
}
void Picture::dilate(int radius, Picture &tmpPicture)
{
	for(int x = radius; x < width-radius; x++)
	{
		for(int y = radius; y < height-radius; y++)
		{
			bool pixelIsaccepted = false;
			for(int filterX = x - radius; !pixelIsaccepted && filterX <= x + radius; filterX++)
			{
				for(int filterY = y - radius; !pixelIsaccepted && filterY <= y + radius; filterY++)
				{
					if (pixelR[filterX][filterY] == 255)
					{
						pixelIsaccepted = true;
					}
				}
			}
			if (pixelIsaccepted == true)
				tmpPicture.pixelR[x][y] = 255;
			else
				tmpPicture.pixelR[x][y] = 0;
		}
	}
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++){
			pixelR[x][y] = tmpPicture.pixelR[x][y];
			pixelG[x][y] = tmpPicture.pixelR[x][y];
			pixelB[x][y] = tmpPicture.pixelR[x][y];
		}
}
void Picture::makeBlack()
{
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			pixelR[x][y] = 0;
			pixelG[x][y] = 0;
			pixelB[x][y] = 0;
		}
	}
}
void Picture::startFire(point startingPoint, color objColor, Picture &tmpPicture)
{
	tmpPicture.makeBlack();
	point currentPosition = startingPoint;
	int pixelCount = 0;
	while(true)
	{
		tmpPicture.pixelR[currentPosition.x][currentPosition.y] = 255;
		//if right is available, is free and not burned
		if(currentPosition.x+1 < width && pixelR[currentPosition.x+1][currentPosition.y] == 255 && tmpPicture.pixelR[currentPosition.x+1][currentPosition.y] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x++;
		}
		//if down is available, is free and not burned
		else if(currentPosition.y+1 < height && pixelR[currentPosition.x][currentPosition.y+1] == 255 && tmpPicture.pixelR[currentPosition.x][currentPosition.y+1] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y++;
		}
		//if left is available, is free and not burned
		else if(currentPosition.x-1 > 0 && pixelR[currentPosition.x-1][currentPosition.y] == 255 && tmpPicture.pixelR[currentPosition.x-1][currentPosition.y] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x--;
		}
		//if up is available, is free and not burned
		else if(currentPosition.y-1 > 0 && pixelR[currentPosition.x][currentPosition.y-1] == 255 && tmpPicture.pixelR[currentPosition.x][currentPosition.y-1] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y--;
		}
		//steps back:
		//if right is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x+1 < width && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x++;
		}
		//if down is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y+1 < height && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y++;
		}
		//if left is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x-1 > 0 && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x--;
		}
		//if up is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y-1 > 0 && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y--;
		}
		else
			break;
	}
	if(pixelCount > 1000)
		numberOfPersons++;
	//cout << pixelCount<< "\n";
	//TODO: Maybe this could be a problem when the next blob has to be found:
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			if(tmpPicture.pixelR[x][y] == 255){
				//the blob has to be at least 1000 pixels big to be taken as a blob of a person => the color will be green
				if(pixelCount > 1000)
				{

					pixelR[x][y] = objColor.r;
					pixelG[x][y] = objColor.g;
					pixelB[x][y] = objColor.b;
				}
				else
				{
					pixelR[x][y] = 1;//objColor.r;
					pixelG[x][y] = 0;
					pixelB[x][y] = 0;
				}
			}
		}
	}
}
void Picture::findAllBLOBs(Picture &tmpPicture, Person persons[], int maxNumberOfPersons)
{
	numberOfPersons = 0;
	color currentColor;
	currentColor.r = 254;
	currentColor.g = 255;
	currentColor.b = 255;

	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			if(pixelR[x][y] == 255)
			{ 
				
				//currentColor.g -= 1;
				//currentColor.b -= 1;
				point currentPoint;
				currentPoint.x = x;
				currentPoint.y = y;
				startFireLoggingPersons(currentPoint);
				currentColor.r -= 1;
			}
		}
	}
	cout << "\n" << currentColor.r;
}
void Picture::placeHats(int minRowLength, int minRowWidth, point &startOfTheLine, int &lengthOfTheLine, Picture hat)
{
	const int maxNumberOfPersons = 3;
	int lengthOfTheLines[maxNumberOfPersons];
	point startOfLines[maxNumberOfPersons];
	for(int k = 0; k < maxNumberOfPersons; k++){
		bool found = false;
		for(int y = 0; y < height && !found; y++)
		{
			for(int x = 0; x < width-minRowLength && !found; x++)
			{
				found = true;
				for(int i = minRowLength; i > 0; i--)
				{
					for(int j = 0; j < minRowWidth; j++){
						if(pixelR[x+i][y+j] != k+255-maxNumberOfPersons+1)
							found = false;
					}
				}

				// Found left corner
				if(found)
				{
					//cout<< "x: " << x << "y: " << y;
					startOfTheLine.x = x;
					startOfTheLine.y = y;
					startOfLines[k] = startOfTheLine;

					/*for(int x = 0; x < width; x++)
					{
						pixelR[x][y] = 255;
						pixelG[x][y] = 0;
						pixelB[x][y] = 0;
					}
					*/
				
					// Strech
					// RGB are all the same, so it doesn't matter what color channel is used
					while((pixelG[x+1][y] == 255 || pixelG[x+2][y] == 255 || pixelG[x+3][y] == 255) && x < width-4)
					{
					
						//pixelR[x][y] = 255;
						//pixelG[x][y] = 0;
						//pixelB[x][y] = 0;
						x++;
						
					}
					lengthOfTheLine = x - startOfTheLine.x;
					lengthOfTheLines[k] = lengthOfTheLine;
				}
			}
		}
	}
	for(int k = 0; k < maxNumberOfPersons; k++){
		if(lengthOfTheLines[k] > 0)
			drawPictureAt(startOfLines[k], lengthOfTheLines[k], hat);
		lengthOfTheLines[k] = 0;
	}
		
}
void Picture::startFireLoggingData(point startingPoint, color objColor, Picture &tmpPicture, Person persons[], int maxNumberOfPersons)
{
	tmpPicture.makeBlack();
	point currentPosition = startingPoint;
	int pixelCount = 0;
	while(true)
	{
		tmpPicture.pixelR[currentPosition.x][currentPosition.y] = 255;
		//if right is available, is free and not burned
		if(currentPosition.x+1 < width && pixelR[currentPosition.x+1][currentPosition.y] == 255 && tmpPicture.pixelR[currentPosition.x+1][currentPosition.y] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x++;
		}
		//if down is available, is free and not burned
		else if(currentPosition.y+1 < height && pixelR[currentPosition.x][currentPosition.y+1] == 255 && tmpPicture.pixelR[currentPosition.x][currentPosition.y+1] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y++;
		}
		//if left is available, is free and not burned
		else if(currentPosition.x-1 > 0 && pixelR[currentPosition.x-1][currentPosition.y] == 255 && tmpPicture.pixelR[currentPosition.x-1][currentPosition.y] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x--;
		}
		//if up is available, is free and not burned
		else if(currentPosition.y-1 > 0 && pixelR[currentPosition.x][currentPosition.y-1] == 255 && tmpPicture.pixelR[currentPosition.x][currentPosition.y-1] != 255)
		{
			pixelCount++;
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y--;
		}
		//steps back:
		//if right is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x+1 < width && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x++;
		}
		//if down is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y+1 < height && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y++;
		}
		//if left is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x-1 > 0 && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1] && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] > tmpPicture.pixelG[currentPosition.x][currentPosition.y-1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x--;
		}
		//if up is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y-1 > 0 && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x+1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x-1][currentPosition.y] && tmpPicture.pixelG[currentPosition.x][currentPosition.y-1] > tmpPicture.pixelG[currentPosition.x][currentPosition.y+1])
		{
			tmpPicture.pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y--;
		}
		else
			break;
	}
	int minX = height + 100;
	int maxX = -100;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			if(tmpPicture.pixelR[x][y] == 255){
				if(pixelCount > minPixelToBeAPerson)
				{
					//detect the possition on the x-axis
					if(x > maxX)
						maxX = x;
					if(x < minX)
						minX = x;

					pixelR[x][y] = objColor.r;
					pixelG[x][y] = objColor.g;
					pixelB[x][y] = objColor.b;
				}
				else
				{
					pixelR[x][y] = 1;//objColor.r;
					pixelG[x][y] = 0;
					pixelB[x][y] = 0;
				}
			}
		}
	}

	if(pixelCount > minPixelToBeAPerson)
	{
		float average = ((minX + maxX)/2);
		float zeroToOne = average/width;

		persons[numberOfPersons].posOnX = 1 - zeroToOne;
		numberOfPersons++;
		
	}
}
void Picture::refreshBGSubtractAndThreshholdForBnW(VideoCapture captureToStoreCamra, Picture refPicture, int threshhold)
{
	captureToStoreCamra >> tmp;
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if((int)tmp.at<Vec3b>(y,x)[1] - refPicture.pixelG[x][y] < -1*threshhold) 
			{
				//cout << (int)tmp.at<Vec3b>(y,x)[0];
				pixelR[x][y] = 255;
				pixelG[x][y] = 255;
				pixelB[x][y] = 255;
			}
			else
			{
				pixelR[x][y] = 0;
				pixelG[x][y] = 0;
				pixelB[x][y] = 0;
			}
		}
	}
}
void Picture::lookForNewPersons(int procentOfScreenUsedForEnterAndExit, int heightOfUpperFOI)
{
	numberOfPersons = 0;

	int hasToBeChanged = 1;

	int y = height-1-radiusForMorfology;
	for(int i = 0; i < 2; i++){
		for(int x = 0; x < width * (procentOfScreenUsedForEnterAndExit/2) / 100; x++){
			//if(pixelR[x][y] == 255)
			{ 
#pragma region showEnterExit
				//pixelR[x][y] = 255;
				//pixelG[x][y] = 0;
				//pixelB[x][y] = 0;
#pragma endregion
				point currentPoint;
				currentPoint.x = x;
				currentPoint.y = y;
				startFireLoggingPersons(currentPoint);
				////startFireLoggingData(currentPoint, currentColor, tmpPicture, persons, maxNumberOfPersons);
				//currentColor.r -= 1;
			}
		}
		for(int x = width-1; x > width - ((width * (procentOfScreenUsedForEnterAndExit/2)) / 100); x--){
			//if(pixelR[x][y] == 255)
			{
#pragma region showEnterExit
				//pixelR[x][y] = 255;
				//pixelG[x][y] = 0;
				//pixelB[x][y] = 0;
#pragma endregion

				point currentPoint;
				currentPoint.x = x;
				currentPoint.y = y;
				startFireLoggingPersons(currentPoint);
				////startFireLoggingData(currentPoint, currentColor, tmpPicture, persons, maxNumberOfPersons);
				//currentColor.r -= 1;
			}
		}
		y = heightOfUpperFOI;
	}
}
void Picture::startFireLoggingPersons(point startingPoint)
{
	if(pixelR[startingPoint.x][startingPoint.y] != 255) 
		return;
	//R is for Input - 255 = blob not classified, 0 = nothing found or to small
	//B is for Burned/Output
	//G is for way back
	//TODO maybee move in lookForNewPersons()-------------------------------------------------------------
	resetChannelsExcept('R');
	point currentPosition = startingPoint;
	int pixelCount = 0;


	//TODO height and width -- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while(true)
	{
		pixelB[currentPosition.x][currentPosition.y] = 255;

		//if right is available, is free and not burned
		if(currentPosition.x+1 < width && pixelR[currentPosition.x+1][currentPosition.y] == 255 && pixelB[currentPosition.x+1][currentPosition.y] != 255)
		{
			pixelCount++;
			pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x++;
		}
		//if down is available, is free and not burned
		else if(currentPosition.y+1 < height && pixelR[currentPosition.x][currentPosition.y+1] == 255 && pixelB[currentPosition.x][currentPosition.y+1] != 255)
		{
			pixelCount++;
			pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y++;
		}
		//if left is available, is free and not burned
		else if(currentPosition.x-1 >= 0 && pixelR[currentPosition.x-1][currentPosition.y] == 255 && pixelB[currentPosition.x-1][currentPosition.y] != 255)
		{
			pixelCount++;
			pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.x--;
		}
		//if up is available, is free and not burned
		else if(currentPosition.y-1 >= 0 && pixelR[currentPosition.x][currentPosition.y-1] == 255 && pixelB[currentPosition.x][currentPosition.y-1] != 255)
		{
			pixelCount++;
			pixelG[currentPosition.x][currentPosition.y] = pixelCount;
			currentPosition.y--;
		}
		//steps back:
		//if right is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x+1 < width && (currentPosition.y+1 >= height || pixelG[currentPosition.x+1][currentPosition.y] > pixelG[currentPosition.x][currentPosition.y+1]) && (currentPosition.x-1 < 0 || pixelG[currentPosition.x+1][currentPosition.y] > pixelG[currentPosition.x-1][currentPosition.y]) && (currentPosition.y-1 < 0 || pixelG[currentPosition.x+1][currentPosition.y] > pixelG[currentPosition.x][currentPosition.y-1]))
		{
			pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x++;
		}
		//if down is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y+1 < height && (currentPosition.x+1 >= width || pixelG[currentPosition.x][currentPosition.y+1] > pixelG[currentPosition.x+1][currentPosition.y]) && (currentPosition.x-1 < 0 || pixelG[currentPosition.x][currentPosition.y+1] > pixelG[currentPosition.x-1][currentPosition.y]) && (currentPosition.y-1 < 0||pixelG[currentPosition.x][currentPosition.y+1] > pixelG[currentPosition.x][currentPosition.y-1]))
		{
			pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y++;
		}
		//if left is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.x-1 >= 0 && (currentPosition.y+1 >= height || pixelG[currentPosition.x-1][currentPosition.y] > pixelG[currentPosition.x][currentPosition.y+1]) && (currentPosition.x+1 >= width || pixelG[currentPosition.x-1][currentPosition.y] > pixelG[currentPosition.x+1][currentPosition.y]) && (currentPosition.y-1 < 0 || pixelG[currentPosition.x-1][currentPosition.y] > pixelG[currentPosition.x][currentPosition.y-1]))
		{
			pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.x--;
		}
		//if up is available, burned and has the biggest count -> we have been there last
		else if(currentPosition.y-1 >= 0 && (currentPosition.x+1 >= width || pixelG[currentPosition.x][currentPosition.y-1] > pixelG[currentPosition.x+1][currentPosition.y]) && (currentPosition.x-1 < 0||pixelG[currentPosition.x][currentPosition.y-1] > pixelG[currentPosition.x-1][currentPosition.y]) && (currentPosition.y+1 >= height||pixelG[currentPosition.x][currentPosition.y-1] > pixelG[currentPosition.x][currentPosition.y+1]))
		{
			pixelG[currentPosition.x][currentPosition.y] = 0;
			currentPosition.y--;
		}
		else
			break;
	}

	//TODO- make the following recursive
	int minX = height + 100;
	int maxX = -100;
	if(pixelCount > minPixelToBeAPerson)
	{
		numberOfPersons++;
		for(int y = 0; y < height; y++)
			for(int x = 0; x < width; x++)
				if(pixelB[x][y] == 255)
				{
					//detect the possition on the x-axis
					if(x > maxX)
						maxX = x;
					if(x < minX)
						minX = x;
					
					pixelR[x][y] = 255 - numberOfPersons;
				}
		float average = ((minX + maxX)/2);
		float zeroToOne = average/width;
		p[numberOfPersons].posX = 1 - zeroToOne;
	}
	else
		for(int y = 0; y < height; y++)
			for(int x = 0; x < width; x++)
				if(pixelB[x][y] == 255)
					pixelR[x][y] = 0;//objColor.r;
}
void Picture::resetChannel(char RorGorB)
{
	switch(RorGorB)
	{
	case 'r':case'R':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelR[x][y] = 0;
			}
		}
		break;
	case 'g':case'G':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelG[x][y] = 0;
			}
		}
		break;
	case 'b':case'B':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelB[x][y] = 0;
			}
		}
		break;
	default:
		cout << "A channel is not reseted!!!!!!!!!!!!!!!!!!!!!\n\nA wrong channel is chosen";
	};
}
void Picture::resetChannelsExcept(char RorGorB)
{
	switch(RorGorB)
	{
	case 'r':case'R':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelB[x][y] = 0;
				pixelG[x][y] = 0;
			}
		}
		break;
	case 'g':case'G':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelR[x][y] = 0;
				pixelB[x][y] = 0;
			}
		}
		break;
	case 'b':case'B':
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				pixelG[x][y] = 0;
				pixelR[x][y] = 0;
			}
		}
		break;
	default:
		cout << "A channel is not reseted!!!!!!!!!!!!!!!!!!!!!\n\nA wrong channel is chosen";
	};
}