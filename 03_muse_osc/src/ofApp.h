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
		
        // 5 eeg bands
        float delta;
        float theta;
        float alpha;
        float beta;
        float gamma;
    
        void drawBandsData();
    
        // processed values (these are pretty much the same as the brainband's esense values)
        float concentration;
        float mellow;
};
