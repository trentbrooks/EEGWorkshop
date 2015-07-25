#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    
    receiver.setup(5000);

    delta = theta = beta = alpha = gamma = 0.0;
    concentration = mellow = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

    while (receiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        
        
        // print all the messages available (a lot with muse): https://sites.google.com/a/interaxon.ca/muse-developer-site/museio/osc-paths/3-4-0
        // ofLog() << msg.getAddress() << " : " << msg.getNumArgs();// << " : " << msg.getArgAsFloat(0);
        
        
        // same as the brainbands esense values for attention + meditation
        // https://sites.google.com/a/interaxon.ca/muse-developer-site/museio/osc-paths/osc-paths---v3-6-0#TOC-Experimental
        // These algorithm values are emitted at 10Hz. Each channel is scored independently for alpha/gamma for mellow/concentration, and the average of the two channels is taken every time. If there is bad data, the previous score is used, but there is always an average of channels.
        if(msg.getAddress() == "/muse/elements/experimental/concentration") {
            concentration = msg.getArgAsFloat(0);
        } else if(msg.getAddress() == "/muse/elements/experimental/mellow") {
            mellow = msg.getArgAsFloat(0);
        }
        
        // Getting the 5 main EEG bands (can get relative or absolute values)
        // filter the messages - 4 channels per data point (left ear(0), left forehead(1), right forehead(2), right ear(3))
        // we are averaging the 4 channels here for simplicity - should probably keep all 4 in float array instead
        // if it NANS - it's because one of the args in the channel is NAN due to head band not being attached correctly. should add checks for this.
        // could also check for "/muse/elements/is_good" for each of the 4 channels. if all 1 then were ok.
        if(msg.getAddress() == "/muse/elements/delta_relative") {
            
            delta = getAverageFromChannels(msg);
   
        } else if(msg.getAddress() == "/muse/elements/theta_relative") {
            
            theta = getAverageFromChannels(msg);
            
        } else if(msg.getAddress() == "/muse/elements/alpha_relative") {

            alpha = getAverageFromChannels(msg);
            
        } else if(msg.getAddress() == "/muse/elements/beta_relative") {

            beta = getAverageFromChannels(msg);
            
        } else if(msg.getAddress() == "/muse/elements/gamma_relative") {

            gamma = getAverageFromChannels(msg);
            
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw a blue rectangle for mellow (meditation)
    /*ofSetColor(0, 174, 240);
    float meditationScaled = ofMap(mellow, 0, 1.0, 0, ofGetHeight());
    ofRect(0, ofGetHeight(), ofGetWidth()*.5, -meditationScaled);
    ofDrawBitmapStringHighlight(ofToString(mellow), ofGetWidth()*.25, ofGetHeight()-50);
    
    // draw a red rectangle for concentration (attention)
    ofSetColor(255, 107, 130);
    float attentionScaled = ofMap(concentration, 0, 1.0, 0, ofGetHeight());
    ofRect(ofGetWidth()*.5, ofGetHeight(), ofGetWidth()*.5, -attentionScaled);
    ofDrawBitmapStringHighlight(ofToString(concentration), ofGetWidth()*.75, ofGetHeight()-50);*/
    

    // to see the 5 eeg bands instead - uncomment this and comment out the above
    drawBandsData();
}

void ofApp::drawBandsData() {
    
    // colours for debug text
    ofColor clearClr(0,0);
    ofColor textClr(0);
    
    
    ofPushMatrix();
    ofTranslate(0,ofGetHeight()*.5);
    
    // draw circles for each of the 5 eeg bands
    int offsetX = ofGetWidth() / 6;
    
    // delta yellow
    ofSetColor(241, 226, 119);
    float deltaScaled = ofMap(delta, 0, 1.0, 0, 150);
    ofCircle(offsetX, 0, deltaScaled);
    ofDrawBitmapStringHighlight("Delta:\n"+ofToString(delta,2), offsetX, 0, clearClr, textClr);
    
    // theta green
    ofSetColor(128, 188, 177);
    float thetaScaled = ofMap(theta, 0, 1.0, 0, 150);
    ofCircle(offsetX*2, 0, thetaScaled);
    ofDrawBitmapStringHighlight("Theta:\n"+ofToString(theta,2), offsetX*2, 0, clearClr, textClr);
    
    // alpha blue
    ofSetColor(0, 174, 240);
    float alphaScaled = ofMap(alpha, 0, 1.0, 0, 150);
    ofCircle(offsetX*3, 0, alphaScaled);
    ofDrawBitmapStringHighlight("Alpha:\n"+ofToString(alpha,2), offsetX*3, 0, clearClr, textClr);
    
    // beta red
    ofSetColor(255, 107, 130);
    float betaScaled = ofMap(beta, 0, 1.0, 0, 150);
    ofCircle(offsetX*4, 0, betaScaled);
    ofDrawBitmapStringHighlight("Beta:\n"+ofToString(beta,2), offsetX*4, 0, clearClr, textClr);
    
    // gamma orange
    ofSetColor(255, 165, 106);
    float gammaScaled = ofMap(gamma, 0, 1.0, 0, 150);
    ofCircle(offsetX*5, 0, gammaScaled);
    ofDrawBitmapStringHighlight("Gamma:\n"+ofToString(gamma,2), offsetX*5, 0, clearClr, textClr);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
float ofApp::getAverageFromChannels(ofxOscMessage& msg) {
    
    float avg = 0;
    int channels = msg.getNumArgs();
    for(int i = 0; i < channels; i++) {
        avg += msg.getArgAsFloat(i);
    }
    avg /= channels;
    return avg;
}

float ofApp::getHighestFromChannels(ofxOscMessage& msg) {
    
    float highest = 0;
    int channels = msg.getNumArgs();
    for(int i = 0; i < channels; i++) {
        float channelValue = msg.getArgAsFloat(i);
        if(channelValue > highest) highest = channelValue;
    }
    return highest;
}

float ofApp::getFromEarChannels(ofxOscMessage& msg) {
    
    float avg = 0;
    avg += msg.getArgAsFloat(0);
    avg += msg.getArgAsFloat(3);
    avg /= 2;
    return avg;
}

float ofApp::getFromForeheadChannels(ofxOscMessage& msg) {
    
    float avg = 0;
    avg += msg.getArgAsFloat(1);
    avg += msg.getArgAsFloat(2);
    avg /= 2;
    return avg;
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
