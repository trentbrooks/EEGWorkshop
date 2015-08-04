#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofBackground(255);
    
    // osc setup (muse)
    receiver.setup(5000);
    
    // eeg values were using: theta + alpha
    eegTheta = eegAlpha = 0.0;
    
    // arc shape properties
    arcResolution = 20;
    arcThickness = 20;
    arcWidth = 100; // 90-110
    arcHeight = 100;
    
    // load the wireframe shader
    useShader = false;
    wireframeShader.setGeometryInputType(GL_TRIANGLES);
    wireframeShader.setGeometryOutputType(GL_TRIANGLES);
    wireframeShader.setGeometryOutputCount(3);
    wireframeShader.load("wireframe.vert","wireframe.frag","wireframe.geom");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //return;
    while (receiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        

        // Band Power Session Scores /muse/elements/delta_session_score
        // Session scores gives values between 0 and 1 by comparing the user's current band power value to an average of their recent historical values of that same band. https://sites.google.com/a/interaxon.ca/muse-developer-site/museio/osc-paths/osc-paths---v3-6-0#TOC-Band-Power-Session-Scores
        if(msg.getAddress() == "/muse/elements/theta_session_score") {
            
            // use theta to adjust the arcResolution
            eegTheta = getHighestFromChannels(msg); // getAverageFromChannels, getHighestFromChannels
            
        } else if(msg.getAddress() == "/muse/elements/alpha_session_score") { //alpha_session_score, alpha_absolute
            
            // use alpha to adjust arcThickness
            eegAlpha = getHighestFromChannels(msg);
            
        }
        
        // also filter the accelerometer values (-512 to 511)
        if(msg.getAddress() == "/muse/acc") {
            
            // applyling the accelerometer z to the arcWidth (tilt head left/right)
            accelerometer.x = msg.getArgAsFloat(0);
            accelerometer.y = msg.getArgAsFloat(1);
            accelerometer.z = msg.getArgAsFloat(2); // -1000 to 1000
            
        }
    }
    
    // check if eeg values are over threshold (we want the arcResolution + arcThickness to be going up or down)
    float thetaVel = 0.5;
    float thetaThreshold = 0.65; // tweak this as required
    arcResolution = (eegTheta > thetaThreshold) ? arcResolution + thetaVel : arcResolution - thetaVel;
    arcResolution = ofClamp(arcResolution, 2, 1000);
    
    float alphaVel = 1.0;
    float alphaThreshold = 0.65; // tweak this as required
    arcThickness = (eegAlpha > alphaThreshold) ? arcThickness + alphaVel : arcThickness - alphaVel;
    arcThickness = ofClamp(arcThickness, 1, 3000);
    
    // check if value is over some threshold then begin adding amount/subtracting to arcWidth
    // middle (looking straight ahead) is supposed to be 0, but is skewed to 200ish
    float middleBase = 200;
    float accVel = 0.02;
    if (accelerometer.z < middleBase - 500) {
        arcWidth -= accVel;
    } else if(accelerometer.z > middleBase + 500) {
        arcWidth += accVel;
    }
    arcWidth = ofClamp(arcWidth, 90, 110);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(0);
    drawArcs();

    
    bool drawDebugInfo = true;
    if(drawDebugInfo) {
        //ofSetColor(0);
        stringstream debugInfo;
        debugInfo << "fps: " << ofToString(ofGetFrameRate(),0) << endl;
        debugInfo << "shader: " << useShader << endl;
        debugInfo << "eeg theta: " << ofToString(eegTheta,1) << endl;
        debugInfo << "eeg alpha: " << ofToString(eegAlpha,1) << endl;
        debugInfo << "accelerometer: " << ofToString(accelerometer.x,0) << ", " << ofToString(accelerometer.y,0) << ", " << ofToString(accelerometer.z,0) << endl;
        debugInfo << "arc width: " << arcWidth << endl;
        debugInfo << "arc res: " << arcResolution << endl;
        debugInfo << "arc thick: " << arcThickness;// << endl;
        ofDrawBitmapStringHighlight(debugInfo.str(), 20,20);
    }
    
}

//--------------------------------------------------------------
void ofApp::drawArcs() {
    
    // if not modulo 2 then toooooooo jumpy when animated
    int roundedArcRes = (int(arcResolution) % 2 == 0) ? arcResolution : arcResolution + 1;
    
    // make an arc/ring shape using ofPolyline
    ofPolyline curve;
    curve.arc(ofGetWidth()*.5, ofGetHeight()*.5, arcWidth, arcHeight, 0, 360, roundedArcRes);
    
    // convert the polyline into a thick ribbon mesh
    ofMesh mesh = polylineToRibbonMesh(curve, arcThickness);

    // draw mesh
    if(!useShader) {
    
        // draw as regular wireframe
        //curve.draw();
        mesh.drawWireframe();
        
    } else {
        
        // or draw with shader (need to draw faces instead) - faster
        // note we are drawing triangles, which get converted to lines in geom shader
        wireframeShader.begin();
        wireframeShader.setUniform2f("ViewportSize", float(ofGetWidth()), float(ofGetHeight()));
        wireframeShader.setUniform1f("LineWidth", 1.0);
        
        mesh.drawFaces();
        
        wireframeShader.end();
    }

}

// code example referenced from 3d/cameraRibbonExample
ofMesh ofApp::polylineToRibbonMesh(ofPolyline& polyline, float thickness) {
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    for(int i = 1; i < polyline.size(); i++){
        
        //find this point and the next point
        ofVec3f thisPoint = polyline[i-1];
        ofVec3f nextPoint = polyline[i];
        
        //get the direction from one to the next.
        //the ribbon should fan out from this direction
        ofVec3f direction = (nextPoint - thisPoint);
        
        //get the distance from one point to the next
        //float distance = direction.length();
        
        //get the normalized direction. normalized vectors always have a length of one
        //and are really useful for representing directions as opposed to something with length
        ofVec3f unitDirection = direction.normalized();
        
        //find both directions to the left and to the right
        ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
        ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
        
        //calculate the points to the left and to the right
        //by extending the current point in the direction of left/right by the length
        ofVec3f leftPoint = thisPoint+toTheLeft * thickness;
        ofVec3f rightPoint = thisPoint+toTheRight * thickness;
        
        // fill mesh
        mesh.addVertex(leftPoint);
        mesh.addVertex(rightPoint);
        
        // draw the end? same orintation as previous
        // only need this if the end needs to join to the beginning
        if(i == polyline.size()-1) {
            ofVec3f firstLeft = mesh.getVertex(0);
            ofVec3f firstRight = mesh.getVertex(1);
            mesh.addVertex(firstLeft);
            mesh.addVertex(firstRight);
        }
    }
    
    return mesh;
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

    if(key == 's') {
        useShader = !useShader;
    } else if(key == OF_KEY_LEFT) {
        arcWidth -= .1;
    } else if(key == OF_KEY_RIGHT) {
        arcWidth += .1;
    }
        
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

    // debug - adjust the arc res + thickness from mouse x + y
    bool adjustArcFromMouse = false;
    if(adjustArcFromMouse) {
        arcResolution = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 1000); // 1000
        arcThickness = ofMap(ofGetMouseY(), 0, ofGetHeight(), 1, 3000); // 3000
    }
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
