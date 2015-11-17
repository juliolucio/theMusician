#pragma once

#include "ofMain.h"
#include "MusicianComposition.h"
#include "ofxCvHaarFinder.h"
#include "HaarFinderThread.h"

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
 
    //gui
    ofImage cursor;
    void drawGUIMouse();
    bool bShowHelp;
    
    //equalizer
    float* fftSmoothed;
    int nBandsToGet;
    void createEqualizer();
    void updateEqualizer();
    void drawEqualizer( int x , int y , int w , int h );
    
    //istructions
    void drawInstructions( );
    
    //timer
    void drawTimer( ofVec3f position );
    void drawEnergy( ofVec3f position , int height , int radius);
    void drawVolumen( ofVec3f position , int height , int radius);
    
    //camera
    void drawInteractionArea();
    ofEasyCam* camera;
    
    //texturing and lighting
    ofImage texture;
    ofMaterial material;
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofLight pointLightTime;

    //composition
    MusicianComposition composition;
    
    HaarFinderThread finder;
    ofVideoGrabber vidGrabber;
    ofImage* img;
    float directorEnergy;
    float volume;

};

