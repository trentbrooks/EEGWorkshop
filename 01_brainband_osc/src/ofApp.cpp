#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    receiver.setup(7771);

    attention = 0;
    meditation = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

    while (receiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        
        // print all the messages
        ofLog() << msg.getAddress() << " : " << msg.getArgAsFloat(0);
        
        // filter the messages
        if(msg.getAddress() == "/attention") {
            attention = msg.getArgAsFloat(0);
        } else if(msg.getAddress() == "/meditation") {
            meditation = msg.getArgAsFloat(0);
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw a blue rectangle for meditation
    ofSetColor(0, 174, 240);
    float meditationScaled = ofMap(meditation, 0, 100, 0, ofGetHeight());
    ofRect(0, ofGetHeight(), ofGetWidth()*.5, -meditationScaled);
    ofDrawBitmapStringHighlight(ofToString(meditation), ofGetWidth()*.25, ofGetHeight()-50);
    
    // draw a red rectangle for attention
    ofSetColor(255, 107, 130);
    float attentionScaled = ofMap(attention, 0, 100, 0, ofGetHeight());
    ofRect(ofGetWidth()*.5, ofGetHeight(), ofGetWidth()*.5, -attentionScaled);
    ofDrawBitmapStringHighlight(ofToString(attention), ofGetWidth()*.75, ofGetHeight()-50);
 
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
