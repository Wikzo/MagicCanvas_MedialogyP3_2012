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
	//captureToStoreCamra.set(CV_CAP_PROP_EXPOSURE, 100.0);
	//captureToStoreCamra.set(CV_CAP_PROP_GAIN, 100.0);
	//cvSetCaptureProperty(captureToStoreCamra, CV_CAP_PROP_EXPOSURE, 0.1f)
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
	//captureToStoreCamra.set(CV_CAP_PROP_EXPOSURE, 1.0);
	//captureToStoreCamra.set(CV_CAP_PROP_GAIN, 1.0);

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
				//pixelG[x][y] = 255;
				//pixelB[x][y] = 255;
			}
			else
			{
				pixelR[x][y] = 0;
				//pixelG[x][y] = 0;
				//pixelB[x][y] = 0;
			}
		}
	}
}
void Picture::refreshDiscradBGSubtractAndThreshholdForBnW(VideoCapture captureToStoreCamra, Picture refPicture, int threshhold, int procentOfScreenUsed, int heightOfLowerFOI, int heightOfUpperFOI)
{
	captureToStoreCamra >> tmp;
	for(int x = 0; x < width; x++)
	{
		//for(int y = height-1; y > height - (int)(((float)(procentOfScreenUsed/2)/100)*height) ; y--)
		for(int y = heightOfLowerFOI; y > heightOfLowerFOI - (int)(((float)(procentOfScreenUsed/2)/100)*height); y--)
		{
			if((int)tmp.at<Vec3b>(y,x)[1] - refPicture.pixelG[x][y] < -1*threshhold) 
			{
				pixelR[x][y] = 255;
			}
			else
			{
				pixelR[x][y] = 0;
			}
		}
		/*
		for(int y = heightOfUpperFOI; y > heightOfUpperFOI - (int)(((float)(procentOfScreenUsed/2)/100)*height); y--)
		{
			if((int)tmp.at<Vec3b>(y,x)[1] - refPicture.pixelG[x][y] < -1*threshhold) 
			{
				pixelR[x][y] = 255;
			}
			else
			{
				pixelR[x][y] = 0;
			}
		}
		*/
	}
}
void Picture::lookForNewPersons(int procentOfScreenUsedForEnterAndExit, int brightestYatX[])
{
	//int y = height-1-radiusForMorfology;

	//int y = heightOfUpperFOI;
	/*for(int i = 0; i < 2; i++){*/
		for(int x = 0; x < width * (procentOfScreenUsedForEnterAndExit/2) / 100; x++)
		{
			//cout << x << ","<< y <<endl;
			//remove:
//#pragma region showEnterExit
				//pixelR[x][brightestYatX[x]] = 255;
				//pixelG[x][brightestYatX[x]] = 0;
				//pixelB[x][brightestYatX[x]] = 0;
//#pragma endregion
			if(pixelR[x][brightestYatX[x]] == 255)
			{ 
				point currentPoint;
				currentPoint.x = x;
				currentPoint.y = brightestYatX[x];
				// find the next empty:


				int j = 0;
				do
				{
					currentPersonId = j;
					j++;
				} 
				while (j < 51 && p[j-1].posX != -1);

				startFireLoggingPersons(currentPoint);

				if(p[currentPersonId].posX != -1)
				{
					personCount++;
					p[currentPersonId].notAddedToTheNewInitialMoveVectorProductYet = true;
					p[currentPersonId].id = personCount;
					p[currentPersonId].moveVector = initialMoveVector;
					p[currentPersonId].heightOfROI = brightestYatX[x];
				}
			}
		}
		for(int x = width-1; x > width - ((width * (procentOfScreenUsedForEnterAndExit/2)) / 100); x--)
		{
//#pragma region showEnterExit
				/*pixelR[x][brightestYatX[x]] = 255;
				pixelG[x][brightestYatX[x]] = 0;
				pixelB[x][brightestYatX[x]] = 0;*/
//#pragma endregion
			if(pixelR[x][brightestYatX[x]] == 255)
			{
				point currentPoint;
				currentPoint.x = x;
				currentPoint.y = brightestYatX[x];
				// find the next empty:

				int j = 0;
				do
				{
					currentPersonId = j;
					j++;
				} 
				while (j < 51 && p[j-1].posX != -1);

				startFireLoggingPersons(currentPoint);

				if(p[currentPersonId].posX != -1)
				{
					personCount++;
					p[currentPersonId].notAddedToTheNewInitialMoveVectorProductYet = true;
					p[currentPersonId].id = personCount;
					p[currentPersonId].moveVector = -1*initialMoveVector;
					p[currentPersonId].heightOfROI = brightestYatX[x];
				}
			}
		}
	//	y = heightOfUpperFOI;
	//}
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

	p[currentPersonId].minX = height + 100;
	p[currentPersonId].maxX = -100;

	//TODO height and width -- test if this is done!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while(true)
	{
		//cout << "lala";
		pixelB[currentPosition.x][currentPosition.y] = 255;

		if(currentPosition.x > p[currentPersonId].maxX)
			p[currentPersonId].maxX = currentPosition.x;
		if(currentPosition.x < p[currentPersonId].minX)
			p[currentPersonId].minX = currentPosition.x;
					
		pixelR[currentPosition.x][currentPosition.y] = 200 + currentPersonId;

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
		//if right is available, burned and has the biggest count(also check if the pixel, that is compared to is available) -> we have been there last -> go back 
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

	//TODO- make the following recursive/build into the recursive function
	//cout << pixelCount << " min: " << minPixelToBeAPerson;
	if(pixelCount > minPixelToBeAPerson)
	{
		float average = ((p[currentPersonId].minX + p[currentPersonId].maxX)/2);
		float zeroToOne = average/width;
		p[currentPersonId].posX = zeroToOne;
	}
	else
	{
		//TODO do it again nothing is found !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for(int y = 0; y < height; y++)
			for(int x = 0; x < width; x++)
				if(pixelB[x][y] == 255)
					pixelR[x][y] = 0;
		p[currentPersonId].posX = -1;
	}
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
bool Picture::person::refind(Picture& parent)
{
	point currentPoint;
	bool found = false;
	int maxAmountToMove = parent.maxAmountToMove;
	parent.currentPersonId = pId;
	//TODO initial vel. handling!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	if(posX+moveVector < 1 && posX+moveVector >= 0 && parent.pixelR[(int)((posX+moveVector)*parent.width)][heightOfROI] == 255)
	{
		//first priority to look is the position+the move vector (has moved normal)
		currentPoint.x = (int)((posX+moveVector)*parent.width);
		currentPoint.y = heightOfROI;
		parent.startFireLoggingPersons(currentPoint);
		if(parent.p[parent.currentPersonId].posX != -1)
		{
			cout << "found first try \n";
			found = true;
		}
	} 
	if(posX < 1 && posX >= 0 && parent.pixelR[(int)(posX*parent.width)][heightOfROI] == 255 && !found)
	{
		//second priority to look is the position (has stoped)
		currentPoint.x = (int)(posX*parent.width);
		currentPoint.y = heightOfROI;
		parent.startFireLoggingPersons(currentPoint);
		if(parent.p[parent.currentPersonId].posX != -1)
		{
			cout << "found second try \n";
			found = true;
		}
	} 
	//TODO on side is never complete!!! --- exp. |( ! )<----f1-m1,2-f2---->|
	//third priority is to look around the most likely point until the max amount to move is reached
	for(int i = 0; (int)((moveVector/2)*parent.width)+i <= maxAmountToMove && (int)((moveVector/2)*parent.width)-i >= -maxAmountToMove && !found; i++/*check if this works*/)
	{
		if((int)((posX+moveVector/2)*parent.width)+i < parent.width && (int)((posX+moveVector/2)*parent.width)+i >= 0 && parent.pixelR[(int)((posX+moveVector/2)*parent.width)+i][heightOfROI] == 255)
		{
			currentPoint.x = (int)((posX+moveVector/2)*parent.width)+i;
			currentPoint.y = heightOfROI;
			parent.startFireLoggingPersons(currentPoint);
			if(parent.p[parent.currentPersonId].posX != -1)
			{
				cout << "found normal \n";
				found = true;
				break;
			}
		}
		if((int)((posX+moveVector/2)*parent.width)-i < parent.width && (int)((posX+moveVector/2)*parent.width)-i >= 0 && parent.pixelR[(int)((posX+moveVector/2)*parent.width)-i][heightOfROI] == 255)
		{
			currentPoint.x = (int)((posX+moveVector/2)*parent.width)-i;
			currentPoint.y = heightOfROI;
			parent.startFireLoggingPersons(currentPoint);
			if(parent.p[parent.currentPersonId].posX != -1)
			{
				cout << "found normal \n";
				found = true;
				break;
			}
		}
	}
	return found;
}
bool Picture::person::refindOccluded(Picture& parent)
{
	point currentPoint;
	bool found = false;
	int maxAmountToMove = parent.maxAmountToMove;
	parent.currentPersonId = pId;
	//TODO initial vel. handling!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	if(posX+moveVector < 1 && posX+moveVector >= 0 && parent.pixelR[(int)((posX+moveVector)*parent.width)][heightOfROI] != 0)
	{
		//first priority to look is the position+the move vector (has moved normal)
		cout << "found occluded - first try \n";
		posX = parent.p[parent.pixelR[(int)((posX+moveVector)*parent.width)][heightOfROI]-200].posX;
		found = true;
	} 
	if(posX < 1 && posX >= 0 && parent.pixelR[(int)(posX*parent.width)][heightOfROI] != 0 && !found)
	{
		//second priority to look is the position (has stoped)
		cout << "found occluded - second try \n";
		posX = parent.p[parent.pixelR[(int)(posX*parent.width)][heightOfROI]-200].posX;
		found = true;
	} 
	//TODO on side is never complete!!! --- exp. |( ! )<----f1-m1,2-f2---->|
	//third priority is to look around the most likely point until the max amount to move is reached
	for(int i = 0; (int)((moveVector/2)*parent.width)+i <= maxAmountToMove && (int)((moveVector/2)*parent.width)-i >= -maxAmountToMove && !found; i++/*check if this works*/)
	{
		if((int)((posX+moveVector/2)*parent.width)+i < parent.width && (int)((posX+moveVector/2)*parent.width)+i >= 0 && parent.pixelR[(int)((posX+moveVector/2)*parent.width)+i][heightOfROI] != 0)
		{
			cout << "found occluded\n";
			posX = parent.p[parent.pixelR[(int)((posX+moveVector/2)*parent.width)+i][heightOfROI]-200].posX;
			found = true;
			break;
		}
		if((int)((posX+moveVector/2)*parent.width)-i < parent.width && (int)((posX+moveVector/2)*parent.width)-i >= 0 && parent.pixelR[(int)((posX+moveVector/2)*parent.width)-i][heightOfROI] != 0)
		{
			cout << "found occluded\n";
			posX = parent.p[parent.pixelR[(int)((posX+moveVector/2)*parent.width)-i][heightOfROI]-200].posX;
			found = true;
			break;
		}
	}
	return found;
}
void Picture::coutPersons()
{
	//system("cls");
	for(int i = 0; i < 50; i++)
	{
		if(p[i].posX != -1)
		{
			
			cout << "id: " << p[i].id << " id: " << p[i].id << " pos: " << p[i].posX << "\n"; 
		}

	}
}
void Picture::clipPersonsAll(int heightOfUpperFOI, int procentOfScreenUsedInTheUpperFOI)
{
	ostringstream ss;
	if (p[0].posX == -1)
	{
		if(alreadySendZero)
			return;
		alreadySendZero = true;
	}
	else
		alreadySendZero = false;

	ss << "q";
	for(int i = 0; i < 10; i++)
	{
		if(p[i].posX == -1)
			p[i].id = -1;
		//if (p[i].heightOfROI == heightOfUpperFOI)
		//{
		//	//perspectiv view adjustment:
		//	ss <<" i"<< p[i].id << "p" << (p[i].posX*(procentOfScreenUsedInTheUpperFOI/100) + (((100-procentOfScreenUsedInTheUpperFOI)/200)*width));
		//}
		//else
			ss <<" i"<< p[i].id << "p" << p[i].posX;
	}
	ss << " q";
	string s(ss.str());
	clipboard(s);
}
void Picture::clipPersonsSmart()
{
	ostringstream ss;
	int i = 0;
	ss << "q";
	while(p[i].posX != -1)
	{
		ss <<" i"<< p[i].id << "p" << p[i].posX;
		i++;
	}
	ss << " q";
	string s(ss.str());
	clipboard(s);
}
void Picture::clipboard(const string &s)
{
  OpenClipboard(0);
  EmptyClipboard(); 
  HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size()+1);
  
  if (!hg)
  {
    CloseClipboard();
    return;
  }
  memcpy(GlobalLock(hg),s.c_str(),s.size());
  GlobalUnlock(hg);
  SetClipboardData(CF_TEXT,hg);
  CloseClipboard();
  GlobalFree(hg);
}
//void Picture::handleFoundPersons()
//{
//	for(int i = 0; i < maxNumberOfPersons; i++)
//		{
//			if(p[i].posX != -1)
//			{
//			float prePos = p[i].posX;
//			//cout << "program is trying to refind" << "\n";
//			//cout << currentPicture.p[i].posX;
//				if(!p[i].refind(currentPicture))
//				{
//				//either exited or occluded
//				//is the blob in the range so it can exit?
//					if(!((prePos*currentPicture.width) - currentPicture.maxAmountToMove > 0) || ((prePos*currentPicture.width) + currentPicture.maxAmountToMove < currentPicture.width))
//					{
//						//is not allowed to exit -> it must be occluded
//						if(!p[i].refindOccluded(currentPicture))
//						{
//							currentPicture.p[i].posX = -1;
//							cout << "there is a fuckup with a person that is neither exited or occluded\n";
//						}
//					
//					}
//					else 
//					{
//						currentPicture.p[i].posX = -1;
//						cout << "normal exit\n";
//					}
//
//				}
//			
//			currentPicture.p[i].moveVector = currentPicture.p[i].posX - prePos;
//			}
//		}
//}

void Picture::saveNumbersOfPersons()
{
	string data;

	ifstream inputFile;
	inputFile.open ("numbersOfPersonsEachDay.txt");
	getline(inputFile,data);
	inputFile.close();


	ofstream outputFile;
	outputFile.open("numbersOfPersonsEachDay.txt");
	outputFile << data << personCount << " ";
	outputFile.close();
}
void Picture::openOldMoveVector()
{
	float data;

	ifstream inputFile;
	inputFile.open ("moveVector.txt");
	//getline(inputFile,data);
	inputFile >> data;
	inputFile.close();
	if(data < 0)
	{
	//the file is empty
		
		cout << "the moveVector-file is either empty or nonexistend" << endl;
		//	<< "enter a guess for the first moveVector" << endl;
		//cin >> initialMoveVector;
		initialMoveVector = 0.2f;

		ofstream outputFile;
		outputFile.open("moveVector.txt");
		outputFile << 0 << endl << 0;
		outputFile.close();
	} 
	else
	{
		initialMoveVector = data;
		//convert the string data into the float initialMoveVector
		//stringstream ss;
		//ss << data;
		//ss >> initialMoveVector;
	}
}
void Picture::saveNewMoveVector()
{
	if(personCount <= 0)
	{
		cout << "there was no person, so the intialMoveVector is not set new";
		return;
	}

	string moveVector;
	string numberOfDays;
	int numberOfDaysInt;
	double moveVectorDouble;

	ifstream inputFile;
	inputFile.open ("moveVector.txt");
	//getline(inputFile,moveVector);
	//getline(inputFile,numberOfDays);
	inputFile >> moveVectorDouble;
	inputFile >> numberOfDaysInt;
	inputFile.close();

	//stringstream ss;
	//ss << numberOfDays;
	//ss >> numberOfDaysInt;

	//ss << moveVector;
	//ss >> moveVectorFloat;

	newInitialMoveVectorProduct /= personCount;

	moveVectorDouble *= numberOfDaysInt;
	moveVectorDouble += newInitialMoveVectorProduct;

	numberOfDaysInt++;
	moveVectorDouble /= numberOfDaysInt;

	ofstream outputFile;
	outputFile.open("moveVector.txt");
	outputFile << moveVectorDouble << endl << numberOfDaysInt;
	outputFile.close();
}
