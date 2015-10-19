#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    
    MusicianMachine* newMachinePiano = new MusicianMachine( "PIANO" );
   // MusicianMachine* newMachineViola = new MusicianMachine( "VIOLA" );
  //  MusicianMachine* newMachineViolin01 = new MusicianMachine( "VIOLIN 01" );
  //  MusicianMachine* newMachineViolin02 = new MusicianMachine( "VIOLIN 02" );
  //  MusicianMachine* newMachineViolin03 = new MusicianMachine( "VIOLIN 03" );
  //  MusicianMachine* newMachineViolonchelo = new MusicianMachine( "VIOLINONCHELO" );
    
    newMachinePiano->load("machines/MachinesJo_02/MachinePiano.mmf");
    //newMachineViola->load("machines/MachinesJo_02/MachineViola.mmf");
    //newMachineViolin01->load("machines/MachinesJo_02/MachineViolin01.mmf");
    //newMachineViolin02->load("machines/MachinesJo_02/MachineViolin02.mmf");
    //newMachineViolin03->load("machines/MachinesJo_02/MachineViolin03.mmf");
    //newMachineViolonchelo->load("machines/MachinesJo_02/MachineViolonchelo.mmf");
 
    machines.push_back( newMachinePiano );
    //machines.push_back( newMachineViola );
    //machines.push_back( newMachineViolin01 );
    //machines.push_back( newMachineViolin02 );
    //machines.push_back( newMachineViolin03 );
   // machines.push_back( newMachineViolonchelo );
     
    //createMachines();

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
    MusicianMachine* newMachinePiano        = new MusicianMachine( "PIANO" );
    MusicianMachine* newMachineViola        = new MusicianMachine( "VIOLA" );
    MusicianMachine* newMachineViolin01     = new MusicianMachine( "VIOLIN 01" );
    MusicianMachine* newMachineViolin02     = new MusicianMachine( "VIOLIN 02" );
    MusicianMachine* newMachineViolin03     = new MusicianMachine( "VIOLIN 03" );
    MusicianMachine* newMachineViolonchelo  = new MusicianMachine( "VIOLINONCHELO" );
    
    //Machine Pianno
    newMachinePiano->addState( "PIANO_SILENCE"  , "sounds/soundsJo_03/Luz_Piano_Part_Silence.wav" , 0 );
    newMachinePiano->addState( "PIANO_01"       , "sounds/soundsJo_03/Luz_Piano_Part_01.mp3" , 0 );
    
    newMachinePiano->addTransition("PIANO_SILENCE" , "PIANO_01"         , 0.5 );
    newMachinePiano->addTransition("PIANO_SILENCE" , "PIANO_SILENCE"    , 0.5 );
    
    newMachinePiano->addTransition("PIANO_01" , "PIANO_SILENCE" , 0.2 );
    newMachinePiano->addTransition("PIANO_01" , "PIANO_01" , 0.8 );
    
    //machine Viola
    newMachineViola->addState( "VIOLA_SILENCE"  , "sounds/soundsJo_03/Luz_Viola_Part_Silence.wav" , 0 );
    newMachineViola->addState( "VIOLA_01"       , "sounds/soundsJo_03/Luz_Viola_Part_01.mp3" , 0 );
    newMachineViola->addState( "VIOLA_02"       , "sounds/soundsJo_03/Luz_Viola_Part_02.mp3" , 0 );
    newMachineViola->addState( "VIOLA_03"       , "sounds/soundsJo_03/Luz_Viola_Part_03.mp3" , 0 );

    newMachineViola->addTransition("VIOLA_SILENCE"  , "VIOLA_SILENCE"   , 0.2 );
    newMachineViola->addTransition("VIOLA_SILENCE"  , "VIOLA_01"        , 0.4 );
    newMachineViola->addTransition("VIOLA_SILENCE"  , "VIOLA_02"        , 0.3 );
    newMachineViola->addTransition("VIOLA_SILENCE"  , "VIOLA_03"        , 0.1 );
    
    newMachineViola->addTransition("VIOLA_01"       , "VIOLA_SILENCE"   , 0.2 );
    newMachineViola->addTransition("VIOLA_01"       , "VIOLA_01"        , 0.2 );
    newMachineViola->addTransition("VIOLA_01"       , "VIOLA_02"        , 0.4 );
    newMachineViola->addTransition("VIOLA_01"       , "VIOLA_03"        , 0.2 );
    
    newMachineViola->addTransition("VIOLA_02"       , "VIOLA_SILENCE"   , 0.1 );
    newMachineViola->addTransition("VIOLA_02"       , "VIOLA_01"        , 0.1 );
    newMachineViola->addTransition("VIOLA_02"       , "VIOLA_02"        , 0.4 );
    newMachineViola->addTransition("VIOLA_02"       , "VIOLA_03"        , 0.4 );
    
    newMachineViola->addTransition("VIOLA_03"       , "VIOLA_SILENCE"   , 0.3 );
    newMachineViola->addTransition("VIOLA_03"       , "VIOLA_01"        , 0.1 );
    newMachineViola->addTransition("VIOLA_03"       , "VIOLA_02"        , 0.5 );
    newMachineViola->addTransition("VIOLA_03"       , "VIOLA_03"        , 0.1 );

    //machine Violin 01
    newMachineViolin01->addState( "VIOLIN_#1_SILENCE" , "sounds/soundsJo_03/Luz_Violin_01_Part_Silence.wav" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_01" , "sounds/soundsJo_03/Luz_Violin_01_Part_01.mp3" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_02" , "sounds/soundsJo_03/Luz_Violin_01_Part_02.mp3" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_03" , "sounds/soundsJo_03/Luz_Violin_01_Part_03.mp3" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_04" , "sounds/soundsJo_03/Luz_Violin_01_Part_04.mp3" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_05" , "sounds/soundsJo_03/Luz_Violin_01_Part_05.mp3" , 0 );
    newMachineViolin01->addState( "VIOLIN_#1_06" , "sounds/soundsJo_03/Luz_Violin_01_Part_06.mp3" , 0 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_SILENCE" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_01"      , 0.5 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_02"      , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_03"      , 0.05 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_04"      , 0.05 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_05"      , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_SILENCE" , "VIOLIN_#1_06"      , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_SILENCE"  , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_01"       , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_02"       , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_03"       , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_04"       , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_05"       , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_01" , "VIOLIN_#1_06"       , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_SILENCE" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_01" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_02" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_03" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_04" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_05" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_02" , "VIOLIN_#1_06" , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_SILENCE" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_01" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_02" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_03" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_04" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_05" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_03" , "VIOLIN_#1_06" , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_SILENCE" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_01" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_02" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_03" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_04" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_05" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_04" , "VIOLIN_#1_06" , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_SILENCE" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_01" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_02" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_03" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_04" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_05" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_05" , "VIOLIN_#1_06" , 0.1 );
    
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_SILENCE" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_01" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_02" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_03" , 0.2 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_04" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_05" , 0.1 );
    newMachineViolin01->addTransition("VIOLIN_#1_06" , "VIOLIN_#1_06" , 0.1 );
    
    //machine Violin 02
    newMachineViolin02->addState( "VIOLIN_#2_SILENCE" , "sounds/soundsJo_03/Luz_Violin_02_Part_Silence.wav" , 0 );
    newMachineViolin02->addState( "VIOLIN_#2_01" , "sounds/soundsJo_03/Luz_Violin_03_Part_01.mp3" , 0 );
    newMachineViolin02->addState( "VIOLIN_#2_02" , "sounds/soundsJo_03/Luz_Violin_03_Part_02.mp3" , 0 );
    newMachineViolin02->addState( "VIOLIN_#2_03" , "sounds/soundsJo_03/Luz_Violin_03_Part_03.mp3" , 0 );
    newMachineViolin02->addState( "VIOLIN_#2_04" , "sounds/soundsJo_03/Luz_Violin_03_Part_04.mp3" , 0 );
    
    newMachineViolin02->addTransition("VIOLIN_#2_SILENCE" , "VIOLIN_#2_SILENCE" , 0.1 );
    newMachineViolin02->addTransition("VIOLIN_#2_SILENCE" , "VIOLIN_#2_01" , 0.3 );
    newMachineViolin02->addTransition("VIOLIN_#2_SILENCE" , "VIOLIN_#2_02" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_SILENCE" , "VIOLIN_#2_03" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_SILENCE" , "VIOLIN_#2_04" , 0.2 );
    
    newMachineViolin02->addTransition("VIOLIN_#2_01" , "VIOLIN_#2_SILENCE" , 0.1 );
    newMachineViolin02->addTransition("VIOLIN_#2_01" , "VIOLIN_#2_01" , 0.1 );
    newMachineViolin02->addTransition("VIOLIN_#2_01" , "VIOLIN_#2_02" , 0.3 );
    newMachineViolin02->addTransition("VIOLIN_#2_01" , "VIOLIN_#2_03" , 0.3 );
    newMachineViolin02->addTransition("VIOLIN_#2_01" , "VIOLIN_#2_04" , 0.2 );
    
    newMachineViolin02->addTransition("VIOLIN_#2_02" , "VIOLIN_#2_SILENCE" , 0.3 );
    newMachineViolin02->addTransition("VIOLIN_#2_02" , "VIOLIN_#2_01" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_02" , "VIOLIN_#2_02" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_02" , "VIOLIN_#2_03" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_02" , "VIOLIN_#2_04" , 0.3 );
    
    newMachineViolin02->addTransition("VIOLIN_#2_03" , "VIOLIN_#2_SILENCE" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_03" , "VIOLIN_#2_01" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_03" , "VIOLIN_#2_02" , 0.4 );
    newMachineViolin02->addTransition("VIOLIN_#2_03" , "VIOLIN_#2_02" , 0.4 );
    newMachineViolin02->addTransition("VIOLIN_#2_03" , "VIOLIN_#2_04" , 0.2 );
    
    newMachineViolin02->addTransition("VIOLIN_#2_04" , "VIOLIN_#2_SILENCE" , 0.3 );
    newMachineViolin02->addTransition("VIOLIN_#2_04" , "VIOLIN_#2_01" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_04" , "VIOLIN_#2_02" , 0.2 );
    newMachineViolin02->addTransition("VIOLIN_#2_04" , "VIOLIN_#2_03" , 0.1 );
    newMachineViolin02->addTransition("VIOLIN_#2_04" , "VIOLIN_#2_04" , 0.2 );
    
    //machine Violin 03
    newMachineViolin03->addState( "VIOLIN_#3_SILENCE" , "sounds/soundsJo_03/Luz_Violin_01_Part_Silence.wav" , 0 );
    newMachineViolin03->addState( "VIOLIN_#3_01" , "sounds/soundsJo_03/Luz_Violin_01_Part_01.mp3" , 0 );
    newMachineViolin03->addState( "VIOLIN_#3_02" , "sounds/soundsJo_03/Luz_Violin_01_Part_02.mp3" , 0 );
    newMachineViolin03->addState( "VIOLIN_#3_03" , "sounds/soundsJo_03/Luz_Violin_01_Part_03.mp3" , 0 );
    newMachineViolin03->addState( "VIOLIN_#3_04" , "sounds/soundsJo_03/Luz_Violin_01_Part_04.mp3" , 0 );
    
    newMachineViolin03->addTransition("VIOLIN_#3_SILENCE" , "VIOLIN_#3_SILENCE" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_SILENCE" , "VIOLIN_#3_01" , 0.4 );
    newMachineViolin03->addTransition("VIOLIN_#3_SILENCE" , "VIOLIN_#3_02" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_SILENCE" , "VIOLIN_#3_03" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_SILENCE" , "VIOLIN_#3_04" , 0.1 );

    newMachineViolin03->addTransition("VIOLIN_#3_01" , "VIOLIN_#3_SILENCE" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_01" , "VIOLIN_#3_01" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_01" , "VIOLIN_#3_02" , 0.4 );
    newMachineViolin03->addTransition("VIOLIN_#3_01" , "VIOLIN_#3_03" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_01" , "VIOLIN_#3_04" , 0.2 );
    
    newMachineViolin03->addTransition("VIOLIN_#3_02" , "VIOLIN_#3_SILENCE" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_02" , "VIOLIN_#3_01" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_02" , "VIOLIN_#3_02" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_02" , "VIOLIN_#3_03" , 0.4 );
    newMachineViolin03->addTransition("VIOLIN_#3_02" , "VIOLIN_#3_04" , 0.2 );
    
    newMachineViolin03->addTransition("VIOLIN_#3_03" , "VIOLIN_#3_SILENCE" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_03" , "VIOLIN_#3_01" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_03" , "VIOLIN_#3_02" , 0.3 );
    newMachineViolin03->addTransition("VIOLIN_#3_03" , "VIOLIN_#3_03" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_03" , "VIOLIN_#3_04" , 0.3 );
    
    newMachineViolin03->addTransition("VIOLIN_#3_04" , "VIOLIN_#3_SILENCE" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_04" , "VIOLIN_#3_01" , 0.1 );
    newMachineViolin03->addTransition("VIOLIN_#3_04" , "VIOLIN_#3_02" , 0.2 );
    newMachineViolin03->addTransition("VIOLIN_#3_04" , "VIOLIN_#3_03" , 0.4 );
    newMachineViolin03->addTransition("VIOLIN_#3_04" , "VIOLIN_#3_04" , 0.2 );
    
    //machine Violonchelo
    newMachineViolonchelo->addState( "VIOLONCHELO_SILENCE" , "sounds/soundsJo_03/Luz_Violonchelo_Part_Silence.wav" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_01" , "sounds/soundsJo_03/Luz_Violonchelo_Part_01.mp3" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_02" , "sounds/soundsJo_03/Luz_Violonchelo_Part_02.mp3" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_03" , "sounds/soundsJo_03/Luz_Violonchelo_Part_03.mp3" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_04" , "sounds/soundsJo_03/Luz_Violonchelo_Part_04.mp3" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_05" , "sounds/soundsJo_03/Luz_Violonchelo_Part_05.mp3" , 0 );
    newMachineViolonchelo->addState( "VIOLONCHELO_06" , "sounds/soundsJo_03/Luz_Violonchelo_Part_06.mp3" , 0 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_SILENCE" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_01" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_04" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_SILENCE" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_SILENCE" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_01" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_04" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_01" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_SILENCE" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_01" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_04" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_02" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_SILENCE" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_01" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_02" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_04" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_03" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_SILENCE" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_01" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_04" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_04" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_SILENCE" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_01" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_04" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_05" , "VIOLONCHELO_06" , 0.1 );
    
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_SILENCE" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_01" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_02" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_03" , 0.2 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_04" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_05" , 0.1 );
    newMachineViolonchelo->addTransition("VIOLONCHELO_06" , "VIOLONCHELO_06" , 0.1 );

    newMachinePiano->save( "machines/MachinesJo_02/MachinePiano.mmf");
    newMachineViola->save( "machines/MachinesJo_02/MachineViola.mmf");
    newMachineViolin01->save( "machines/MachinesJo_02/MachineViolin01.mmf");
    newMachineViolin02->save( "machines/MachinesJo_02/MachineViolin02.mmf");
    newMachineViolin03->save( "machines/MachinesJo_02/MachineViolin03.mmf");
    newMachineViolonchelo->save( "machines/MachinesJo_02/MachineViolonchelo.mmf");

    machines.push_back( newMachinePiano         );
    machines.push_back( newMachineViola         );
    machines.push_back( newMachineViolin01      );
    machines.push_back( newMachineViolin02      );
    machines.push_back( newMachineViolin03      );
    machines.push_back( newMachineViolonchelo   );
}

//--------------------------------------------------------------
void ofApp::createMachinesTest(){
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
