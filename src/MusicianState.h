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
    float param01;

public:
    ofSoundPlayer sound;
    MusicianState( );
    MusicianState( string name , string soundFileName , float param01Value );
    string getName();
    bool load( std::ifstream* fileIn );
    bool save( std::ofstream* fileIn );
    
};
#endif
