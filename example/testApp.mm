#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	

	ofEnableAlphaBlending();	
	ofBackground(127,127,127);
	
	TIME_SAMPLE_SET_AVERAGE_RATE(0.5);
}


void testApp::update(){
	
	// png OF
	TIME_SAMPLE_START("load PNG OF");
	original.loadImage("rgba.png");
	TIME_SAMPLE_STOP("load PNG OF");

	TIME_SAMPLE_START("save PNG OF");
	original.saveImage("rgba_out.png");
	TIME_SAMPLE_STOP("save PNG OF");

	// jpeg OF
	TIME_SAMPLE_START("load jpeg OF");
	original2.loadImage("nemo.jpg");
	TIME_SAMPLE_STOP("load jpeg OF");

	TIME_SAMPLE_START("save jpeg OF");
	original2.saveImage("nemo_out.jpg");
	TIME_SAMPLE_STOP("save jpeg OF");
	
	//ofxFastIOImage
	TIME_SAMPLE_START("save ofxFastIOImage");
	ofxFastIOImage::saveTo( original.getPixels(), "fast" );
	TIME_SAMPLE_STOP("save ofxFastIOImage");
	
	TIME_SAMPLE_START("load ofxFastIOImage");
	ofxFastIOImage::loadFrom( loadedPix, "fast" );
	TIME_SAMPLE_STOP("load ofxFastIOImage");

	TS_START("uploadPixels");
	loadedTex.loadData(loadedPix);
	TS_STOP("uploadPixels");
}

//--------------------------------------------------------------
void testApp::draw(){

	int ww = 200;
	original.draw(0, 0, ww, ww);

}

