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
	camera1.open(0);
	Picture currentPicture; 
	Picture BG; // 
	BG.openCamera(camera1);
	Picture tmpPicture;
	//This tmpPicture is used to temporarrily store the picture at different moments
	tmpPicture.openCamera(camera1);
	tmpPicture.makeBlack(); // function to avoid colored pixels on the sisdes of the transformed image.
	Picture hat;
	hat.openFile("nisse.jpg");
	


	while(true){ //To be played all the time.
		currentPicture.openCamera(camera1);
		//BG subtraction with threshold to detect the diferences on the pixels and transform to black the pixels that didn't change
		currentPicture.binaryPictureOfWhatMovedInComparrisionTo(BG,50);
		
		// Opening + closing
		currentPicture.erode(3, tmpPicture); // radius of 3 to erode and dilate
		currentPicture.dilate(3, tmpPicture);
		currentPicture.dilate(3, tmpPicture);
		currentPicture.erode(3, tmpPicture);

