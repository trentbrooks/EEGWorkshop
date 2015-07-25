#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFFTLive.h"
#include "ofxOsc.h"
#include "ofxObjLoader.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxPanel gui;
    ofxSlider<int> meshIndex;
    ofxToggle bUseTexture;
    ofxToggle bUseAudioInput;
    ofxSlider<float> audioPeakDecay;
    ofxSlider<float> audioMaxDecay;
    ofxToggle audioMirror;
    
    // changing from fftLive to fftBase
    //ofxFFTLive fftLive;
    ofxFFTBase fftLive;

    ofEasyCam camera;
    
    ofTexture meshTexture;
    ofxAssimpModelLoader model;
    vector<ofMesh> meshes;
    ofMesh meshWarped;
    
    // eeg vales
    ofxOscReceiver receiver;
    vector<float> eegValues;
    int rawIndex;
    
};
