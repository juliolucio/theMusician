//
//  MusicianTransition.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#ifndef MusicianTransition_h
#define MusicianTransition_h

#include <stdio.h>
#include "ofMain.h"
class MusicianState;
class MusicianTransition{
private:
    MusicianState* stateInitial;
    MusicianState* stateFinal;
    float probability;
public:
    MusicianTransition(  MusicianState* theStateInitial , MusicianState* theStateFinal , float theProbability );
    string getNameStateInitial();
    string getNameStateFinal();
    MusicianState* getStateFinal();
    MusicianState* getStateInitial();
    float getProbability();
    bool save( std::ofstream* fileOut);
};
#endif
