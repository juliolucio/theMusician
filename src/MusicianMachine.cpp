//
//  MusicianMachine.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#include "MusicianMachine.h"
#include "MusicianState.h"
#include "MusicianTransition.h"

//-------------------------------------------------------------
MusicianMachine::MusicianMachine( string theName ){
    name = theName;
    currentState = 0;
    fileIn = 0;
    fileOut = 0;
    isItActive = true;
    hasJustChangedState = false;
}
//-------------------------------------------------------------
MusicianMachine::~MusicianMachine(){
    clear();
}
//-------------------------------------------------------------
bool MusicianMachine::addState( string theName , string theSoundFilaeName , float theParam01 ){
    for( int s = 0 ; s < states.size() ; s ++ )
        if( states[ s ]->getName() == theName ){
            cout << "State name already exist... \n";
            return false;
        }
    MusicianState* newState = new MusicianState( theName , theSoundFilaeName ,  theParam01 );
    states.push_back( newState );
    if( !currentState )
        currentState = states[0];
    return true;
}
//-------------------------------------------------------------
bool MusicianMachine::addTransition( string nameState01 , string nameState02 , float theProbability ){
    for( int t = 0 ; t < transitions.size() ; t ++ )
        if( transitions[ t ]->getNameStateInitial() == nameState01 && transitions[ t ]->getNameStateFinal() == nameState02 ){
            cout << "Transition already defined ... \n";
            return false;
        }
    
    MusicianState* tempInitialState = 0;
    MusicianState* tempFinalState = 0;
    for( int s = 0 ; s < states.size() ; s ++ ){
        if( states[ s ]->getName() == nameState01 )
            tempInitialState = states[ s ];
        if( states[ s ]->getName() == nameState02 )
            tempFinalState = states[ s ];
    }
    if( !tempInitialState || !tempFinalState ){
        cout << "Missing state in trasition ... \n";
        return false;
    }
    MusicianTransition* newTransition = new MusicianTransition( tempInitialState , tempFinalState , theProbability );
    transitions.push_back( newTransition );
    return true;
}
//-------------------------------------------------------------
void MusicianMachine::updateStates( float theEnergy ){
    float dice = ofRandom( 0 , 1 );
    map<float,MusicianTransition*> posibleTransitions;
    map<float,MusicianTransition*>::iterator posibleTransitionsIterator;
    for( int t = 0 ; t < transitions.size() ; t ++ ){
        if( transitions[t]->getNameStateInitial() == currentState->getName() ){
            pair<float,MusicianTransition*> newPosibleTransition;
            float energuiFinal = transitions[t]->getStateFinal()->getEnergy();
            float energyInitial = transitions[t]->getStateInitial()->getEnergy();
            
            if( theEnergy > 0.65 ){
                if( energuiFinal > energyInitial ){
                    newPosibleTransition.first = transitions[t]->getProbability();
                    newPosibleTransition.second = transitions[t];
                    posibleTransitions.insert(newPosibleTransition);
                }
            }
            
            else if( theEnergy < 0.35 ){
                if( energuiFinal < energyInitial ){
                    newPosibleTransition.first = transitions[t]->getProbability();
                    newPosibleTransition.second = transitions[t];
                    posibleTransitions.insert(newPosibleTransition);
                }
            }
            else{
                newPosibleTransition.first = transitions[t]->getProbability();
                newPosibleTransition.second = transitions[t];
                posibleTransitions.insert(newPosibleTransition);
            }
        }
    }
    
    if( !posibleTransitions.size() ){
        currentState->sound.play();
    }
    
    else{
        //normalizing probabilities
        float total = 0;
        for( posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,MusicianTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            total += tempPosibleTransition.first;
        }
        
        for( posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,MusicianTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            tempPosibleTransition.first /= total;
        }
        float valuerReached = 0;
        for(posibleTransitionsIterator = posibleTransitions.begin() ; posibleTransitionsIterator != posibleTransitions.end() ;posibleTransitionsIterator ++ ){
            pair<float,MusicianTransition*> tempPosibleTransition = (*posibleTransitionsIterator);
            valuerReached += tempPosibleTransition.first;
            if( valuerReached >= dice ){
                setCurrentState( tempPosibleTransition.second->getStateFinal() );
                return;
            }
        }
        if( posibleTransitions.size() ){
            posibleTransitionsIterator--;
            setCurrentState( (*posibleTransitionsIterator).second->getStateFinal() );
            return;
        }
    }
}
//-------------------------------------------------------------
void MusicianMachine::start(){
    currentState->sound.play();
}
//-------------------------------------------------------------
void MusicianMachine::stop(){
}

//-------------------------------------------------------------
void MusicianMachine::draw(){
}
//-------------------------------------------------------------
void MusicianMachine::clear(){
    for( int s = 0 ; s < states.size() ; s ++ )
        delete states[s];
    states.clear();
    
    for( int t = 0 ; t < transitions.size() ; t ++ )
        delete transitions[t];
    transitions.clear();
}
///----------------------------------------------------------------------------------
bool MusicianMachine::load( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "Machine File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    std::string junk;
    int numStates;
    int numTransitions;
    
    (*fileIn) >> junk;
    if( junk != "numStates=" ){
        cout << "* MusicianMachine  load: Bad tag numStates\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    (*fileIn) >> junk;
    if( junk != "numTransitions=" ){
        cout << "* MusicianMachine  load: Bad tag numTransitions\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numTransitions;
    
    clear();
    
    for( int s = 0 ; s < numStates ; s ++ ){
        MusicianState* newState = new MusicianState();
        if( !newState->load(fileIn) ){
            cout << "* MusicianMachine  load: Bad tag numTransitions\n";
            fileIn->close();
            return false;
        }
        states.push_back(newState);
        if( !currentState )
            currentState = states[0];
    }
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        (*fileIn) >> junk;
        std::string stateNameInitial;
        if( junk != "stateNameInitial=" ){
            cout << "* MusicianMachine  load: Bad tag stateNameInitial\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameInitial;
        
        (*fileIn) >> junk;
        std::string stateNameFinal;
        if( junk != "stateNameFinal=" ){
            cout << "* MusicianMachine  load: Bad tag stateNameFinal\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameFinal;
        
        (*fileIn) >> junk;
        float probability;
        if( junk != "probability=" ){
            cout << "* MusicianMachine  load: Bad tag probability\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* MusicianMachine  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
bool MusicianMachine::save( string fileName ){
    if( !fileOut )
        fileOut = new ofstream();
    fileOut->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::out );
    if ( !fileOut->is_open() ){
        cout << "Couldt create Machien file: ";
        cout << fileName << "\n";
        fileOut->close();
        return false;
    }
    (*fileOut) << "numStates= ";
    (*fileOut) << states.size();
    (*fileOut) << "\n";
    
    (*fileOut) << "numTransitions= ";
    (*fileOut) << transitions.size();
    (*fileOut) << "\n";
    
    for( int s = 0 ; s < states.size() ; s ++ ){
        if( !states[s]->save(fileOut) ){
            cout << "* MusicianMachine  save: couldnt save state\n";
            fileOut->close();
            return false;
        }
    }
    
    for( int t = 0 ; t < transitions.size() ; t ++ ){
        if( !transitions[t]->save(fileOut) ){
            cout << "* MusicianMachine  save transition: couldnt save transition" << t << "\n";
            fileOut->close();
            return false;
        }
    }
    fileOut->close();
    return  true;
}
//-----------------------------------------------------------
string MusicianMachine::getName(){
    return name;
}
//-----------------------------------------------------------
string MusicianMachine::getCurrentStateName(){
    return currentState->getName();
}
//-----------------------------------------------------------
float MusicianMachine::getCurrentStatePercentaje(){
    return currentState->sound.getPosition();
}
//-----------------------------------------------------------
void MusicianMachine::setActive( bool ifIsItActive ){
    isItActive = ifIsItActive;
}
//-----------------------------------------------------------
bool MusicianMachine::isAtcive(){
    return  isItActive;
}
//-----------------------------------------------------------
bool MusicianMachine::justChangedState(){
    if( hasJustChangedState ){
        hasJustChangedState = false;
        return true;
    }
    return false;
}
//-----------------------------------------------------------
bool MusicianMachine::justFinishidState(){
    return !currentState->sound.getIsPlaying();
}
//-----------------------------------------------------------
void MusicianMachine::setCurrentState( MusicianState* newState ){
    cout << "Machine : " << name << "  form " << currentState->getName();
    currentState = newState;
    cout << " to " << currentState->getName() << "\n";
    currentState->sound.play();
    hasJustChangedState = true;
    if( isAtcive() )
        currentState->sound.setVolume(1);
    else
        currentState->sound.setVolume(0);
}