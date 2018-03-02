#include "ofxFastIOImage.h"
#include <iostream>
#include "ofxTimeMeasurements.h"


#ifndef PROFILE_FAST_IO_IMAGE
	#pragma push_macro("TS_START_NIF")
	#pragma push_macro("TS_STOP_NIF")
	#undef TS_START_NIF
	#undef TS_STOP_NIF
	#define TS_START_NIF
	#define TS_STOP_NIF
#endif

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
	TS_START_NIF("save bytes");
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
	TS_STOP_NIF("save bytes");
	TS_START_NIF("save xml");
	ofxXmlSettings xml;
	xml.setValue("w", (int)data.getWidth());
	xml.setValue("h", (int)data.getHeight());
	xml.setValue("imgType", data.getImageType());
	xml.saveFile(infoFile);
	TS_STOP_NIF("save xml");
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
		TS_START_NIF("alloc bytes");
		data.allocate(w, h, type); //internally, it wont realloc if its alreadyy alloc'd but it will overwrite the pixel format
		TS_STOP_NIF("alloc bytes");
		int bpp = 1;
		if (type == OF_IMAGE_COLOR ) bpp = 3;
		if (type == OF_IMAGE_COLOR_ALPHA ) bpp = 4;
		string fullPath = ofToDataPath(dataFile, true);

		TS_START_NIF("read bytes");
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
		TS_STOP_NIF("read bytes");
		return true;
	}
	return false;
}

void swapRB(ofPixels & pix) {
	size_t x;
	size_t len = pix.getTotalBytes();
	size_t skip = pix.getNumChannels();
	unsigned char * data = pix.getData();
	for(x = 0; x < len; x += skip) {
		std::swap(data[x], data[x+2]);
	}
}

void reverseBytes(unsigned char *start, size_t size) {
	unsigned char *lo = start;
	unsigned char *hi = start + size - 1;
	unsigned char swap;
	while (lo < hi) {
		swap = *lo;
		*lo++ = *hi;
		*hi-- = swap;
	}
}

bool ofxFastIOImage::loadFromBMP( ofPixels & data, const string & bmpPath, bool convertToRGB, bool fixFlip ){

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

			data.allocate(dim.w, dim.h, OF_PIXELS_BGR); //internally, it wont realloc if its alreadyy alloc'd but it will overwrite the pixel format

			fseek(f, 54, SEEK_SET); //skip to data
			n = fread((char*)data.getData(), dim.w * dim.h * 3, 1, f); //read img dimensions
			fclose(f);
			TS_STOP_NIF("read bytes BMP");

			if(fixFlip){
				TS_START_NIF("reverse bytes BMP");
				data.mirror(true, false);
				TS_STOP_NIF("reverse bytes BMP");
			}

			if(convertToRGB){
				TS_START_NIF("swap bytes BMP");
				data.swapRgb();
				TS_STOP_NIF("swap bytes BMP");
			}
			return true;
		}
	TS_STOP_NIF("read bytes BMP");
	return false;
}


bool ofxFastIOImage::loadFromTGA( ofPixels & data, const string & tgaPath, bool convertToRGB ){

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

		data.allocate(dim.w, dim.h, OF_PIXELS_BGR); //internally, it wont realloc if its alreadyy alloc'd but it will overwrite the pixel format

		fseek(f, 2, SEEK_CUR); //advance 2 - skip to data
		n = fread((char*)data.getData(), dim.w * dim.h * 3, 1, f);
		fclose(f);
		TS_STOP_NIF("read bytes TGA");
		if(convertToRGB){
			TS_START_NIF("swap bytes TGA");
			data.swapRgb();
			TS_STOP_NIF("swap bytes TGA");
		}
		return true;
	}

	TS_STOP_NIF("read bytes TGA");
	return false;
}


#ifndef PROFILE_FASTIOIMAGE
	#pragma pop_macro("TS_START_NIF")
	#pragma pop_macro("TS_STOP_NIF")
#endif
