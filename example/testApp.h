#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxFastIOImage.h"
#include "ofxTimeMeasurements.h"

class testApp : public ofxiPhoneApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs & touch){};
	void touchMoved(ofTouchEventArgs & touch){};
	void touchUp(ofTouchEventArgs & touch){};
	void touchDoubleTap(ofTouchEventArgs & touch){};
	void touchCancelled(ofTouchEventArgs & touch){};
	
	ofImage original;
	ofImage * loaded;
	ofImage original2;
	
};


