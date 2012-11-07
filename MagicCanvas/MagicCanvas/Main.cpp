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
	int** pixelR;
	int** pixelG;
	int** pixelB;
	void openFile(string name);
	void openCamera(VideoCapture captureToStoreCamra);
	void binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold);
	void output();
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
int main(){
	//setup:
	point lowerLeftCornerOfHat;
	int widthOfHat;

	VideoCapture camera1;
	camera1.open(0);
	Picture testPicture;
	Picture BG;
	BG.openCamera(camera1);
	Picture tmpPicture;
	//This tmpPicture is used to temporarrily store the picture at different moments
	tmpPicture.openCamera(camera1);
	tmpPicture.makeBlack();
	Picture hat;
	hat.openFile("nisse.jpg");
	


	while(true){
		testPicture.openCamera(camera1);
		testPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,50);
		testPicture.erode(3, tmpPicture);
		testPicture.dilate(3, tmpPicture);
		testPicture.dilate(3, tmpPicture);
		testPicture.erode(3, tmpPicture);
		testPicture.findFirstRow(50, 10, lowerLeftCornerOfHat, widthOfHat);
		if(widthOfHat > 0)
			testPicture.drawPictureAt(lowerLeftCornerOfHat, widthOfHat, hat);
		widthOfHat = 0;
		testPicture.output();
		testPicture.reset();
		int keyInput = waitKey(10);
		//cout << keyInput;
		if (keyInput == 115)
		{
			BG.reset();
			BG.openCamera(camera1);
		}
		else if (keyInput == 27) 
		{
			cout << "\nEsc was pressed\nThe program exits when you press a key.";
			waitKey(0);
			return 0;
		}
	}
	/*
	for(int i = 0; i < 3; i++)
	{
		delete [] testPicture.pixel[i];
	}
	delete [] testPicture.pixel;
	*/
	
}

void Picture::openFile(string name)
{
	Mat tmp;
	tmp = imread(name);
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
void Picture::output()
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
	imshow("output", out);
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
void Picture::openCamera(VideoCapture captureToStoreCamra)
{
	Mat tmp;
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
void Picture::binaryPictureOfWhatMovedInComparrisionTo(Picture refPicture, int threshhold)
{
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			if((pixelB[x][y] - refPicture.pixelB[x][y] > threshhold || pixelB[x][y] - refPicture.pixelB[x][y] < -1*threshhold) || (pixelR[x][y] - refPicture.pixelR[x][y] > threshhold || pixelR[x][y] - refPicture.pixelR[x][y] < -1*threshhold) || (pixelG[x][y] - refPicture.pixelG[x][y] > threshhold || pixelG[x][y] - refPicture.pixelG[x][y] < -1*threshhold))
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
				
				while((pixelG[x+1][y] == 255 || pixelG[x+2][y] == 255 || pixelG[x+3][y] == 255) && x < width-4)
				{
					
					pixelR[x][y] = 255;
					pixelG[x][y] = 0;
					pixelB[x][y] = 0;
					x++;
						
				}
				lengthOfTheLine = x - startOfTheLine.x;
			}
		}
	}
}
void Picture::drawPictureAt(point lowerLeftCorner, int newwidth, Picture pictureToDraw)
{
	float sf = (float)pictureToDraw.width/newwidth;

	int newheight = pictureToDraw.height/sf;


	cout << "x: " << lowerLeftCorner.x << " y: " << lowerLeftCorner.y << " width: " << newwidth << " height: " << newheight << " sf: " << sf <<"\n";
	
	
	for(int x = 0; x < newwidth; x++){
		for(int y = 0; y < newheight; y++){
			if((lowerLeftCorner.y - newheight + y) >= 0){ 
				pixelR[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelR[(int)(x*sf)][(int)(y*sf)];
				pixelG[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelG[(int)(x*sf)][(int)(y*sf)];
				pixelB[lowerLeftCorner.x + x][lowerLeftCorner.y - newheight + y] = pictureToDraw.pixelB[(int)(x*sf)][(int)(y*sf)];
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
