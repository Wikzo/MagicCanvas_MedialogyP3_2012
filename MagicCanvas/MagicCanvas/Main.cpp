#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

void clipboard(const string &s);
void setupP(const int &numbersOfpersons, Picture::person personArray[]);

int main(){
	//find way to optimize the initial move vector
	const int maxNumberOfPersons = 50;
	

	ofstream myFile;
	//setup:
	point lowerLeftCornerOfHat; // point with two variables to connect the lower point of the image with the Red Line (head)
	int widthOfHat; // Red Line (real distance of the head)

	VideoCapture camera1;
	camera1.open(0);

	VideoCapture testVideo1;
	testVideo1.open("Library_Observation_Nov19.wmv");

	Picture currentPicture; 
	Picture BG; // 
	Picture tmpPicture;
	Picture hat;

	currentPicture.initialize(camera1);
	BG.initialize(camera1);
	tmpPicture.initialize(camera1); //This tmpPicture is used to temporarrily store the picture at different moments
	hat.initialize("nisse.jpg");
	
	tmpPicture.makeBlack(); // function to avoid colored pixels on the sisdes of the transformed image.
	currentPicture.minPixelToBeAPerson = 1000;
	currentPicture.radiusForMorfology = 3;
	currentPicture.numberOfPersons = 0;
	currentPicture.initialMoveVector = 0.1f;

	hat.output("Window for control");
	// gustav test vibe
	/*
	Picture images[20];
	//Picture::GrabMultipleBackgroundImages(camera1, images, 20);

	for (int i = 0; i < 20; i++)
	{
		images[i].openCamera(camera1);
		images[i].output("hey" + i);
	}*/
	setupP(maxNumberOfPersons, currentPicture.p);
	currentPicture.personCount = 0;
	currentPicture.maxAmountToMove = (int) (currentPicture.width*0.3f);

	while(true){ //To be played all the time.
		//currentPicture.refresh(testVideo1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't change
		//currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,10);
		//counter++;
		//cout << counter;
		currentPicture.refreshBGSubtractAndThreshholdForBnW(camera1,BG,30);

		// Closing
		
		//currentPicture.erode(currentPicture.radiusForMorfology, tmpPicture); // radius of 3 to erode and dilate
		//currentPicture.dilate(currentPicture.radiusForMorfology, tmpPicture);
		//currentPicture.findAllBLOBs(tmpPicture, person, maxNumberOfPersons);
		//currentPicture.lookForNewPersons(30, 100);
		//cout << currentPicture.numberOfPersons;
		//if(currentPicture.numberOfPersons > 0)
		//system("cls");

		for(int i = 0; i < maxNumberOfPersons; i++)
		{
			if(currentPicture.p[i].posX != -1)
			{
			float prePos = currentPicture.p[i].posX;
			//cout << currentPicture.p[i].posX;

			if(!currentPicture.p[i].refind(currentPicture))
			{
			//either exited or occluded
			//is the blob in the range so it can exit?
				if(!((prePos*currentPicture.width) - currentPicture.maxAmountToMove < 0) || ((prePos*currentPicture.width) + currentPicture.maxAmountToMove > currentPicture.width))
				{
					//is not allowed to exit -> it must be occluded
					if(!currentPicture.p[i].refindOccluded(currentPicture))
					{
						currentPicture.p[i].posX = -1;
						cout << "there is a fuckup with a person that is neither exited or occluded";
					}
					
				}
			}
			
			currentPicture.p[i].moveVector = currentPicture.p[i].posX - prePos;
			}
		}
		//currentPicture.numberOfPersons = 0;

		currentPicture.lookForNewPersons(20, currentPicture.height/2);

		// Hat size + draw
		
		//currentPicture.startFire(lowerLeftCornerOfHat, tmpPicture);

		//currentPicture.findAllBLOBs(tmpPicture, person, maxNumberOfPersons);
		//currentPicture.placeHats(50,10,lowerLeftCornerOfHat,widthOfHat, hat);
		//
		//currentPicture.findFirstRow(50, 10, lowerLeftCornerOfHat, widthOfHat); // 50, 10 --> messures how big should be the head.


		//send to file that is read by unity-----------------------------------------------------------------------------------------------------
		//ofstream myFile ("test.txt");

		//ostringstream ss;
		//for(int i = 0; i < currentPicture.numberOfPersons; i++)
		//{
		//	//if(myFile.is_open()){
		//		ss << person[i].posOnX << "p";
		//		//myFile << /*"\nThe " << i+1 << ". persons Xposition is: " <<*/"\n" << person[i].posOnX;
		//		cout << "\nThe " << i << ". persons Xposition is: " << person[i].posOnX;
		//	//} else { cout << "cant create file";}
		//}
		//string s(ss.str());
		//clipboard(s);

		//myFile.close();
		//--------------------------------------------------------------------------------------------------------------------------------------

		
		//if(widthOfHat > 0)
		//	currentPicture.drawPictureAt(lowerLeftCornerOfHat, widthOfHat, hat);
		//widthOfHat = 0;
		//
		currentPicture.output("video");
		
		int keyInput = waitKey(10);
		//cout << keyInput;
		if (keyInput == 115) // s
		{
			
			BG.refresh(camera1);
			BG.output("Window for control");
		}
		else if (keyInput == 27) // escape
		{
			cout << "\nEsc was pressed\nThe program exits when you press a key.";
			waitKey(0);
			return 0;
		}
	}
	currentPicture.reset();
	BG.reset();
}
//START CLIPBOARD
void clipboard(const string &s)
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
//SLUT CLIPBOARD

void setupP(const int &numbersOfpersons, Picture::person personArray[])
{
	for(int i = 0; i < numbersOfpersons; i++)
	{
		personArray[i].pId = i;
		personArray[i].posX = -1;
	}
}