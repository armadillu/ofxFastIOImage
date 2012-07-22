#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxFastIOImage{

public:
	
	static bool saveTo( ofImage * img, string path );
	static ofImage* loadFrom( string path );
};

