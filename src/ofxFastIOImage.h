#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxFastIOImage{

public:
	
	static bool saveTo( const ofPixels & data, const string & path );
	static bool loadFrom( ofPixels & data, const string & path );
};

