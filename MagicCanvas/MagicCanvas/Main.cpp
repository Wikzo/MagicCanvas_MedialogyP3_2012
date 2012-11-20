#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "Picture.h"
using namespace std;
using namespace cv;

int main(){
	//setup:
	point lowerLeftCornerOfHat; // point with two variables to connect the lower point of the image with the Red Line (head)
	int widthOfHat; // Red Line (real distance of the head)

	VideoCapture camera1;
	camera1.open(1);

	Picture currentPicture; 
	Picture BG; // 
	Picture tmpPicture;
	Picture hat;

	currentPicture.initialize(camera1);
	BG.initialize(camera1);
	tmpPicture.initialize(camera1); //This tmpPicture is used to temporarrily store the picture at different moments
	hat.initialize("nisse.jpg");
	
	tmpPicture.makeBlack(); // function to avoid colored pixels on the sisdes of the transformed image.

	while(true){ //To be played all the time.
		currentPicture.refresh(camera1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't change
		//currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,25);
		//
		//// Closing
		//currentPicture.erode(3, tmpPicture); // radius of 3 to erode and dilate
		//currentPicture.dilate(3, tmpPicture);

		//// Hat size + draw
		//
		////currentPicture.startFire(lowerLeftCornerOfHat, tmpPicture);

		////currentPicture.findAllBLOBs(tmpPicture);
		////currentPicture.placeHats(50,10,lowerLeftCornerOfHat,widthOfHat, hat);
		////
		//currentPicture.findFirstRow(50, 10, lowerLeftCornerOfHat, widthOfHat); // 50, 10 --> messures how big should be the head.
		//if(widthOfHat > 0)
		//	currentPicture.drawPictureAt(lowerLeftCornerOfHat, widthOfHat, hat);
		//widthOfHat = 0;
		
		currentPicture.output("video");
		
		int keyInput = waitKey(10);
		//cout << keyInput;
		if (keyInput == 115) // s
		{
			
			BG.refresh(camera1);
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
