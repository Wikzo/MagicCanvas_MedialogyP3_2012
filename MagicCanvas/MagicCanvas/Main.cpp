#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

void clipboard(const string &s);

int main(){
	//find way to optimize the initial move vector
	const int maxNumberOfPersons = 50;
	Person person[maxNumberOfPersons]; // 15 is the max number of persons

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


	while(true){ //To be played all the time.
		//currentPicture.refresh(testVideo1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't change
		//currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,10);
		//counter++;
		//cout << counter;
		currentPicture.refreshBGSubtractAndThreshholdForBnW(camera1,BG,30);
		
		currentPicture.lookForNewPersons(20, currentPicture.height/2);
		// Closing
		
		//currentPicture.erode(currentPicture.radiusForMorfology, tmpPicture); // radius of 3 to erode and dilate
		//currentPicture.dilate(currentPicture.radiusForMorfology, tmpPicture);
		//currentPicture.findAllBLOBs(tmpPicture, person, maxNumberOfPersons);
		//currentPicture.lookForNewPersons(30, 100);
		//cout << currentPicture.numberOfPersons;
		if(currentPicture.numberOfPersons > 0)
		system("cls");
		for(int i = 0; i < currentPicture.numberOfPersons; i++)
		{
			
			cout << currentPicture.p[i].posX;
		}
		currentPicture.numberOfPersons = 0;

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
