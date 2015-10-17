#pragma once

#include "ofMain.h"
#include "MusicianMachine.h"


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
    
    //GUI
    ofPoint GUIFirstCirclePosition;
    int GUICircleRadius;
    int GUICircleSpacing;
    ofImage cursor;
    void createGUI();
    void drawGUI();
    void mouseReleasedGUI( int x , int y , int buton );
    void drawGUIMouse();
    
    //equalizer
    float* fftSmoothed;
    int nBandsToGet;
    void createEqualizer();
    void updateEqualizer();
    void drawEqualizer( int x , int y , int w , int h );
    
    //machines
    std::vector<MusicianMachine*> machines;
    void createMachines();
};

