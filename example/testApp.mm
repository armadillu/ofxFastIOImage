#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	
	ofxiPhoneSetOrientation( OF_ORIENTATION_90_LEFT );
	
	//make retina devices upscale with nearest (not default linear)
	EAGLView * view = ofxiPhoneGetGLView();
	view.layer.magnificationFilter = kCAFilterNearest;

	
	ofEnableAlphaBlending();	
	ofBackground(127,127,127);
	
	TIME_SAMPLE_SET_AVERAGE_RATE(0.5);
	loaded = NULL;
}


void testApp::update(){
	
	// png OF
	TIME_SAMPLE_START("load OF");
	original.loadImage("rgba.png");
	TIME_SAMPLE_STOP("load OF");

	TIME_SAMPLE_START("save OF");
	original.saveImage(ofxiPhoneGetDocumentsDirectory() + "rgba_out.png");
	TIME_SAMPLE_STOP("save OF");

	// jpeg OF
	TIME_SAMPLE_START("load jpeg OF");
	original2.loadImage("nemo.jpg");
	TIME_SAMPLE_STOP("load jpeg OF");

	TIME_SAMPLE_START("save jpeg OF");
	original2.saveImage(ofxiPhoneGetDocumentsDirectory() + "nemo2.jpg");
	TIME_SAMPLE_STOP("save jpeg OF");
	
	//ofxFastIOImage
	TIME_SAMPLE_START("save ofxFastIOImage");
	ofxFastIOImage::saveTo( &original, ofxiPhoneGetDocumentsDirectory() + "fast" );
	TIME_SAMPLE_STOP("save ofxFastIOImage");
	
	TIME_SAMPLE_START("load ofxFastIOImage");
	if (loaded) delete loaded;
	loaded = ofxFastIOImage::loadFrom( ofxiPhoneGetDocumentsDirectory() + "fast" );
	TIME_SAMPLE_STOP("load ofxFastIOImage");
}

//--------------------------------------------------------------
void testApp::draw(){

	int ww = 200;
	original.draw(0, 0, ww, ww);
	if(loaded) loaded->draw(ww, 0, ww, ww);
	
	TIME_SAMPLE_DRAW(10, ww + 10);
}

