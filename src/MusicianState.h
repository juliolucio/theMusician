//
//  MusicianState.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#ifndef MusicianState_h
#define MusicianState_h

#include <stdio.h>
class ofSoundPlayer;

class MusicianState{
private:
    string name;
    string soundFileName;
    float energy;

public:
    ofSoundPlayer sound;
    MusicianState( );
    MusicianState( string name , string soundFileName , float param01Value );
    string getName();
    float getEnergy();
    bool load( std::ifstream* fileIn );
    bool save( std::ofstream* fileIn );
    
};
#endif
