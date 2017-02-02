#include "ofxFastIOImage.h"
#include <iostream>


bool ofxFastIOImage::saveTo( const ofPixels & data, const string & path ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;

	ofDirectory d;
	bool ok = d.createDirectory( path );
	if (!ok) return ok;
	int len = data.getWidth() * data.getHeight();
	    
	switch ( data.getImageType() ) {
		case  OF_IMAGE_COLOR_ALPHA:
			len *= 4;
			break;
		case  OF_IMAGE_COLOR:
			len *= 3;
			break;
		case  OF_IMAGE_GRAYSCALE:
			len = len;
			break;
	}
	
    ofstream myFile (dataFile.c_str(), ios::out | ios::binary);
    myFile.write ((char*)data.getPixels(), len);
	
	ofxXmlSettings xml;
	xml.setValue("w", (int)data.getWidth());
	xml.setValue("h", (int)data.getHeight());
	xml.setValue("imgType", data.getImageType());
	xml.saveFile(infoFile);
	return ok;
}


bool ofxFastIOImage::loadFrom( ofPixels & data, const string & path  ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;

	ofxXmlSettings xml;
	bool ok = xml.loadFile( infoFile);
	if (ok){
		int w = xml.getValue("w", 0);
		int h = xml.getValue("h", 0);
		ofImageType type = (ofImageType)xml.getValue("imgType", OF_IMAGE_UNDEFINED);
		data.allocate(w, h, type);
		int bpp = 1;
		if (type == OF_IMAGE_COLOR ) bpp = 3;
		if (type == OF_IMAGE_COLOR_ALPHA ) bpp = 4;		
		ifstream myFile (dataFile.c_str(), ios::in | ios::binary);
		myFile.read((char*)data.getData(), w * h * bpp);
		return true;
	}
	return false;
}
