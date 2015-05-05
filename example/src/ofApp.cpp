#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// setup ofxOscReceiver
	receiver_.setup(12345);
	// event listener
	receiver_.addListener("/funcA", this, &ofApp::callbackA);
	receiver_.addListener("/funcB", this, &ofApp::callbackA);
	// set reference directly
	receiver_.addReference("/intval", ref_);
}

void ofApp::callbackA(ofxOscMessage &msg)
{
	cout << "callback A called" << endl;
}

void ofApp::callbackB(ofxOscMessage &msg)
{
	cout << "callback B called" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(ofToString(ref_), 10,10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
