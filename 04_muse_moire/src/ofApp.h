#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

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
    
    
        ofxOscReceiver receiver;
    
        // helper methods to get individual channel data
        float getAverageFromChannels(ofxOscMessage& msg);
        float getHighestFromChannels(ofxOscMessage& msg);
        float getFromEarChannels(ofxOscMessage& msg);
        float getFromForeheadChannels(ofxOscMessage& msg);
    
        // eeg device values
        float eegTheta, eegAlpha;
        ofVec3f accelerometer;
    
    
        // arc shape drawing
        void drawArcs();
        float arcResolution, arcThickness;
        float arcWidth, arcHeight;
    
        // convert a path/polyline to a ribbon mesh
        ofMesh polylineToRibbonMesh(ofPolyline& polyline, float thickness);
    
        // wireframe shader for better performance
        bool useShader;
        ofShader wireframeShader;

};
