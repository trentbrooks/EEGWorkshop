#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackground(50);
    ofSetFrameRate(30);
    //ofDisableArbTex();

    receiver.setup(7771);

    
    ofLoadImage(meshTexture, "texture.jpg");
    
    //--------------------------------------------------------------
    ofMesh customMesh;
    customMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    customMesh.addVertex(ofVec3f(-100, -100));
    customMesh.addVertex(ofVec3f(-100, 100));
    customMesh.addVertex(ofVec3f(100, -100));
    customMesh.addVertex(ofVec3f(100, 100));
    
    customMesh.addTexCoord(ofVec2f(0, 0));
    customMesh.addTexCoord(ofVec2f(0, 1));
    customMesh.addTexCoord(ofVec2f(1, 0));
    customMesh.addTexCoord(ofVec2f(1, 1));
    
    customMesh.addIndex(0);
    customMesh.addIndex(3);
    customMesh.addIndex(1);
    
    customMesh.addIndex(0);
    customMesh.addIndex(2);
    customMesh.addIndex(3);
    
    //--------------------------------------------------------------
    ofSpherePrimitive sphere(100, 20, OF_PRIMITIVE_TRIANGLES);
    
    //--------------------------------------------------------------
    model.loadModel("head.dae");
    
    //--------------------------------------------------------------
    meshIndex = 0;
    meshes.push_back(customMesh);
    meshes.push_back(sphere.getMesh());
    meshes.push_back(model.getMesh(0));
    
    //--------------------------------------------------------------
    //fftLive.setup();
    rawIndex = 0;
    eegValues.resize(512); // samples per second
    fftLive.setup();
    fftLive.setBufferSize(eegValues.size()); // must be set manually for a custom signal
    
    //--------------------------------------------------------------
    string guiPath = "audio.xml";
    gui.setup("audio", guiPath, 20, 20);
    gui.add(meshIndex.setup("meshIndex", 0, 0, meshes.size()-1));
    gui.add(bUseTexture.setup("bUseTexture", false));
    gui.add(bUseAudioInput.setup("bUseAudioInput", false));
    gui.add(audioPeakDecay.setup("audioPeakDecay", 0.915, 0.9, 1.0));
    gui.add(audioMaxDecay.setup("audioMaxDecay", 0.995, 0.9, 1.0));
    gui.add(audioMirror.setup("audioMirror", true));
    gui.loadFromFile(guiPath);

}

//--------------------------------------------------------------
void ofApp::update() {
    
    // check for waiting messages
	while(receiver.hasWaitingMessages()){

        ofxOscMessage msg;
		receiver.getNextMessage(&msg);
        
        // print all the messages
        //ofLog() << msg.getAddress() << " : " << msg.getArgAsFloat(0);
        
		// filter the raw data only
		if(msg.getAddress() == "/raw") {
            float raw = msg.getArgAsFloat(0);
            eegValues[rawIndex] = raw / 4096.0; // raw range is -2048 to 2048
            
            rawIndex++;
            if(rawIndex >= eegValues.size()) rawIndex = 0;

        }
    }
    
    fftLive.setPeakDecay(audioPeakDecay);
    fftLive.setMaxDecay(audioMaxDecay);
    fftLive.setMirrorData(audioMirror);
    fftLive.update();
    
    //---------------------------------------------------------- dispacing mesh using audio.
    ofMesh & meshOriginal = meshes[meshIndex];
    meshWarped = meshOriginal;
    
    if(bUseAudioInput == false) {
        return;
    }
    
    // need to manually insert the values into fft for processing
    float *data = &eegValues[0];
    fftLive.audioIn(data);
    
    vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
    vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
    int numOfVerts = meshOriginal.getNumVertices();
    
    float * audioData = new float[numOfVerts];
    fftLive.getFftPeakData(audioData, numOfVerts);
    
    float meshDisplacement = 100;
    
    for(int i=0; i<numOfVerts; i++) {
        float audioValue = audioData[i];
        ofVec3f & vertOriginal = vertsOriginal[i];
        ofVec3f & vertWarped = vertsWarped[i];
        
        ofVec3f direction = vertOriginal.getNormalized();
        vertWarped = vertOriginal + direction * meshDisplacement * audioValue;
    }
    
    delete[] audioData;
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    //----------------------------------------------------------
    gui.draw();
    
    int w = 512;
    int h = 256;
    int x = 20;
    int y = ofGetHeight() - h - 20;
    fftLive.draw(x, y, w, h);
    
    //----------------------------------------------------------
    ofEnableDepthTest();
    
    float dist = 400;
    camera.setDistance(dist);
    camera.begin();
    
    if(bUseTexture == true) {
        ofEnableNormalizedTexCoords();
        meshTexture.bind();
    }
    
    ofSetColor(ofColor::white);
    meshWarped.drawFaces();
    
    if(bUseTexture == true) {
        meshTexture.unbind();
        ofDisableNormalizedTexCoords();
    }
    
    ofSetColor(ofColor::black);
    meshWarped.drawWireframe();
    
    camera.end();
    
    ofDisableDepthTest();
    ofSetColor(ofColor::white);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == OF_KEY_LEFT) {
        meshIndex = meshIndex - 1;
        if(meshIndex < 0) {
            meshIndex = meshes.size() - 1;
        }
    } else if(key == OF_KEY_RIGHT) {
        meshIndex = meshIndex + 1;
        if(meshIndex > meshes.size() - 1) {
            meshIndex = 0;
        }
    } else if(key == ' ') {
        bUseAudioInput = !bUseAudioInput;
    } else if(key == 'e' || key == 'E') {
        ofxObjLoader::save("mesh_export.obj", meshWarped);
    }
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