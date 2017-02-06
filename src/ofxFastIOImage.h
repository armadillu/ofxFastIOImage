#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"


class ofxFastIOImage{

public:
	
	static bool saveTo( const ofPixels & data, const string & path );
	static bool loadFrom( ofPixels & data, const string & path );

	//those are optimized for speed and assume a certain header type
	//only tested with BMP's & TGAs spit out by quicktimePlayer (img sequence)
	//BMP: RGB & header of 54 bytes total
	//TGA: RGB & header of 14 bytes total
	static bool loadFromBMP( ofPixels & data, const string & bmpPath );
	static bool loadFromTGA( ofPixels & data, const string & tgaPath );
};

