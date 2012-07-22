#include "ofxFastIOImage.h"
#include <fstream.h>


bool ofxFastIOImage::saveTo( ofImage * img, string path ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;

	ofDirectory d;
	bool ok = d.createDirectory( path );
	if (!ok) return ok;
	int len = img->getWidth() * img->getHeight();
	    
	switch ( img->getPixelsRef().getImageType() ) {

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
	
//	ofFile file;
//	file.open( dataFile, ofFile::WriteOnly, true );
//	ofBuffer b; 
//	b.set((char* )img->getPixels(), len );
//	file.writeFromBuffer(b);
//	file.close();

    ofstream myFile (dataFile.c_str(), ios::out | ios::binary);
    myFile.write ((char*)img->getPixels(), len);
	
	ofxXmlSettings xml;
	xml.setValue("w", img->getWidth());
	xml.setValue("h", img->getHeight());
	xml.setValue("imgType", img->getPixelsRef().getImageType());
	xml.saveFile(infoFile);
	return ok;
}


ofImage * ofxFastIOImage::loadFrom( string path ){

	string dataFile = path + "/data.bin" ;
	string infoFile = path + "/info.xml" ;
	
	ofxXmlSettings xml;
	bool ok = xml.loadFile( infoFile);
	if (ok){
		int w = xml.getValue("w", 0);
		int h = xml.getValue("h", 0);
		ofImageType type = (ofImageType)xml.getValue("imgType", OF_IMAGE_UNDEFINED);
		
		ofImage * img = new ofImage();
		img->allocate(w, h,type);
		int bpp = 1;
		if (type == OF_IMAGE_COLOR ) bpp = 3;
		if (type == OF_IMAGE_COLOR_ALPHA ) bpp = 4;		
		
		unsigned char * tmp = (unsigned char*)malloc( sizeof(char) * w * h * bpp);
		
		ifstream myFile (dataFile.c_str(), ios::in | ios::binary);
		myFile.read((char*)tmp, w * h * bpp);
		//img->allocate(w, h,type);
		img->setFromPixels(tmp, w, h, type);
		free (tmp);
		return img;
	}
	return NULL;
}
