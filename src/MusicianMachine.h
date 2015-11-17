//
//  MusicianMachine.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#ifndef MusicianMachine_h
#define MusicianMachine_h

#include <stdio.h>
#include "ofMain.h"
class MusicianState;
class MusicianTransition;

class MusicianMachine{
private:
    vector< MusicianState* > states;
    vector< MusicianTransition* > transitions;
    MusicianState* currentState;
    MusicianState* nextState;
    MusicianState*prevState;
    ofstream* fileOut;
    ifstream* fileIn;
    string name;
    bool isItActive;
    bool hasJustChangedState;
    float volume;
    void clear();
    
public:
    MusicianMachine( string theName );
    ~MusicianMachine();
    bool addState( string theName , string theSoundFilaeName , float theParam01 );
    bool addTransition( string nameState01 , string nameState02 , float theProbability );

    bool load( string fileName );
    bool save( string fileName );
    void updateStates( float theEnergy);
    void setVolume(float theVolume );
    void start();
    void stop();
    void draw();
    string getName();
    string getCurrentStateName();
    float getCurrentStatePercentaje();
    void setActive( bool ifIsItActive );
    bool isAtcive();
    bool justChangedState();
    bool justFinishidState();
    void setCurrentState( MusicianState* newState );
    

};

#endif 
