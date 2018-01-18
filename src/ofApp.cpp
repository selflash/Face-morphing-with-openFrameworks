#include "ofApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
// SETUP / UPDATE / DRAW
//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(255);
    
	_face1Tracker.setup();
    if(_image_face1.load("images/face1.jpg")) cout << "image1 had loaded" << endl;
    _delaunay_face1 = new ofxDelaunay();
    _fbo1.allocate(_image_face1.getWidth(), _image_face1.getHeight(), GL_RGBA);
    
	_face2Tracker.setup();
	if(_image_face2.load("images/face2.jpg")) cout << "image2 had loaded" << endl;
    _delaunay_face2 = new ofxDelaunay();
    _fbo2.allocate(_image_face2.getWidth(), _image_face2.getHeight(), GL_RGBA);

//    _dissolveShader.load("shaders/dissolve");
    
    //------------------------------------------
//    int width = ofGetWidth();
//    int height = ofGetHeight();
//    _vboQuad.clear();
//    _vboQuad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//    _vboQuad.addVertex(ofVec3f(0, 0, 0));
//    _vboQuad.addVertex(ofVec3f(0, height, 0));
//    _vboQuad.addVertex(ofVec3f(width, 0, 0));
//    _vboQuad.addVertex(ofVec3f(width, height, 0));
//    
//    _vboQuad.addTexCoord(ofVec2f(0, 0));
//    _vboQuad.addTexCoord(ofVec2f(0, height));
//    _vboQuad.addTexCoord(ofVec2f(width, 0));
//    _vboQuad.addTexCoord(ofVec2f(width, height));
    //------------------------------------------
    
    ofEnableAlphaBlending();
    
    
    
    //------------------------------------------
    //setup
    _detectFace1();
    _detectFace2();
    
    _updateFaceMesh1();
    _updateFaceMesh2();
    
    _updateReference1();
    _updateReference2();
    //------------------------------------------
}

//--------------------------------------------------------------
void ofApp::update() {
    
    float temp;
    temp = (float)(ofGetMouseX() - ofGetWidth() * 0.5) / (float)(ofGetWidth() * 0.5);
    temp *= 2.0;
    temp += 0.5;
    if(temp < 0.0) temp = 0.0;
    if(temp > 1.0) temp = 1.0;
    _interpolationRatio += (temp - _interpolationRatio) * 0.1;
    
    _morphing();
    
//    _fbo1.begin();
//    ofClear(255, 255, 255, 0);
//    _drawFace1();
//    _fbo1.end();
//    
//    _fbo2.begin();
//    ofClear(255, 255, 255, 0);
//    _drawFace2();
//    _fbo2.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    if(_debug) {
        _debugDraw();
    } else {
        //----------------------------------
        ofPushMatrix();
        ofTranslate(ofGetWidth() * 0.5 - 640 * 1.75 * 0.5, ofGetHeight() * 0.5 - 480 * 1.75 * 0.5);
        
        ofScale(1.75, 1.75);
        _drawFace1(1.0);
        _drawFace2(_interpolationRatio);

        ofPopMatrix();
        
//        _dissolveShader.begin();
//        _dissolveShader.setUniformTexture("u_source", _fbo1.getTextureReference(), 0);
//        _dissolveShader.setUniformTexture("u_dist", _fbo2.getTextureReference(), 1);
//        _dissolveShader.setUniform1f("u_interpolationRatio", _interpolationRatio);
//        _vboQuad.draw();
//        _dissolveShader.end();
        //----------------------------------
    }
    
    //----------------------------------
    ofPushStyle();
	ofSetColor(0);
    
    std::string str = "";
    str += ofToString((int) ofGetFrameRate()) + "\n";
    str += "[Debug] Push a key\n";
	ofDrawBitmapString(str, 10, 20);
    ofPopStyle();
    //----------------------------------
}

//--------------------------------------------------------------
// PROTECTED / PRIVATE METHOD
//--------------------------------------------------------------

//--------------------------------------------------------------
//
void ofApp::_detectFace1() {
    _face1Tracker.reset();
	_face1Tracker.update(toCv(_image_face1));
    
    int imageWidth = _image_face1.getWidth();
    int imageHeight = _image_face1.getHeight();
    
	if(!_face1Tracker.getFound()) return;
    _mesh_face1 = _face1Tracker.getImageMesh();
    
    //----------------------------------
    _delaunay_face1->reset();
    _points1.clear();
    
    int i; int l;
    
    //----------------------------------
    //Add points of a face.
    l = _mesh_face1.getNumVertices();
    for(i = 0; i < l; i++) {
        _delaunay_face1->addPoint(_mesh_face1.getVertex(i));
        _points1.push_back(_mesh_face1.getVertex(i));
    }
    //----------------------------------
    
    //----------------------------------
    float scaleFactor;
    ofPolyline outline = _face1Tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    ofVec2f position = _face1Tracker.getPosition();
    //Add extended points of a contour of a face.
    scaleFactor = 1.5;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face1->addPoint(point);
        _points1.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 2.0;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face1->addPoint(point);
        _points1.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 2.5;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face1->addPoint(point);
        _points1.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 3.0;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face1->addPoint(point);
        _points1.push_back(point);
    }
    //Add extended points of a contour of a face.
//    scaleFactor = 3.5;
//    l = outline.size();
//    for(i = 0; i < l; i++) {
//        ofVec2f point((outline[i] - position) * scaleFactor + position);
//
//        _delaunay_face1->addPoint(point);
//        _points1.push_back(point);
//    }
    //----------------------------------
    
    //----------------------------------
    //Add 4 points to corners.
    _delaunay_face1->addPoint(ofVec2f(0, 0));
    _delaunay_face1->addPoint(ofVec2f(imageWidth, 0));
    _delaunay_face1->addPoint(ofVec2f(imageWidth, imageHeight));
    _delaunay_face1->addPoint(ofVec2f(0, imageHeight));
    
    _points1.push_back(ofVec2f(0, 0));
    _points1.push_back(ofVec2f(imageWidth, 0));
    _points1.push_back(ofVec2f(imageWidth, imageHeight));
    _points1.push_back(ofVec2f(0, imageHeight));
    //----------------------------------
    
    _delaunay_face1->triangulate();
}
//--------------------------------------------------------------
//
void ofApp::_detectFace2() {
    _face2Tracker.reset();
	_face2Tracker.update(toCv(_image_face2));
    
    int imageWidth = _image_face2.getWidth();
    int imageHeight = _image_face2.getHeight();
    
	if(!_face2Tracker.getFound()) return;
    _mesh_face2 = _face2Tracker.getImageMesh();
    
    //----------------------------------
    _delaunay_face2->reset();
    _points2.clear();
    
    int i; int l;
    
    //----------------------------------
    //Add points of a face.
    l = _mesh_face2.getNumVertices();
    for(i = 0; i < l; i++) {
        _delaunay_face2->addPoint(_mesh_face2.getVertex(i));
        _points2.push_back(_mesh_face2.getVertex(i));
    }
    //----------------------------------
    
    //----------------------------------
    float scaleFactor;
    ofPolyline outline = _face2Tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    ofVec2f position = _face2Tracker.getPosition();
    //Add extended points of a contour of a face.
    scaleFactor = 1.5;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face2->addPoint(point);
        _points2.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 2.0;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face2->addPoint(point);
        _points2.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 2.5;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face2->addPoint(point);
        _points2.push_back(point);
    }
    //Add extended points of a contour of a face.
    scaleFactor = 3.0;
    l = outline.size();
    for(i = 0; i < l; i++) {
        ofVec2f point((outline[i] - position) * scaleFactor + position);
        
        _delaunay_face2->addPoint(point);
        _points2.push_back(point);
    }
    //Add extended points of a contour of a face.
//    scaleFactor = 3.5;
//    l = outline.size();
//    for(i = 0; i < l; i++) {
//        ofVec2f point((outline[i] - position) * scaleFactor + position);
//
//        _delaunay_face2->addPoint(point);
//        _points2.push_back(point);
//    }
    //----------------------------------
    
    //----------------------------------
    //Add 4 points to corners.
    _delaunay_face2->addPoint(ofVec2f(0, 0));
    _delaunay_face2->addPoint(ofVec2f(imageWidth, 0));
    _delaunay_face2->addPoint(ofVec2f(imageWidth, imageHeight));
    _delaunay_face2->addPoint(ofVec2f(0, imageHeight));
    
    _points2.push_back(ofVec2f(0, 0));
    _points2.push_back(ofVec2f(imageWidth, 0));
    _points2.push_back(ofVec2f(imageWidth, imageHeight));
    _points2.push_back(ofVec2f(0, imageHeight));
    //----------------------------------
    
    _delaunay_face2->triangulate();
}

//--------------------------------------------------------------
void ofApp::_updateFaceMesh1() {
    int i; int l;
    ofMesh* triangleMesh;
    
    //----------------------------------
    triangleMesh = &_delaunay_face2->triangleMesh;
    
    std::vector<int> delaunayToFinal(triangleMesh->getNumVertices(), -1);
    std::vector<int> finalToDelaunay;
    
    //    int i; int l;
    int j; int k;
    int reindexed = 0;
    l = _mesh_face2.getNumVertices();
    for(i = 0; i < l; i++) {
        float minDistance = 0;
        int best = 0;
        
        k = triangleMesh->getNumVertices();
        for(j = 0; j < k; j++) {
            ofVec3f mesh_face2_vertex = _mesh_face2.getVertex(i);
            ofVec3f triangleMesh_vertex = triangleMesh->getVertex(j);
            float distance = triangleMesh_vertex.distance(mesh_face2_vertex);
            if(j == 0 || distance < minDistance) {
                minDistance = distance;
                best = j;
            }
        }
        
        delaunayToFinal[best] = reindexed++;
        finalToDelaunay.push_back(best);
    }
    
    l = delaunayToFinal.size();
    for(i = 0; i < l; i++) {
        if(delaunayToFinal[i] == -1) {
            delaunayToFinal[i] = reindexed++;
            finalToDelaunay.push_back(i);
        }
    }
    //----------------------------------
    
    //----------------------------------
    _finalMesh_face1.clear();
    _finalMesh_face1.setMode(OF_PRIMITIVE_TRIANGLES);
    
    l = delaunayToFinal.size();
    for(i = 0; i < l; i++) {
        int index = finalToDelaunay[i];
        _finalMesh_face1.addVertex(triangleMesh->getVertex(index));
        _finalMesh_face1.addTexCoord(triangleMesh->getVertex(index));
    }
    l = triangleMesh->getNumIndices();
    for(i = 0; i < l; i++) {
        _finalMesh_face1.addIndex(delaunayToFinal[triangleMesh->getIndex(i)]);
    }
}

//--------------------------------------------------------------
void ofApp::_updateFaceMesh2() {
    int i; int l;
    ofMesh* triangleMesh;
    
    //----------------------------------
    triangleMesh = &_delaunay_face1->triangleMesh;
    
    std::vector<int> delaunayToFinal(triangleMesh->getNumVertices(), -1);
    std::vector<int> finalToDelaunay;
    
    //    int i; int l;
    int j; int k;
    int reindexed = 0;
    l = _mesh_face1.getNumVertices();
    for(i = 0; i < l; i++) {
        float minDistance = 0;
        int best = 0;
        
        k = triangleMesh->getNumVertices();
        for(j = 0; j < k; j++) {
            ofVec3f mesh_face1_vertex = _mesh_face1.getVertex(i);
            ofVec3f triangleMesh_vertex = triangleMesh->getVertex(j);
            float distance = triangleMesh_vertex.distance(mesh_face1_vertex);
            if(j == 0 || distance < minDistance) {
                minDistance = distance;
                best = j;
            }
        }
        
        delaunayToFinal[best] = reindexed++;
        finalToDelaunay.push_back(best);
    }
    
    l = delaunayToFinal.size();
    for(i = 0; i < l; i++) {
        if(delaunayToFinal[i] == -1) {
            delaunayToFinal[i] = reindexed++;
            finalToDelaunay.push_back(i);
        }
    }
    //----------------------------------
    
    //----------------------------------
    _finalMesh_face2.clear();
    _finalMesh_face2.setMode(OF_PRIMITIVE_TRIANGLES);
    
    l = delaunayToFinal.size();
    for(i = 0; i < l; i++) {
        int index = finalToDelaunay[i];
        _finalMesh_face2.addVertex(triangleMesh->getVertex(index));
        _finalMesh_face2.addTexCoord(triangleMesh->getVertex(index));
    }
    l = triangleMesh->getNumIndices();
    for(i = 0; i < l; i++) {
        _finalMesh_face2.addIndex(delaunayToFinal[triangleMesh->getIndex(i)]);
    }
}

//--------------------------------------------------------------
//
void ofApp::_updateReference1() {
    _reference1.clear();
    _temp1.clear();
    
    int i; int l;
    int j; int k;
    
    l = _points1.size();
    for(i = 0; i < l; i++) {
        ofVec2f p1 = _points1[i];
        
        k = _finalMesh_face2.getNumVertices();
        for(j = 0; j < k; j++) {
            ofVec2f p2 = _finalMesh_face2.getVertices()[j];
            if(p2.x == p1.x && p2.y == p1.y) {
                _reference1.push_back(&_finalMesh_face2.getVertices()[j]);
                _temp1.push_back(p2);
            }
        }
    }
}
//--------------------------------------------------------------
//
void ofApp::_updateReference2() {
    _reference2.clear();
    _temp2.clear();
    
    int i; int l;
    int j; int k;
    
    l = _points2.size();
    for(i = 0; i < l; i++) {
        ofVec2f p1 = _points2[i];
        
        k = _finalMesh_face1.getNumVertices();
        for(j = 0; j < k; j++) {
            ofVec2f p2 = _finalMesh_face1.getVertices()[j];
            if(p2.x == p1.x && p2.y == p1.y) {
                _reference2.push_back(&_finalMesh_face1.getVertices()[j]);
                _temp2.push_back(p2);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::_drawFace1(float alpha, bool debug) {
    int i; int l;
    
    //--------------------------------------
    ofPushStyle();
    ofSetColor(255, 255, 255, alpha * 255);
    _image_face2.bind();
    _finalMesh_face1.drawFaces();
    _image_face2.unbind();
    ofPopStyle();
    //--------------------------------------

    if(!debug) return;
    
    //--------------------------------------
    ofPushStyle();
    ofSetColor(255, 0, 0);
    _finalMesh_face1.drawWireframe();
    ofPopStyle();
    //--------------------------------------

    //--------------------------------------
    l = _finalMesh_face1.getNumVertices();
    for(i = 0; i < l; i++) {
        ofVec3f position = _finalMesh_face1.getVertices()[i];
        
        ofPushMatrix();
        ofTranslate(position.x, position.y);
        
        ofPushStyle();
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(ofToString(i), -5, -5);
        ofPopStyle();
        
        ofPopMatrix();
    }
    //--------------------------------------
}
//--------------------------------------------------------------
void ofApp::_drawFace2(float alpha, bool debug) {
    int i; int l;
    
    //--------------------------------------
    ofPushStyle();
    ofSetColor(255, 255, 255, alpha * 255);
    _image_face1.bind();
    _finalMesh_face2.drawFaces();
    _image_face1.unbind();
    ofPopStyle();
    //--------------------------------------
    
    if(!debug) return;
    
    //--------------------------------------
    ofPushStyle();
    ofSetColor(255, 0, 0);
    _finalMesh_face2.drawWireframe();
    ofPopStyle();
    //--------------------------------------
    
    //--------------------------------------
    l = _finalMesh_face2.getNumVertices();
    for(i = 0; i < l; i++) {
        ofVec3f position = _finalMesh_face2.getVertices()[i];
        
        ofPushMatrix();
        ofTranslate(position.x, position.y);
        
        ofPushStyle();
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(ofToString(i), -5, -5);
        ofPopStyle();
        
        ofPopMatrix();
    }
    //--------------------------------------
}

//--------------------------------------------------------------
void ofApp::_drawWireFrame1() {
    ofMesh* triangulated;
    int i; int l;
    
    _image_face1.draw(0, 0);
    
    triangulated = &_delaunay_face1->triangleMesh;
    _image_face1.bind();
    ofPushStyle();
    ofSetColor(255, 0, 0);
    triangulated->drawWireframe();
    //    triangulated->drawFaces();
    ofPopStyle();
    _image_face1.unbind();
    
    l = _delaunay_face1->triangleMesh.getNumVertices();
    for(i = 0; i < l; i++) {
        ofPushMatrix();
        ofTranslate(_delaunay_face1->triangleMesh.getVertices()[i].x, _delaunay_face1->triangleMesh.getVertices()[i].y);
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawCircle(0, 0, 3);
        ofPopStyle();
        
        //        ofPushStyle();
        //        ofSetColor(0, 0, 0);
        //        ofDrawBitmapString(ofToString(i), -5, -5);
        //        ofPopStyle();
        
        ofPopMatrix();
    }
    l = _points1.size();
    for(i = 0; i < l; i++) {
        ofPushMatrix();
        ofTranslate(_points1[i].x, _points1[i].y);
        
        ofPushStyle();
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(ofToString(i), -5, -5);
        ofPopStyle();
        
        ofPopMatrix();
    }
}
//--------------------------------------------------------------
void ofApp::_drawWireFrame2() {
    ofMesh* triangulated;
    int i; int l;
    
    _image_face2.draw(0, 0);
    
    triangulated = &_delaunay_face2->triangleMesh;
    _image_face2.bind();
    ofPushStyle();
    ofSetColor(255, 0, 0);
    triangulated->drawWireframe();
    ofPopStyle();
    //
    //    triangulated->drawFaces();
    _image_face2.unbind();
    
    l = _delaunay_face2->triangleMesh.getNumVertices();
    for(i = 0; i < l; i++) {
        ofPushMatrix();
        ofTranslate(_delaunay_face2->triangleMesh.getVertices()[i].x, _delaunay_face2->triangleMesh.getVertices()[i].y);
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofDrawCircle(0, 0, 3);
        ofPopStyle();
        
        //        ofPushStyle();
        //        ofSetColor(0, 0, 0);
        //        ofDrawBitmapString(ofToString(i), -5, -5);
        //        ofPopStyle();
        
        ofPopMatrix();
    }
    l = _points2.size();
    for(i = 0; i < l; i++) {
        ofPushMatrix();
        ofTranslate(_points2[i].x, _points2[i].y);
        
        ofPushStyle();
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(ofToString(i), -5, -5);
        ofPopStyle();
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::_morphing() {
    int imageWidth;
    int imageHeight;
    int i; int l;
    
    imageWidth = _image_face1.getWidth();
    imageHeight = _image_face1.getHeight();
    l = _reference1.size();
    for(i = 0; i < l; i++) {
        _reference1[i]->x = _temp2[i].x * (1 - _interpolationRatio) + _temp1[i].x * _interpolationRatio;
        _reference1[i]->y = _temp2[i].y * (1 - _interpolationRatio) + _temp1[i].y * _interpolationRatio;
        
        if(_reference1[i]->x < 0) _reference1[i]->x = 0;
        if(_reference1[i]->x > imageWidth) _reference1[i]->x = imageWidth;
        if(_reference1[i]->y < 0) _reference1[i]->y = 0;
        if(_reference1[i]->y > imageHeight) _reference1[i]->y = imageHeight;
    }
    
    imageWidth = _image_face2.getWidth();
    imageHeight = _image_face2.getHeight();
    l = _reference2.size();
    for(i = 0; i < l; i++) {
        _reference2[i]->x = _temp2[i].x * (1 - _interpolationRatio) + _temp1[i].x * _interpolationRatio;
        _reference2[i]->y = _temp2[i].y * (1 - _interpolationRatio) + _temp1[i].y * _interpolationRatio;
        
        if(_reference2[i]->x < 0) _reference2[i]->x = 0;
        if(_reference2[i]->x > imageWidth) _reference2[i]->x = imageWidth;
        if(_reference2[i]->y < 0) _reference2[i]->y = 0;
        if(_reference2[i]->y > imageHeight) _reference2[i]->y = imageHeight;
        
    }
}

//--------------------------------------------------------------
void ofApp::_debugDraw() {
    //----------------------------------
    //Draw an original image.
    ofPushMatrix();
	ofTranslate(0, 0);
    _image_face1.draw(0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
	ofTranslate(0, 480);
    _image_face2.draw(0, 0);
    ofPopMatrix();
    //----------------------------------
    
    //----------------------------------
    //Draw wireframes.
    ofPushMatrix();
	ofTranslate(640, 0);
    _drawWireFrame1();
    ofPopMatrix();
    
    ofPushMatrix();
	ofTranslate(640, 480);
    _drawWireFrame2();
    ofPopMatrix();
    //----------------------------------
    
    //----------------------------------
    //Draw a result.
    ofPushMatrix();
    ofTranslate(640 * 2, 0);
    _drawFace1(1.0, true);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(640 * 2, 480);
    _drawFace2(1.0, true);
    ofPopMatrix();
    //----------------------------------
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
//    if(key == '1') _detectFace1();
//    if(key == '2') _detectFace2();
//    
//    if(key == ' ') {
//        _updateFaceMesh1();
//        _updateFaceMesh2();
//
//        _updateReference1();
//        _updateReference2();
//    }
    if(key == 'a') _debug = !_debug;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
