//
//  MusicianState.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#include "ofMain.h"
#include "MusicianState.h"
//----------------------------------------------------------------------------------
MusicianState::MusicianState(){
}
//----------------------------------------------------------------------------------
MusicianState::MusicianState( string theStatetNane , string theSoundFileName , float param01Value ){
    if( !sound.loadSound( theSoundFileName ) ){
        cout << "Failed to load sound file ";
        cout << soundFileName << "\n";
    }
    name = theStatetNane;
    soundFileName = theSoundFileName;
    param01 = param01Value;
    sound.stop();
    sound.setLoop(false);
    sound.setMultiPlay(true);
}
//----------------------------------------------------------------------------------
string MusicianState::getName(){
    return name;
}
//----------------------------------------------------------------------------------
bool MusicianState::load(std::ifstream* fileIn ){
    std::string junk;
    
    (*fileIn) >> junk;
    if( junk != "name=" ){
        cout << "* Musician  State load: Bad tag name\n";
        return false;
    }
    (*fileIn) >> name;
    
    (*fileIn) >> junk;
    if( junk != "soundFileName=" ){
        cout << "* Musician  State load: Bad tag soundFileName\n";
        return false;
    }
    (*fileIn) >> soundFileName;
    
    (*fileIn) >> junk;
    if( junk != "param01=" ){
        cout << "* Musician  State load: Bad tag param01\n";
        return false;
    }
    (*fileIn) >> param01;
    
    if( !sound.loadSound( soundFileName ) ){
        cout << "Failed to load sound file ";
        cout << soundFileName << "\n";
        return false;
    }
    sound.stop();
    sound.setLoop(false);
    return true;
}
//-----------------------------------------------------------
bool MusicianState::save(std::ofstream* fileOut ){
    (*fileOut) << "name= ";
    (*fileOut) << name;
    (*fileOut) << "\n";
    
    (*fileOut) << "soundFileName= ";
    (*fileOut) << soundFileName;
    (*fileOut) << "\n";
    
    (*fileOut) << "param01= ";
    (*fileOut) << param01;
    (*fileOut) << "\n";
    
    return  true;
}
