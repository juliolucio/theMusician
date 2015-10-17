//
//  MusicianTransition.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#include "MusicianTransition.h"
#include "MusicianState.h"
//----------------------------------------------------------
MusicianTransition::MusicianTransition(  MusicianState* theStateInitial , MusicianState* theStateFinal , float theProbability ){
    stateInitial = theStateInitial;
    stateFinal = theStateFinal;
    probability = theProbability;
}
//----------------------------------------------------------
string MusicianTransition::getNameStateInitial(){
    return stateInitial->getName();
}
//----------------------------------------------------------
string MusicianTransition::getNameStateFinal(){
    return stateFinal->getName();
}
//----------------------------------------------------------
MusicianState* MusicianTransition::getStateFinal(){
    return stateFinal;
}
//----------------------------------------------------------
float MusicianTransition::getProbability(){
    return probability;
}
//-----------------------------------------------------------
bool MusicianTransition::save(std::ofstream* fileOut ){
    (*fileOut) << "stateNameInitial= ";
    (*fileOut) << stateInitial->getName();
    (*fileOut) << "\n";
    
    (*fileOut) << "stateNameFinal= ";
    (*fileOut) << stateFinal->getName();;
    (*fileOut) << "\n";
    
    (*fileOut) << "probability= ";
    (*fileOut) << probability;
    (*fileOut) << "\n";
    
    return  true;
}

