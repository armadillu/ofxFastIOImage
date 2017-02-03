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
		if(data.getWidth() != w || data.getHeight() != h || data.getImageType() != type ){
			TS_START("alloc bytes");
			data.allocate(w, h, type);
			TS_STOP("alloc bytes");
		}
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


bool ofxFastIOImage::loadFromBMP( ofPixels & data, const string & bmpPath ){

	string fullPath = ofToDataPath(bmpPath, true);

	TS_START_NIF("read bytes BMP");
		FILE * f = fopen(fullPath.c_str(), "rb");
		if (f){
			struct bmpDim{
				uint32_t w;
				uint32_t h;
			};

			bmpDim dim;
			int n;
			fseek(f, 18, SEEK_CUR); //skip to img dimensions
			n = fread((char*)&dim, 8, 1, f); //read img dimensions

			if(data.getWidth() != dim.w || data.getHeight() != dim.h || data.getImageType() != OF_IMAGE_COLOR ){
				TS_START("alloc BMP");
				data.allocate(dim.w, dim.h, OF_PIXELS_RGB);
				TS_STOP("alloc BMP");
			}

			fseek(f, 54, SEEK_SET); //skip to data
			n = fread((char*)data.getData(), dim.w * dim.h * 3, 1, f); //read img dimensions
			fclose(f);
			TS_STOP_NIF("read bytes BMP");
			return true;
		}

	TS_STOP_NIF("read bytes BMP");
	return false;
}




bool ofxFastIOImage::loadFromTGA( ofPixels & data, const string & tgaPath ){

	string fullPath = ofToDataPath(tgaPath, true);

	TS_START_NIF("read bytes TGA");
	FILE * f = fopen(fullPath.c_str(), "rb");
	if (f){
		struct tgaDim{
			uint16_t w;
			uint16_t h;
		};

		tgaDim dim;
		int n;
		fseek(f, 12, SEEK_CUR); //skip to img dimensions
		n = fread((char*)&dim, 4, 1, f); //read img dimensions

		if(data.getWidth() != dim.w || data.getHeight() != dim.h || data.getImageType() != OF_IMAGE_COLOR ){
			TS_START("alloc TGA");
			data.allocate(dim.w, dim.h, OF_PIXELS_RGB);
			TS_STOP("alloc TGA");
		}

		fseek(f, 2, SEEK_CUR); //advance 2 - skip to data
		n = fread((char*)data.getData(), dim.w * dim.h * 3, 1, f);
		fclose(f);
		TS_STOP_NIF("read bytes TGA");
		return true;
	}

	TS_STOP_NIF("read bytes TGA");
	return false;
}


