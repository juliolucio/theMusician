//
//  MusicianComposition.h
//  theMusiciam
//
//  Created by Julio Lucio on 10/24/15.
//
//
#pragma once

#include "ofMain.h"
#include "MusicianMachine.h"
#include "MusicianMachineController.h"

class MusicianComposition{
    
public:
    MusicianComposition();
    ~MusicianComposition();
    
    void setup( ofEasyCam* theCamera );
    void update( float theEnergy , float theVolume  );
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
    
    float getPosition();
    bool isMachineActive( int machineIndex );
    
    //GUI
    ofPoint GUIFirstCirclePosition;
    int GUICircleRadius;
    int GUICircleSpacing;
    ofImage cursor;
    void drawGUI( ofPoint GUIFirstCirclePosition ,int GUICircleRadius,
                 int GUICircleSpacing);
    void mouseReleasedGUI( int x , int y , int buton );
    
    //machines
    std::vector<MusicianMachine*> machines;
    std::vector<MusicianMachineController*> machinesControllers;
    float energy;
    
    void createMachines();
    void createMachinesTest();
    
    ofEasyCam* camera;
};

