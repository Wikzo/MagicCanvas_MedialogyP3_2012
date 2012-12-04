#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

/*TODOs 
don't go throung the hole picture in the beginning - Done
get the speed of each person in the beginning and make the initial movevector adapt - Done needs to be tested
setupBG autofunction - Done

occlusion instead of midtpoint
improve recursion 
automized setup function (both take bg and find height of the upper loi) - first in the libary
seperate the upper from the lower analysis (when low discard high)
*/
void configBG(Picture &BG, VideoCapture &camera1, int threshholdPixelChange, int threshholdPixelsChanged, int threshholdFramesChanged);
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
	currentPicture.minPixelToBeAPerson = 100;
	currentPicture.radiusForMorfology = 3;
	currentPicture.numberOfPersons = 0;
	currentPicture.initialMoveVector = 0.1f;

	hat.output("Window for control");

	setupP(maxNumberOfPersons, currentPicture.p);
	currentPicture.personCount = 0;
	currentPicture.maxAmountToMove = (int) (currentPicture.width*0.2f);
	currentPicture.makeBlack();

	currentPicture.newInitialMoveVectorProduct = 0;
	currentPicture.openOldMoveVector();

	configBG(BG, camera1, 100, 600, 5);

	while(true){ //To be played all the time.
		//currentPicture.refresh(testVideo1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't changeconf
		//currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,10);
		//currentPicture.refreshBGSubtractAndThreshholdForBnW(camera1,BG,30);
		currentPicture.refreshDiscradBGSubtractAndThreshholdForBnW(camera1,BG,30, 20, currentPicture.height/2);
		// Closing
		
		//currentPicture.erode(currentPicture.radiusForMorfology, tmpPicture); // radius of 3 to erode and dilate
		//currentPicture.dilate(currentPicture.radiusForMorfology, tmpPicture);

		for(int i = 0; i < maxNumberOfPersons; i++)
		{
			if(currentPicture.p[i].posX != -1) // <- does the person exist?
			{
			float prePos = currentPicture.p[i].posX; // <-save the current position
			//cout << "program is trying to refind" << "\n";
			//cout << currentPicture.p[i].posX;
				if(!currentPicture.p[i].refind(currentPicture)) // <- find the new position of the person, when not found do the following:
				{
					if(currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet) // <- when the initial movevector isn't mesured yet
					{
						if(currentPicture.personCount-1 >= 0) // <- when this is not the first person
						{
							currentPicture.newInitialMoveVectorProduct += currentPicture.newInitialMoveVectorProduct/(currentPicture.personCount-1); // <- add the average of the until now found initial move vectors to the move vector product
							currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet = false; // <- and make shure that it isn't added again
						}
						else // when this is the first person:
						{
							currentPicture.newInitialMoveVectorProduct = currentPicture.initialMoveVector; // <- since there is nothig to take the average from take the movevector the prgram started with
							currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet = false; // <- and make shure that it isn't added again
						}
					}
				//either exited or occluded
				//is the blob in the range so it can exit?
					if(!((prePos*currentPicture.width) - currentPicture.maxAmountToMove > 0) || ((prePos*currentPicture.width) + currentPicture.maxAmountToMove < currentPicture.width)) // check if the person is NOT close enogth to the edge of the picture to exit
					{
						//is not allowed to exit -> it must be occluded
						if(!currentPicture.p[i].refindOccluded(currentPicture))
						{
							currentPicture.p[i].posX = -1;
							cout << "there is a fuckup with a person that is neither exited or occluded\n";
						}
					
					}
					else 
					{
						currentPicture.p[i].posX = -1;
						cout << "normal exit\n";
					}

				}
				currentPicture.p[i].moveVector = currentPicture.p[i].posX - prePos;
				if(currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet)
				{
					if(currentPicture.p[i].moveVector < 0)
						currentPicture.newInitialMoveVectorProduct += -currentPicture.p[i].moveVector;
					else
						currentPicture.newInitialMoveVectorProduct += currentPicture.p[i].moveVector;
					currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet = false;
				}
			}
		}

		currentPicture.lookForNewPersons(20, currentPicture.height/2);

		currentPicture.coutPersons();

		currentPicture.clipPersonsAll();

#pragma region how to write to a file
		//ofstream myFile ("test.txt");
		//for(int i = 0; i < currentPicture.numberOfPersons; i++)
		//{
		//	if(myFile.is_open()){
		//		myFile << "\nThe " << i+1 << ". persons Xposition is: " << "\n" << currentPicture.p[i].posX;
		//	} else { cout << "cant create file";}
		//}
		//myFile.close();
#pragma endregion

		currentPicture.output("video");
		
		int keyInput = waitKey(10);
		//cout << keyInput;
		if (keyInput == 115) // <-s
		{
			
			BG.refresh(camera1);
			BG.output("Window for control");
		}
		else if (keyInput == 27) // <-escape
		{
			currentPicture.saveNumbersOfPersons();
			currentPicture.saveNewMoveVector();
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
void configBG(Picture &BG, VideoCapture &camera1, int threshholdPixelChange, int threshholdPixelsChanged, int threshholdFramesChanged)
{
	Picture test1;
	Picture test2;
	test1.initialize(camera1);
	test2.initialize(camera1);

	double framesUnchangeged = 0;
	while(true)
	{
		test1.refresh(camera1);
		waitKey(30);
		test2.refresh(camera1);
		double pixelChange = 0;
		double pixelsChanged = 0;
		

		for(int x = 0; x < test1.width; x++)
		{
			for(int y = 0; y < test1.height; y++)
			{
				pixelChange = 0;
				if(test1.pixelR[x][y] - test2.pixelR[x][y] > 0)
					pixelChange += test1.pixelR[x][y] - test2.pixelR[x][y];
				else
					pixelChange += test2.pixelR[x][y] - test1.pixelR[x][y];

				if(test1.pixelG[x][y] - test2.pixelG[x][y] > 0)
					pixelChange += test1.pixelG[x][y] - test2.pixelG[x][y];
				else
					pixelChange += test2.pixelG[x][y] - test1.pixelG[x][y];

				if(test1.pixelB[x][y] - test2.pixelB[x][y] > 0)
					pixelChange += test1.pixelB[x][y] - test2.pixelB[x][y];
				else
					pixelChange += test2.pixelB[x][y] - test1.pixelB[x][y];
				if(pixelChange > threshholdPixelChange)
					pixelsChanged++;
				//cout << pixelChange << endl;

			}
		}
		cout << pixelsChanged << endl;
		if(pixelsChanged < threshholdPixelsChanged)
			framesUnchangeged++;
		else
			framesUnchangeged = 0;

		if(framesUnchangeged >= threshholdFramesChanged)
		{
			BG.refresh(camera1);
			return;
		}
		//pixelChange /= test1.width;
		//pixelChange /= test1.height;
		//cout << pixelChange << endl;

		//		if(!((test1.pixelB[x][y] - test2.pixelB[x][y] > threshhold || test1.pixelB[x][y] - test2.pixelB[x][y] < -1*threshhold)
		//			|| (test1.pixelR[x][y] - test2.pixelR[x][y] > threshhold || test1.pixelR[x][y] - test2.pixelR[x][y] < -1*threshhold)
		//			|| (test1.pixelG[x][y] - test2.pixelG[x][y] > threshhold || test1.pixelG[x][y] - test2.pixelG[x][y] < -1*threshhold)))
		//		{
		//			picturesWithoutChanges = 1;
		//		}
		//	}
		//}
		//if(picturesWithoutChanges == 1)
		//{
		//	BG.refresh(camera1);
		//	return;

		//}

		////return 0;
	}
}