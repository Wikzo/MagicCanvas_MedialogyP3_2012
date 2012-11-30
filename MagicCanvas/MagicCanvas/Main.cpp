#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

/*TODOs 
occlusion instead of midtpoint
improve recursion 
don't go throung the hole picture in the beginning - Done
get the speed of each person in the beginning and make the initial movevector adapt
automized setup function (both take bg and find height of the upper loi)
seperate the upper from the lower analysis
write stats (how many, how long etc.) how many is done
flip values
*/
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

	setupP(maxNumberOfPersons, currentPicture.p);
	currentPicture.personCount = 0;
	currentPicture.maxAmountToMove = (int) (currentPicture.width*0.2f);
	currentPicture.makeBlack();

	currentPicture.newInitialMoveVectorProduct = 0;
	currentPicture.openOldMoveVector();

	while(true){ //To be played all the time.
		//currentPicture.refresh(testVideo1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't change
		//currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,10);
		//currentPicture.refreshBGSubtractAndThreshholdForBnW(camera1,BG,30);
		currentPicture.refreshDiscradBGSubtractAndThreshholdForBnW(camera1,BG,30, 20, currentPicture.height/2);
		// Closing
		
		//currentPicture.erode(currentPicture.radiusForMorfology, tmpPicture); // radius of 3 to erode and dilate
		//currentPicture.dilate(currentPicture.radiusForMorfology, tmpPicture);

		for(int i = 0; i < maxNumberOfPersons; i++)
		{
			if(currentPicture.p[i].posX != -1)
			{
			float prePos = currentPicture.p[i].posX;
			//cout << "program is trying to refind" << "\n";
			//cout << currentPicture.p[i].posX;
				if(!currentPicture.p[i].refind(currentPicture))
				{
					if(currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet)
					{
						if(currentPicture.personCount-1 > 0)
						{
							currentPicture.newInitialMoveVectorProduct += currentPicture.newInitialMoveVectorProduct/(currentPicture.personCount-1);
							currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet = false;
						}
						else
						{
							currentPicture.newInitialMoveVectorProduct = currentPicture.initialMoveVector;
							currentPicture.p[i].notAddedToTheNewInitialMoveVectorProductYet = false;
						}
					}
				//either exited or occluded
				//is the blob in the range so it can exit?
					if(!((prePos*currentPicture.width) - currentPicture.maxAmountToMove > 0) || ((prePos*currentPicture.width) + currentPicture.maxAmountToMove < currentPicture.width))
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