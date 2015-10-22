//
//  MusicianMachineView.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#ifndef MusicianMachineView_h
#define MusicianMachineView_h

#include <stdio.h>
#include "ofMain.h"

class MusicianMachineView{
private:
    map<int,ofIcoSpherePrimitive*> states;
    map<int,ofCylinderPrimitive*> transitions;
    
    vector<string> statesNames;
    vector<float> statesEnergy;
    vector<string> transitionStateNameInitial;
    vector<string> transitionStateNameFinal;
    vector<float> transitionStateProbabilities;

    int stateCurrent;
    int statePrevious;
    int lastTransition;
    
    int radiusSphere;
    int radiusCylinder;
    
    ofstream* fileOut;
    ifstream* fileIn;
    string name;
    bool isItActive;
    void clear();
    int getStateIndex( string name );
    vector<ofMeshFace> triangles;
    
    ofImage texture;
    ofMaterial material;

    
public:
    MusicianMachineView( string theName );
    ~MusicianMachineView();
    bool addState( string theName , float theParam01 );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );
    void draw();
    bool load( string fileName );
    void setActive( bool ifIsItActive );
    bool setCurrentState( string stateName );
};

#endif 
