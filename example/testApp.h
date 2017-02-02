#pragma once

#include "ofMain.h"
#include "ofxFastIOImage.h"
#include "ofxTimeMeasurements.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();

	ofImage original;
	ofImage original2;

	ofPixels loadedPix;
	ofTexture loadedTex;
	
};


