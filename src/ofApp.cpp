#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    MusicianMachine* newMachineA = new MusicianMachine( "Machine_A" );
    MusicianMachine* newMachineB = new MusicianMachine( "Machine_B" );
    MusicianMachine* newMachineC = new MusicianMachine( "Machine_C" );
    MusicianMachine* newMachineD = new MusicianMachine( "Machine_D" );
    MusicianMachine* newMachineE = new MusicianMachine( "Machine_E" );
    MusicianMachine* newMachineF = new MusicianMachine( "Machine_F" );
    MusicianMachine* newMachineG = new MusicianMachine( "Machine_G" );
    
    newMachineA->load("machines/MachinesTest/MachineA.mmf");
    newMachineB->load("machines/MachinesTest/MachineB.mmf");
    newMachineC->load("machines/MachinesTest/MachineC.mmf");
    newMachineD->load("machines/MachinesTest/MachineD.mmf");
    newMachineE->load("machines/MachinesTest/MachineE.mmf");
    newMachineF->load("machines/MachinesTest/MachineF.mmf");
    newMachineG->load("machines/MachinesTest/MachineG.mmf");

    machines.push_back( newMachineA );
    machines.push_back( newMachineB );
    machines.push_back( newMachineC );
    machines.push_back( newMachineD );
    machines.push_back( newMachineE );
    machines.push_back( newMachineF );
    machines.push_back( newMachineG );
    
    createEqualizer();
    createGUI();
    
    for( int m = 0 ; m < machines.size() ; m++)
        machines[m]->start();
}
//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(127,127,127);
    updateEqualizer();
      for( int m = 0 ; m < machines.size() ; m++)
        machines[m]->update();
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255,255,255,255);
    ofPoint equalizerSize = ofPoint( 400 , 70 );
    
    drawEqualizer( ( ofGetWidth() / 2 ) - ( equalizerSize.x / 2 ) , 4*( ofGetHeight() / 5 )  , equalizerSize.x , equalizerSize.y );
    drawGUI();
    drawGUIMouse();
}
//--------------------------------------------------------------
void ofApp::createGUI(){
    cursor.loadImage("GUI/cursor.png");
    GUIFirstCirclePosition = ofPoint( ofGetWidth() / ( 2 * machines.size() ) , ofGetHeight() / 2);
    GUICircleSpacing = 2 * GUIFirstCirclePosition.x;
    GUICircleRadius = ( GUICircleSpacing- .1 * GUICircleSpacing ) / 2;
}
//--------------------------------------------------------------
void ofApp::drawGUI(){
    ofColor colorOn = ofColor( 200 , 200 , 40 );
    ofColor colorOff = ofColor( 50 , 20 , 70 );
    ofPoint firstCirclePosition;
    for( int m = 0 ; m < machines.size() ; m ++ ){
        if( machines[m]->isAtcive() )
            ofSetColor( colorOn );
        else
            ofSetColor( colorOff );
        ofFill();
        ofCircle( GUIFirstCirclePosition.x + GUICircleSpacing * m , GUIFirstCirclePosition.y , GUICircleRadius );
        ofSetColor( colorOff );
        ofDrawBitmapString( machines[m]->getName(),
                           GUIFirstCirclePosition.x + GUICircleSpacing * m - GUICircleRadius / 2 ,
                           GUIFirstCirclePosition.y  -  GUICircleRadius / 5 );
        
        ofDrawBitmapString( machines[m]->getCurrentStateName(),
                           GUIFirstCirclePosition.x + GUICircleSpacing * m - GUICircleRadius / 2,
                           GUIFirstCirclePosition.y  + GUICircleRadius / 5 );
        ofNoFill();
        ofCircle( GUIFirstCirclePosition.x + GUICircleSpacing * m , GUIFirstCirclePosition.y , GUICircleRadius );
        ofCircle( GUIFirstCirclePosition.x + GUICircleSpacing * m , GUIFirstCirclePosition.y , GUICircleRadius * .9 );
        
        
    }
    ofSetColor(255,255,255);
    drawGUIMouse();
}
//--------------------------------------------------------------
void ofApp::mouseReleasedGUI( int x , int y , int buton ){
    int machineClicked = -1;
    for( int m = 0 ; m < machines.size() ; m ++ ){
        ofPoint clickToCircleVector = ofPoint( x - ( GUIFirstCirclePosition.x + GUICircleSpacing * m ), y - GUIFirstCirclePosition.y );
        if( clickToCircleVector.length() < GUICircleRadius ){
            machineClicked = m;
            break;
        }
    }
    if( machineClicked != -1 )
        machines[ machineClicked ]->setActive( !machines[ machineClicked ]->isAtcive() );
}
//--------------------------------------------------------------
void ofApp::drawGUIMouse(){
    ofPoint position = ofPoint( ofGetMouseX() , ofGetMouseY());
    ofEnableAlphaBlending();
    
    cursor.draw(position, 20 , 20 );
    ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::createEqualizer(){
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 128;
}
//--------------------------------------------------------------
void ofApp::updateEqualizer(){
    ofSoundUpdate();
    float * val = ofSoundGetSpectrum(nBandsToGet);
    for (int i = 0;i < nBandsToGet; i++){
        fftSmoothed[i] *= 0.96f;
        if (fftSmoothed[i] < val[i])
            fftSmoothed[i] = val[i];
    }
}
//--------------------------------------------------------------
void ofApp::drawEqualizer( int x , int y , int w , int h ){
    int width = w / nBandsToGet;
    for (int i = 0;i < nBandsToGet; i++){
        ofSetColor(ofMap(fftSmoothed[i] , 0 , 1 , 0 , 255 ),ofMap(fftSmoothed[i] , 0 , 1 , 255 , 0 ),0,255);
        ofFill();
        ofRect( x + i * width , y + h , width ,-(fftSmoothed[i] * h));
         ofNoFill();
        ofSetColor( 200 , 200 , 200 );
        ofRect( x + i * width , y + h , width , - h);
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mouseReleasedGUI( x , y , button );
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    createGUI();
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
//--------------------------------------------------------------
void ofApp::createMachines(){
    MusicianMachine* newMachineA = new MusicianMachine( "Machine_A" );
    MusicianMachine* newMachineB = new MusicianMachine( "Machine_B" );
    MusicianMachine* newMachineC = new MusicianMachine( "Machine_C" );
    MusicianMachine* newMachineD = new MusicianMachine( "Machine_D" );
    MusicianMachine* newMachineE = new MusicianMachine( "Machine_E" );
    MusicianMachine* newMachineF = new MusicianMachine( "Machine_F" );
    MusicianMachine* newMachineG = new MusicianMachine( "Machine_G" );
    
    //Machine A
    newMachineA->addState( "LUZ_A_01" , "sounds/soundsJo_01/LUZ_A_01.aif" , 0 );
    newMachineA->addState( "LUZ_A_02" , "sounds/soundsJo_01/LUZ_A_02.aif" , 0 );
    newMachineA->addState( "LUZ_A_03" , "sounds/soundsJo_01/LUZ_A_03.aif" , 0 );
    newMachineA->addState( "LUZ_A_04" , "sounds/soundsJo_01/LUZ_A_04.aif" , 0 );
    newMachineA->addState( "LUZ_A_05" , "sounds/soundsJo_01/LUZ_A_05.aif" , 0 );
    
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_01" , 0.2 );
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_02" , 0.6 );
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_03" , 0.2 );
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_04" , 0.05 );
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_05" , 0.05 );
    newMachineA->addTransition("LUZ_A_01" , "LUZ_A_0" , 0.05 );
    
    
    newMachineA->addTransition("LUZ_A_02" , "LUZ_A_01" , 0.05 );
    newMachineA->addTransition("LUZ_A_02" , "LUZ_A_02" , 0.1 );
    newMachineA->addTransition("LUZ_A_02" , "LUZ_A_03" , 0.6 );
    newMachineA->addTransition("LUZ_A_02" , "LUZ_A_04" , 0.2 );
    newMachineA->addTransition("LUZ_A_02" , "LUZ_A_05" , 0.05 );
    
    newMachineA->addTransition("LUZ_A_03" , "LUZ_A_01" , 0.05 );
    newMachineA->addTransition("LUZ_A_03" , "LUZ_A_02" , 0.2 );
    newMachineA->addTransition("LUZ_A_03" , "LUZ_A_03" , 0.1 );
    newMachineA->addTransition("LUZ_A_03" , "LUZ_A_04" , 0.6 );
    newMachineA->addTransition("LUZ_A_03" , "LUZ_A_05" , 0.05 );
    
    newMachineA->addTransition("LUZ_A_04" , "LUZ_A_01" , 0.1 );
    newMachineA->addTransition("LUZ_A_04" , "LUZ_A_02" , 0.3 );
    newMachineA->addTransition("LUZ_A_04" , "LUZ_A_03" , 0.05 );
    newMachineA->addTransition("LUZ_A_04" , "LUZ_A_04" , 0.4 );
    newMachineA->addTransition("LUZ_A_04" , "LUZ_A_05" , 0.05 );
    
    newMachineA->addTransition("LUZ_A_05" , "LUZ_A_01" , 0.4 );
    newMachineA->addTransition("LUZ_A_05" , "LUZ_A_02" , 0.2 );
    newMachineA->addTransition("LUZ_A_05" , "LUZ_A_03" , 0.2 );
    newMachineA->addTransition("LUZ_A_05" , "LUZ_A_04" , 0.1 );
    newMachineA->addTransition("LUZ_A_05" , "LUZ_A_05" , 0.1 );
    
    //machine B
    newMachineB->addState( "LUZ_B_01" , "sounds/soundsJo_01/LUZ_B_01.aif" , 0 );
    newMachineB->addState( "LUZ_B_02" , "sounds/soundsJo_01/LUZ_B_02.aif" , 0 );
    newMachineB->addState( "LUZ_B_03" , "sounds/soundsJo_01/LUZ_B_03.aif" , 0 );
    newMachineB->addState( "LUZ_B_04" , "sounds/soundsJo_01/LUZ_B_04.aif" , 0 );
    newMachineB->addState( "LUZ_B_05" , "sounds/soundsJo_01/LUZ_B_05.aif" , 0 );
    
    newMachineB->addTransition("LUZ_B_01" , "LUZ_B_01" , 0.2 );
    newMachineB->addTransition("LUZ_B_01" , "LUZ_B_02" , 0.6 );
    newMachineB->addTransition("LUZ_B_01" , "LUZ_B_03" , 0.2 );
    newMachineB->addTransition("LUZ_B_01" , "LUZ_B_04" , 0.05 );
    newMachineB->addTransition("LUZ_B_01" , "LUZ_B_05" , 0.05 );
    
    newMachineB->addTransition("LUZ_B_02" , "LUZ_B_01" , 0.05 );
    newMachineB->addTransition("LUZ_B_02" , "LUZ_B_02" , 0.1 );
    newMachineB->addTransition("LUZ_B_02" , "LUZ_B_03" , 0.6 );
    newMachineB->addTransition("LUZ_B_02" , "LUZ_B_04" , 0.2 );
    newMachineB->addTransition("LUZ_B_02" , "LUZ_B_05" , 0.05 );
    
    newMachineB->addTransition("LUZ_B_03" , "LUZ_B_01" , 0.05 );
    newMachineB->addTransition("LUZ_B_03" , "LUZ_B_02" , 0.2 );
    newMachineB->addTransition("LUZ_B_03" , "LUZ_B_03" , 0.1 );
    newMachineB->addTransition("LUZ_B_03" , "LUZ_B_04" , 0.6 );
    newMachineB->addTransition("LUZ_B_03" , "LUZ_B_05" , 0.05 );
    
    newMachineB->addTransition("LUZ_B_04" , "LUZ_B_01" , 0.1 );
    newMachineB->addTransition("LUZ_B_04" , "LUZ_B_02" , 0.3 );
    newMachineB->addTransition("LUZ_B_04" , "LUZ_B_03" , 0.05 );
    newMachineB->addTransition("LUZ_B_04" , "LUZ_B_04" , 0.4 );
    newMachineB->addTransition("LUZ_B_04" , "LUZ_B_05" , 0.05 );
    
    newMachineB->addTransition("LUZ_B_05" , "LUZ_B_01" , 0.4 );
    newMachineB->addTransition("LUZ_B_05" , "LUZ_B_02" , 0.2 );
    newMachineB->addTransition("LUZ_B_05" , "LUZ_B_03" , 0.2 );
    newMachineB->addTransition("LUZ_B_05" , "LUZ_B_04" , 0.1 );
    newMachineB->addTransition("LUZ_B_05" , "LUZ_B_05" , 0.1 );
    
    //machine C
    newMachineC->addState( "LUZ_C_01" , "sounds/soundsJo_01/LUZ_C_01.aif" , 0 );
    newMachineC->addState( "LUZ_C_02" , "sounds/soundsJo_01/LUZ_C_02.aif" , 0 );
    newMachineC->addState( "LUZ_C_03" , "sounds/soundsJo_01/LUZ_C_03.aif" , 0 );
    newMachineC->addState( "LUZ_C_04" , "sounds/soundsJo_01/LUZ_C_04.aif" , 0 );
    newMachineC->addState( "LUZ_C_05" , "sounds/soundsJo_01/LUZ_C_05.aif" , 0 );
    
    newMachineC->addTransition("LUZ_C_01" , "LUZ_C_01" , 0.2 );
    newMachineC->addTransition("LUZ_C_01" , "LUZ_C_02" , 0.6 );
    newMachineC->addTransition("LUZ_C_01" , "LUZ_C_03" , 0.2 );
    newMachineC->addTransition("LUZ_C_01" , "LUZ_C_04" , 0.05 );
    newMachineC->addTransition("LUZ_C_01" , "LUZ_C_05" , 0.05 );
    
    newMachineC->addTransition("LUZ_C_02" , "LUZ_C_01" , 0.05 );
    newMachineC->addTransition("LUZ_C_02" , "LUZ_C_02" , 0.1 );
    newMachineC->addTransition("LUZ_C_02" , "LUZ_C_03" , 0.6 );
    newMachineC->addTransition("LUZ_C_02" , "LUZ_C_04" , 0.2 );
    newMachineC->addTransition("LUZ_C_02" , "LUZ_C_05" , 0.05 );
    
    newMachineC->addTransition("LUZ_C_03" , "LUZ_C_01" , 0.05 );
    newMachineC->addTransition("LUZ_C_03" , "LUZ_C_02" , 0.2 );
    newMachineC->addTransition("LUZ_C_03" , "LUZ_C_03" , 0.1 );
    newMachineC->addTransition("LUZ_C_03" , "LUZ_C_04" , 0.6 );
    newMachineC->addTransition("LUZ_C_03" , "LUZ_C_05" , 0.05 );
    
    newMachineC->addTransition("LUZ_C_04" , "LUZ_C_01" , 0.1 );
    newMachineC->addTransition("LUZ_C_04" , "LUZ_C_02" , 0.3 );
    newMachineC->addTransition("LUZ_C_04" , "LUZ_C_03" , 0.05 );
    newMachineC->addTransition("LUZ_C_04" , "LUZ_C_04" , 0.4 );
    newMachineC->addTransition("LUZ_C_04" , "LUZ_C_05" , 0.05 );
    
    newMachineC->addTransition("LUZ_C_05" , "LUZ_C_01" , 0.4 );
    newMachineC->addTransition("LUZ_C_05" , "LUZ_C_02" , 0.2 );
    newMachineC->addTransition("LUZ_C_05" , "LUZ_C_03" , 0.2 );
    newMachineC->addTransition("LUZ_C_05" , "LUZ_C_04" , 0.1 );
    newMachineC->addTransition("LUZ_C_05" , "LUZ_C_05" , 0.1 );
    
    //Machine D
    newMachineD->addState( "LUZ_D_01" , "sounds/soundsJo_01/LUZ_D_01.aif" , 0 );
    newMachineD->addState( "LUZ_D_02" , "sounds/soundsJo_01/LUZ_D_02.aif" , 0 );
    newMachineD->addState( "LUZ_D_03" , "sounds/soundsJo_01/LUZ_D_03.aif" , 0 );
    newMachineD->addState( "LUZ_D_04" , "sounds/soundsJo_01/LUZ_D_04.aif" , 0 );
    newMachineD->addState( "LUZ_D_05" , "sounds/soundsJo_01/LUZ_D_05.aif" , 0 );
    
    newMachineD->addTransition("LUZ_D_01" , "LUZ_D_01" , 0.2 );
    newMachineD->addTransition("LUZ_D_01" , "LUZ_D_02" , 0.6 );
    newMachineD->addTransition("LUZ_D_01" , "LUZ_D_03" , 0.2 );
    newMachineD->addTransition("LUZ_D_01" , "LUZ_D_04" , 0.05 );
    newMachineD->addTransition("LUZ_D_01" , "LUZ_D_05" , 0.05 );
    
    newMachineD->addTransition("LUZ_D_02" , "LUZ_D_01" , 0.05 );
    newMachineD->addTransition("LUZ_D_02" , "LUZ_D_02" , 0.1 );
    newMachineD->addTransition("LUZ_D_02" , "LUZ_D_03" , 0.6 );
    newMachineD->addTransition("LUZ_D_02" , "LUZ_D_04" , 0.2 );
    newMachineD->addTransition("LUZ_D_02" , "LUZ_D_05" , 0.05 );
    
    newMachineD->addTransition("LUZ_D_03" , "LUZ_D_01" , 0.05 );
    newMachineD->addTransition("LUZ_D_03" , "LUZ_D_02" , 0.2 );
    newMachineD->addTransition("LUZ_D_03" , "LUZ_D_03" , 0.1 );
    newMachineD->addTransition("LUZ_D_03" , "LUZ_D_04" , 0.6 );
    newMachineD->addTransition("LUZ_D_03" , "LUZ_D_05" , 0.05 );
    
    newMachineD->addTransition("LUZ_D_04" , "LUZ_D_01" , 0.1 );
    newMachineD->addTransition("LUZ_D_04" , "LUZ_D_02" , 0.3 );
    newMachineD->addTransition("LUZ_D_04" , "LUZ_D_03" , 0.05 );
    newMachineD->addTransition("LUZ_D_04" , "LUZ_D_04" , 0.4 );
    newMachineD->addTransition("LUZ_D_04" , "LUZ_D_05" , 0.05 );
    
    newMachineD->addTransition("LUZ_D_05" , "LUZ_D_01" , 0.4 );
    newMachineD->addTransition("LUZ_D_05" , "LUZ_D_02" , 0.2 );
    newMachineD->addTransition("LUZ_D_05" , "LUZ_D_03" , 0.2 );
    newMachineD->addTransition("LUZ_D_05" , "LUZ_D_04" , 0.1 );
    newMachineD->addTransition("LUZ_D_05" , "LUZ_D_05" , 0.1 );
    
    //Machine E
    newMachineE->addState( "LUZ_E_01" , "sounds/soundsJo_01/LUZ_E_01.aif" , 0 );
    newMachineE->addState( "LUZ_E_02" , "sounds/soundsJo_01/LUZ_E_02.aif" , 0 );
    newMachineE->addState( "LUZ_E_03" , "sounds/soundsJo_01/LUZ_E_03.aif" , 0 );
    newMachineE->addState( "LUZ_E_04" , "sounds/soundsJo_01/LUZ_E_04.aif" , 0 );
    newMachineE->addState( "LUZ_E_05" , "sounds/soundsJo_01/LUZ_E_05.aif" , 0 );
    newMachineE->addState( "LUZ_E_06" , "sounds/soundsJo_01/LUZ_E_06.aif" , 0 );
    
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_01" , 0.2 );
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_02" , 0.6 );
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_03" , 0.2 );
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_04" , 0.05 );
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_05" , 0.05 );
    newMachineE->addTransition("LUZ_E_01" , "LUZ_E_06" , 0.1 );
    
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_01" , 0.05 );
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_02" , 0.1 );
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_03" , 0.6 );
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_04" , 0.2 );
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_05" , 0.05 );
    newMachineE->addTransition("LUZ_E_02" , "LUZ_E_06" , 0.1 );
    
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_01" , 0.05 );
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_02" , 0.2 );
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_03" , 0.1 );
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_04" , 0.6 );
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_05" , 0.05 );
    newMachineE->addTransition("LUZ_E_03" , "LUZ_E_06" , 0.1 );
    
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_01" , 0.1 );
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_02" , 0.3 );
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_03" , 0.05 );
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_04" , 0.4 );
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_05" , 0.05 );
    newMachineE->addTransition("LUZ_E_04" , "LUZ_E_06" , 0.1 );
    
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_01" , 0.4 );
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_02" , 0.2 );
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_03" , 0.2 );
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_04" , 0.1 );
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_05" , 0.1 );
    newMachineE->addTransition("LUZ_E_05" , "LUZ_E_06" , 0.1 );
    
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_01" , 0.4 );
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_02" , 0.2 );
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_03" , 0.2 );
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_04" , 0.1 );
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_05" , 0.1 );
    newMachineE->addTransition("LUZ_E_06" , "LUZ_E_06" , 0.1 );
    
    //Machine F
    newMachineF->addState( "LUZ_F_01" , "sounds/soundsJo_01/LUZ_F_01.aif" , 0 );
    newMachineF->addState( "LUZ_F_02" , "sounds/soundsJo_01/LUZ_F_02.aif" , 0 );
    newMachineF->addState( "LUZ_F_03" , "sounds/soundsJo_01/LUZ_F_03.aif" , 0 );
    newMachineF->addState( "LUZ_F_04" , "sounds/soundsJo_01/LUZ_F_04.aif" , 0 );
    
    newMachineF->addTransition("LUZ_F_01" , "LUZ_F_01" , 0.2 );
    newMachineF->addTransition("LUZ_F_01" , "LUZ_F_02" , 0.6 );
    newMachineF->addTransition("LUZ_F_01" , "LUZ_F_03" , 0.2 );
    newMachineF->addTransition("LUZ_F_01" , "LUZ_F_04" , 0.1 );
    
    newMachineF->addTransition("LUZ_F_02" , "LUZ_F_01" , 0.1 );
    newMachineF->addTransition("LUZ_F_02" , "LUZ_F_02" , 0.1 );
    newMachineF->addTransition("LUZ_F_02" , "LUZ_F_03" , 0.6 );
    newMachineF->addTransition("LUZ_F_02" , "LUZ_F_04" , 0.2 );
    
    
    newMachineF->addTransition("LUZ_F_03" , "LUZ_F_01" , 0.1 );
    newMachineF->addTransition("LUZ_F_03" , "LUZ_F_02" , 0.2 );
    newMachineF->addTransition("LUZ_F_03" , "LUZ_F_03" , 0.1 );
    newMachineF->addTransition("LUZ_F_03" , "LUZ_F_04" , 0.6 );
    
    
    newMachineF->addTransition("LUZ_F_04" , "LUZ_F_01" , 0.1 );
    newMachineF->addTransition("LUZ_F_04" , "LUZ_F_02" , 0.3 );
    newMachineF->addTransition("LUZ_F_04" , "LUZ_F_03" , 0.1 );
    newMachineF->addTransition("LUZ_F_04" , "LUZ_F_04" , 0.4 );
    
    //Machine G
    newMachineG->addState( "LUZ_G_01" , "sounds/soundsJo_01/LUZ_G_01.aif" , 0 );
    newMachineG->addState( "LUZ_G_02" , "sounds/soundsJo_01/LUZ_G_02.aif" , 0 );
    newMachineG->addState( "LUZ_G_03" , "sounds/soundsJo_01/LUZ_G_04.aif" , 0 );
    newMachineG->addState( "LUZ_G_04" , "sounds/soundsJo_01/LUZ_G_04.aif" , 0 );
    newMachineG->addState( "LUZ_G_05" , "sounds/soundsJo_01/LUZ_G_05.aif" , 0 );
    
    newMachineG->addTransition("LUZ_G_01" , "LUZ_G_01" , 0.2 );
    newMachineG->addTransition("LUZ_G_01" , "LUZ_G_02" , 0.6 );
    newMachineG->addTransition("LUZ_G_01" , "LUZ_G_03" , 0.2 );
    newMachineG->addTransition("LUZ_G_01" , "LUZ_G_04" , 0.05 );
    newMachineG->addTransition("LUZ_G_01" , "LUZ_G_05" , 0.05 );
    
    newMachineG->addTransition("LUZ_G_02" , "LUZ_G_01" , 0.05 );
    newMachineG->addTransition("LUZ_G_02" , "LUZ_G_02" , 0.1 );
    newMachineG->addTransition("LUZ_G_02" , "LUZ_G_03" , 0.6 );
    newMachineG->addTransition("LUZ_G_02" , "LUZ_G_04" , 0.2 );
    newMachineG->addTransition("LUZ_G_02" , "LUZ_G_05" , 0.05 );
    
    newMachineG->addTransition("LUZ_G_03" , "LUZ_G_01" , 0.05 );
    newMachineG->addTransition("LUZ_G_03" , "LUZ_G_02" , 0.2 );
    newMachineG->addTransition("LUZ_G_03" , "LUZ_G_03" , 0.1 );
    newMachineG->addTransition("LUZ_G_03" , "LUZ_G_04" , 0.6 );
    newMachineG->addTransition("LUZ_G_03" , "LUZ_G_05" , 0.05 );
    
    newMachineG->addTransition("LUZ_G_04" , "LUZ_G_01" , 0.1 );
    newMachineG->addTransition("LUZ_G_04" , "LUZ_G_02" , 0.3 );
    newMachineG->addTransition("LUZ_G_04" , "LUZ_G_03" , 0.05 );
    newMachineG->addTransition("LUZ_G_04" , "LUZ_G_04" , 0.4 );
    newMachineG->addTransition("LUZ_G_04" , "LUZ_G_05" , 0.05 );
    
    newMachineG->addTransition("LUZ_G_05" , "LUZ_G_01" , 0.4 );
    newMachineG->addTransition("LUZ_G_05" , "LUZ_G_02" , 0.2 );
    newMachineG->addTransition("LUZ_G_05" , "LUZ_G_03" , 0.2 );
    newMachineG->addTransition("LUZ_G_05" , "LUZ_G_04" , 0.1 );
    newMachineG->addTransition("LUZ_G_05" , "LUZ_G_05" , 0.1 );
    
    newMachineA->save( "machines/MachineA.mmf");
    newMachineB->save( "machines/MachineB.mmf");
    newMachineC->save( "machines/MachineC.mmf");
    newMachineD->save( "machines/MachineD.mmf");
    newMachineE->save( "machines/MachineE.mmf");
    newMachineF->save( "machines/MachineF.mmf");
    newMachineG->save( "machines/MachineG.mmf");
    
    machines.push_back( newMachineA );
//    machines.push_back( newMachineB );
//    machines.push_back( newMachineC );
//    machines.push_back( newMachineD );
//    machines.push_back( newMachineE );
//    machines.push_back( newMachineF );
//    machines.push_back( newMachineG );
}
