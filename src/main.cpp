#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(640 * 3 * 0.85, 480 * 2 * 0.85, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
