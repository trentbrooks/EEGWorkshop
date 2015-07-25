#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    // setup thinkgear hardware using serial streamer or comms driver (osx only tested).
    // TG_STREAM_PARSER is default
    // blinks only work in TG_COMMS_DRIVER mode
    tg.setup("/dev/tty.BrainBand-DevB", 57600, TG_COMMS_DRIVER); //TG_STREAM_PARSER or TG_COMMS_DRIVER
    tg.addEventListener(this);
    
    attention = 0;
    meditation = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

    tg.update();
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
// thinkgear events
void ofApp::onThinkgearReady(ofxThinkgearEventArgs& args){
    ofLog() << "*** THINKGEAR Ready...";
}

void ofApp::onThinkgearConnecting(ofxThinkgearEventArgs& args){
    ofLog() << "*** THINKGEAR connecting...";
}

void ofApp::onThinkgearError(ofMessage& err){
    ofLog() << "*** THINKGEAR onError..." << err.message;
}

// raw data does not send at frame rate - it sends faster! 512 samples a second
void ofApp::onThinkgearRaw(ofxThinkgearEventArgs& args){
    //ofLog() << "raw: " << args.raw;
}

// no visual indicator for blinks
void ofApp::onThinkgearBlinkStrength(ofxThinkgearEventArgs& args){
    // only works when using the comms driver (TG_COMMS_DRIVER)
    ofLog() << "blink: " << (int)args.blinkStrength;
}


// battery never fires?
void ofApp::onThinkgearBattery(ofxThinkgearEventArgs& args){
    ofLog() << "*** THINKGEAR battery: " << (int)args.battery;
}

void ofApp::onThinkgearPoorSignal(ofxThinkgearEventArgs& args){
    ofLog() << "poor signal: " << (int)args.poorSignal;
}

void ofApp::onThinkgearAttention(ofxThinkgearEventArgs& args){
    ofLog() << "attention: " << (int)args.attention;
    attention = args.attention;
}

void ofApp::onThinkgearMeditation(ofxThinkgearEventArgs& args){
    ofLog() << "meditation: " << (int)args.meditation;
    meditation = args.meditation;
}

void ofApp::onThinkgearEeg(ofxThinkgearEventArgs& args){
    
    // about the 8 eeg bands (Comparison table of EEG rhythmic activity frequency bands): http://en.wikipedia.org/wiki/Electroencephalography
    // about neurusky's interpretation: http://support.neurosky.com/kb/development-2/eeg-band-power-values-units-amplitudes-and-meaning
    
    stringstream eegBands;
    eegBands << "delta: " << args.eegDelta;
    eegBands << ", theta: " << args.eegTheta;
    eegBands << ", alpha low: " << args.eegLowAlpha;
    eegBands << ", alpha high: " << args.eegHighAlpha;
    eegBands << ", beta low: " << args.eegLowBeta;
    eegBands << ", beta high: " << args.eegHighBeta;
    eegBands << ", gamma low: " << args.eegLowGamma;
    eegBands << ", gamma high: " << args.eegHighGamma;
    ofLog() << eegBands.str();
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
