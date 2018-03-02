#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"


class ofxFastIOImage{

public:


	//these save the pixel data as binary directly, no header no nothing
	//they will create an xml side-file to store metadata
	static bool saveTo( const ofPixels & data, const string & path );
	static bool loadFrom( ofPixels & data, const string & path );

	//those are optimized for speed and assume a certain header type
	//only tested with BMP's & TGAs spit out by quicktimePlayer (img sequence)
	//BMP: BGR & header of 54 bytes total
	//TGA: BGR & header of 14 bytes total
	//
	// Note that those image formated saved the image flipped to disk, so your image
	// willbe upside down. Up to you to flip the texture in GL (again)

	//convertToRGB delivers the pixels as RGB, otherwise you get BGR (as it's to disk - blame the img format )
	//converting comes with an extra CPU cost, so if you only want to draw the image,
	//there's no need to set convertToRGB to true

	//BMP's must be BGR & uncompressed! (ffmpeg converts to those by default)
	static bool loadFromBMP( ofPixels & data, const string & bmpPath, bool convertToRGB , bool fixFlip);

	//TGA's must be BGR & uncompressed!
	//from ffmpeg, you want to use this command to convert any movie to a valid img sequence
	//	$ ffmpeg -i inputMovie.mov -coder "raw" -y "outputFolder/output_%06d.tga"
	static bool loadFromTGA( ofPixels & data, const string & tgaPath, bool convertToRGB );

};

