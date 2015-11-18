//
//  MusicianMachineView.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#include "MusicianMachineView.h"
//-------------------------------------------------------------
//-------------------------------------------------------------
//public
//-------------------------------------------------------------
//-------------------------------------------------------------
MusicianMachineView::MusicianMachineView( string theName ){
    name = theName;
    stateCurrent = -1;
    statePrevious = 0;
    lastTransition = 0;
    radiusSphere = 20 ;
    radiusCylinder = 2;
    fileIn = 0;
    fileOut = 0;
    isItActive = true;
    
    ofDisableArbTex();
    //texture.loadImage("GUI/earth.png");
    //texture.rotate90(2);
    //texture.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
}
//-------------------------------------------------------------
MusicianMachineView::~MusicianMachineView(){
    clear();
}
//-------------------------------------------------------------
bool MusicianMachineView::addState( string theName , float theParam01 ){
    ofIcoSpherePrimitive* newSphere = new ofIcoSpherePrimitive();
    newSphere->setRadius(radiusSphere);
    float delta;// = ofRandom( -6 * radiusSphere ,  6 * radiusSphere );
    if( states.size() == 0 )
        delta = 0;
    newSphere->setPosition( delta , 6 * radiusSphere * theParam01 , 0 );
    newSphere->setResolution(4);
    states.insert(pair<int,ofIcoSpherePrimitive*>(states.size(),newSphere));
    statesNames.push_back( theName );
    statesEnergy.push_back(theParam01);
    if( stateCurrent == -1 )
        stateCurrent = 0;
    return true;
}
//-------------------------------------------------------------
bool MusicianMachineView::addTransition( string nameState01 , string nameState02 , float theProbability ){
    ofCylinderPrimitive* newcilinder = new ofCylinderPrimitive();
    ofVec3f initStatePosition = states[getStateIndex(nameState01)]->getPosition();
    ofVec3f finalStatePosition = states[getStateIndex(nameState02)]->getPosition();
    newcilinder->setPosition( initStatePosition );
    newcilinder->lookAt( finalStatePosition - initStatePosition );
    newcilinder->setResolution(15, 5, 5);
    newcilinder->setRadius( radiusCylinder );
    transitions.insert(pair<int,ofCylinderPrimitive*>(transitions.size(),newcilinder));
    transitionStateNameInitial.push_back(nameState01);
    transitionStateNameFinal.push_back(nameState02);
    transitionStateProbabilities.push_back(theProbability);
    return true;
}
//-------------------------------------------------------------
void MusicianMachineView::draw(){
    if( !isItActive )
        return;
    
    map<int,ofIcoSpherePrimitive*>::iterator it = states.begin();
    int stateIndex = 0;
    while( it != states.end() ){
        ofIcoSpherePrimitive* tempSphere = (*it).second;
        if( stateCurrent  == stateIndex || statePrevious  == stateIndex ){
            float amplitude;
            float liquidness = 2;
            float speedDampen = 5;
            
            if( stateCurrent  == stateIndex && stateCurrent != 0 ){
                amplitude = 20;
                liquidness = 1;
                speedDampen = 1;
                ofSetColor(30);
            }
            else {
                amplitude = 10;
                liquidness = 2;
                speedDampen = 5;
                ofSetColor(100);
            }
            
            ofVboMesh mesh = tempSphere->getMesh();
            vector<ofVec3f>& verts = mesh.getVertices();
            for(unsigned int i = 0; i < verts.size(); i++){
                verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
                verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
                verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            }
            ofPushMatrix();
            ofTranslate( tempSphere->getPosition() );
            
            //texture.getTextureReference().bind();
            mesh.drawFaces();
            //texture.getTextureReference().unbind();
            
            ofSetColor(255);
            ofPopMatrix();
        }
        else{
            //texture.getTextureReference().bind();
            tempSphere->draw();
            //texture.getTextureReference().unbind();
        }
        it++;
        stateIndex++;
    }
    
    map<int,ofCylinderPrimitive*>::iterator itt = transitions.begin();
    int transitionIndex = 0;
    int numTransitions = transitions.size();
    while( itt != transitions.end() ){
        //        ofCylinderPrimitive* tempCilinder = (*itt).second;
        //        if( stateCurrent  == getStateIndex( transitionStateNameFinal[transitionIndex] ) || statePrevious  == getStateIndex( transitionStateNameInitial[transitionIndex] ) ){
        //            float amplitude = 3;
        //            float liquidness = 2;
        //            float speedDampen = 5;
        //            ofVboMesh mesh = tempCilinder->getMesh();
        //            vector<ofVec3f>& verts = mesh.getVertices();
        //            for(unsigned int i = 0; i < verts.size(); i++){
        //                verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        //                verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        //                verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        //            }
        //            ofPushMatrix();
        //            ofTranslate( tempCilinder->getPosition() );
        //            //mesh.drawFaces();
        //            ofPopMatrix();
        //        }
        //
        //        else
        //            tempCilinder->draw();
        
        
        
        int indexStateInitial   = getStateIndex( transitionStateNameInitial[transitionIndex] );
        int indexStateFinal     = getStateIndex( transitionStateNameFinal[transitionIndex] );
        
        int energyInitial = statesEnergy[ indexStateInitial ];
        int energyFinal = statesEnergy[ indexStateFinal ];
        
        if( indexStateInitial != indexStateFinal ){
            
            ofVec3f statePositionInitial = states[ indexStateInitial    ]->getPosition();
            ofVec3f statePositionFinal   = states[ indexStateFinal      ]->getPosition();
            ofVec3f directionVector = statePositionFinal - statePositionInitial;
            float legth = directionVector.length();
            directionVector = directionVector.normalize();
            
            
            ofVec3f miiddlePoint = statePositionInitial + directionVector * ( legth / 2.0f );
            
            if( energyFinal > energyInitial ){
                miiddlePoint += ofVec3f( -4 , 0 , 0 ) * ( numTransitions - transitionIndex );
                ofSetColor( ofMap( transitionStateProbabilities[ transitionIndex ] , 0 , 1 , 0 , 255 ) , 10 , 10  );
            }
            else{
                miiddlePoint += ofVec3f( 4 , 0 , 0 ) * ( numTransitions - transitionIndex );
                ofSetColor( 10 , 10 , ofMap( transitionStateProbabilities[ transitionIndex ] , 0 , 1 , 0 , 255 )  );
            }
            
            if( stateCurrent  == indexStateFinal && statePrevious  == indexStateInitial )
                ofSetColor( 10 , 200 , 10  );
            
            ofDisableLighting();
            ofLine( statePositionInitial , miiddlePoint );
            ofLine( miiddlePoint , statePositionFinal );
            ofDrawBitmapString( ofToString( transitionStateProbabilities[ transitionIndex ]), miiddlePoint.x, miiddlePoint.y );
            ofEnableLighting();
            ofFill();
            ofDrawSphere( miiddlePoint, ofMap( transitionStateProbabilities[ transitionIndex ] , 0 , 1 , 0 , 25 ));
            ofSetColor(255 );
        }
        itt++;
        transitionIndex++;
    }
}
///----------------------------------------------------------------------------------
bool MusicianMachineView::load( string fileName ){
    if( !fileIn )
        fileIn = new ifstream();
    
    fileIn->open( ofToDataPath( fileName ).c_str() , std::ios_base::binary | std::ios_base::in );
    if ( !fileIn->is_open() ){
        cout << "MachineView File not found: ";
        cout << fileName << "\n";
        fileIn->close();
        return false;
    }
    
    std::string junk;
    int numStates;
    int numTransitions;
    
    (*fileIn) >> junk;
    if( junk != "numStates=" ){
        cout << "* MusicianMachineView  load: Bad tag numStates\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numStates;
    
    (*fileIn) >> junk;
    if( junk != "numTransitions=" ){
        cout << "* MusicianMachineView  load: Bad tag numTransitions\n";
        fileIn->close();
        return false;
    }
    (*fileIn) >> numTransitions;
    
    clear();
    
    for( int s = 0 ; s < numStates ; s ++ ){
        std::string junk;
        
        (*fileIn) >> junk;
        if( junk != "name=" ){
            cout << "* Musician  State load: Bad tag name\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> name;
        
        
        (*fileIn) >> junk;
        if( junk != "soundFileName=" ){
            cout << "* Musician  State load: Bad tag soundFileName\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> junk;
        
        (*fileIn) >> junk;
        if( junk != "param01=" ){
            cout << "* Musician  State load: Bad tag param01\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> junk;
        addState( name , ofToFloat(junk) );
    }
    
    for( int t = 0 ; t < numTransitions ; t ++ ){
        (*fileIn) >> junk;
        std::string stateNameInitial;
        if( junk != "stateNameInitial=" ){
            cout << "* MusicianMachineView  load: Bad tag stateNameInitial\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameInitial;
        
        (*fileIn) >> junk;
        std::string stateNameFinal;
        if( junk != "stateNameFinal=" ){
            cout << "* MusicianMachineView  load: Bad tag stateNameFinal\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> stateNameFinal;
        
        (*fileIn) >> junk;
        float probability;
        if( junk != "probability=" ){
            cout << "* MusicianMachineView  load: Bad tag probability\n";
            fileIn->close();
            return false;
        }
        (*fileIn) >> probability;
        
        if( !addTransition( stateNameInitial , stateNameFinal, probability ) ){
            cout << "* MusicianMachineView  load: couldnt add transition at load\n";
            fileIn->close();
            return false;
        }
    }
    fileIn->close();
    return true;
}
//-----------------------------------------------------------
void MusicianMachineView::setActive( bool ifIsItActive ){
    isItActive = ifIsItActive;
}
//-----------------------------------------------------------
bool MusicianMachineView::setCurrentState( string stateName ){
    int nexState = getStateIndex( stateName );
    if( nexState != -1 ){
        statePrevious = stateCurrent;
        stateCurrent = nexState;
        return true;
    }
    return false;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//private
//-------------------------------------------------------------
//-------------------------------------------------------------
void MusicianMachineView::clear(){
    for( int s = 0 ; s < states.size() ; s ++ )
        delete states[s];
    states.clear();
    
    for( int t = 0 ; t < transitions.size() ; t ++ )
        delete transitions[t];
    transitions.clear();
}
//-------------------------------------------------------------
int MusicianMachineView::getStateIndex( string name){
    for( int s = 0 ; s < statesNames.size() ; s ++ )
        if( name == statesNames[s] )
            return s;
    return -1;
}
