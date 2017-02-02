#include "ofxFastIOImage.h"
#include <iostream>
#include "ofxTimeMeasurements.h"

bool ofxFastIOImage::saveTo( const ofPixels & data, const string & path ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;

	ofDirectory d;
	bool ok = d.createDirectory( path, true, true );
	if (!ok) return ok;
	size_t len = data.getWidth() * data.getHeight();
	    
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

	string fullPath = ofToDataPath(dataFile, true);
	TS_START("save bytes");
	#if 1
    	ofstream myFile (fullPath.c_str(), ios::out | ios::binary);
    	myFile.write((char*)data.getData(), len);
	#else
		FILE * f = fopen(fullPath.c_str(), "wb");
		int n;
		if (f){
			n = fwrite((char*)data.getData(), 1, len, f);
			fclose(f);
		}else{
			ofLogError() << "can't save binary data to '" << fullPath << "'";
		}
	#endif
	TS_STOP("save bytes");
	TS_START("save xml");
	ofxXmlSettings xml;
	xml.setValue("w", (int)data.getWidth());
	xml.setValue("h", (int)data.getHeight());
	xml.setValue("imgType", data.getImageType());
	xml.saveFile(infoFile);
	TS_STOP("save xml");
	return ok;
}


bool ofxFastIOImage::loadFrom( ofPixels & data, const string & path ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;

	TS_START_NIF("load xml");
	ofxXmlSettings xml;
	bool ok = xml.loadFile( infoFile);
	TS_STOP_NIF("load xml");
	if (ok){
		size_t w = xml.getValue("w", 0);
		size_t h = xml.getValue("h", 0);
		ofImageType type = (ofImageType)xml.getValue("imgType", OF_IMAGE_UNDEFINED);
		data.allocate(w, h, type);
		int bpp = 1;
		if (type == OF_IMAGE_COLOR ) bpp = 3;
		if (type == OF_IMAGE_COLOR_ALPHA ) bpp = 4;
		string fullPath = ofToDataPath(dataFile, true);

		TS_START("read bytes");
		#if 1
			FILE * f = fopen(fullPath.c_str(), "rb");
			int n;
			if (f){
				n = fread((char*)data.getData(), w * h * bpp, 1, f);
				fclose(f);
			}
		#else
			ifstream myFile (fullPath.c_str(), ios::in | ios::binary);
			myFile.read((char*)data.getData(), w * h * bpp);
		#endif
		TS_STOP("read bytes");
		return true;
	}
	return false;
}
