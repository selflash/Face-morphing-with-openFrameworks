#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxDelaunay.h"

class ofApp : public ofBaseApp {
    public:
    
    protected:
        
    private:
        bool _debug = false;
    
        ofxFaceTracker _face1Tracker;
        ofImage _image_face1;
        ofxDelaunay* _delaunay_face1;
        ofMesh _mesh_face1;
        ofMesh _finalMesh_face1;
        std::vector<ofVec2f> _points1;
        std::vector<ofVec3f> _temp1;
        std::vector<ofVec3f*> _reference1;
        ofFbo _fbo1;
    
        ofxFaceTracker _face2Tracker;
        ofImage _image_face2;
        ofxDelaunay* _delaunay_face2;
        ofMesh _mesh_face2;
        ofMesh _finalMesh_face2;
        std::vector<ofVec2f> _points2;
        std::vector<ofVec3f> _temp2;
        std::vector<ofVec3f*> _reference2;
        ofFbo _fbo2;
    
//        ofShader _dissolveShader;
//        ofVboMesh _vboQuad;
    
        //For morphing.
        bool _isInterpolating = false;
        float _interpolationRatio = 1.0;

	
    public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    protected:
        
    private:
        void _detectFace1();
        void _detectFace2();
        
        void _updateFaceMesh1();
        void _updateFaceMesh2();
    
        void _updateReference1();
        void _updateReference2();
    
        void _drawFace1(float alpha = 1.0, bool debug = false);
        void _drawFace2(float alpha = 1.0, bool debug = false);
    
        void _drawWireFrame1();
        void _drawWireFrame2();
    
        void _morphing();
    
        void _debugDraw();
    
};
