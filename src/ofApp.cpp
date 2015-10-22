#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(20);
    
    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    texture.loadImage("GUI/earth.png");
    texture.rotate90(2);
    texture.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    if( 0 )
        vidGrabber.initGrabber(640, 480, true);
    
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    pointLightTime.setDiffuseColor(ofFloatColor(.85, .85, .55) );
    pointLightTime.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    // this sets the camera's distance from the object
    cam.setDistance(1200);
    
    ofSetCircleResolution(64);
    ofSetSphereResolution(24);
    
    MusicianMachine* newMachinePiano = new MusicianMachine( "PIANO" );
    MusicianMachine* newMachineViola = new MusicianMachine( "VIOLA" );
    MusicianMachine* newMachineViolin01 = new MusicianMachine( "VIOLIN 01" );
    MusicianMachine* newMachineViolin02 = new MusicianMachine( "VIOLIN 02" );
    MusicianMachine* newMachineViolin03 = new MusicianMachine( "VIOLIN 03" );
    MusicianMachine* newMachineViolonchelo = new MusicianMachine( "VIOLINONCHELO" );
    
    newMachinePiano->load("machines/MachinesJo_02/MachinePiano.mmf");
    newMachineViola->load("machines/MachinesJo_02/MachineViola.mmf");
    newMachineViolin01->load("machines/MachinesJo_02/MachineViolin01.mmf");
    newMachineViolin02->load("machines/MachinesJo_02/MachineViolin02.mmf");
    newMachineViolin03->load("machines/MachinesJo_02/MachineViolin03.mmf");
    newMachineViolonchelo->load("machines/MachinesJo_02/MachineViolonchelo.mmf");
    
    machines.push_back( newMachinePiano );
    machines.push_back( newMachineViola );
    machines.push_back( newMachineViolin01 );
    machines.push_back( newMachineViolin02 );
    machines.push_back( newMachineViolin03 );
    machines.push_back( newMachineViolonchelo );
    
    MusicianMachineController* newMAchineControllerPiano = new MusicianMachineController( newMachinePiano , "machines/MachinesJo_02/MachinePiano.mmf" );
    MusicianMachineController* newMAchineControllerViola = new MusicianMachineController( newMachineViola , "machines/MachinesJo_02/MachineViola.mmf" );
    MusicianMachineController* newMAchineControllerViolin01 = new MusicianMachineController( newMachineViolin01 , "machines/MachinesJo_02/MachineViolin01.mmf" );
    MusicianMachineController* newMAchineControllerViolin02 = new MusicianMachineController( newMachineViolin02 , "machines/MachinesJo_02/MachineViolin02.mmf" );
    MusicianMachineController* newMAchineControllerViolin03 = new MusicianMachineController( newMachineViolin03 , "machines/MachinesJo_02/MachineViolin03.mmf" );
    MusicianMachineController* newMAchineControllerViolonchelo = new MusicianMachineController( newMachineViolonchelo , "machines/MachinesJo_02/MachineViolonchelo.mmf" );
    
    machinesControllers.push_back( newMAchineControllerPiano   );
    machinesControllers.push_back( newMAchineControllerViola );
    machinesControllers.push_back( newMAchineControllerViolin01 );
    machinesControllers.push_back( newMAchineControllerViolin02 );
    machinesControllers.push_back( newMAchineControllerViolin03 );
    machinesControllers.push_back( newMAchineControllerViolonchelo );
    
    //createMachines();
    createEqualizer();
    createGUI();
    
    for( int m = 0 ; m < machines.size() ; m++){
        machines[m]->start();
    }
}
//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(127,127,127);
    updateEqualizer();
    
    if( machines[0]->justFinishidState() )
        for( int m = 0 ; m < machines.size() ; m++)
            if(machines[m] && machines[m]->isAtcive() )
                machines[m]->updateStates();
    
    for( int mc = 0 ; mc < machinesControllers.size() ; mc++)
        if(machinesControllers[mc])
            machinesControllers[mc]->update();
    
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    //ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    if( 0 )
        vidGrabber.update();
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255,255,255,255);
    ofPoint equalizerSize = ofPoint( 400 , 70 );
    cam.begin();
    
    ofEnableDepthTest();
    
    ofEnableLighting();
    //pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    pointLightTime.enable();
    
    //ofRotateX(ofRadToDeg(.5));
    //ofRotateY(ofRadToDeg(-.5));
    
    ofBackground(127);
    /*
     ofSetColor(255,0,0);
     // ofFill();
     //ofDrawBox(30);
     ofNoFill();
     //ofSetColor(0);
     ofDrawBox(30);
     
     ofPushMatrix();
     ofTranslate(0,0,20);
     ofSetColor(0,0,255);
     //ofFill();
     //ofDrawBox(5);
     ofNoFill();
     //ofSetColor(0);
     ofDrawBox(5);
     ofSetColor(255);
     ofPopMatrix();
     */
    
    //drawing the views
    for( int m = 0 ; m < machinesControllers.size() ; m++){
        if(machinesControllers[m]){
            ofPushMatrix();
            ofTranslate( (m * 310) - 820 , -500 );
            machinesControllers[m]->drawView();
            ofPopMatrix();
        }
    }
    
    material.begin();
    if(1) texture.getTextureReference().bind();
    if(0) vidGrabber.getTextureReference().bind();
    
    ofSetColor( 255 );
    ofVec3f timerPositionCenter = ofVec3f( 0 , 320 , 0 ) ;
    int centerRadius = 100;
    int satelitesRadius = centerRadius / 8;
    int satelitesDistance = 1.2 * centerRadius;
    ofDrawSphere( timerPositionCenter , centerRadius );
    
    if(1) texture.getTextureReference().unbind();
    if(0) vidGrabber.getTextureReference().unbind();
    material.end();
   
    for( int e = 0 ; e <= 8 ; e ++ ){
        ofSetColor( 20 , 60 , 10 );
        if( e == 0 )
            ofSetColor( 180 , 60 , 60 );
        float angle = e * ( ( 2 * PI ) / 8.8f ) + PI / 2;
        ofVec3f timerPositionGAuge = ofVec3f( satelitesDistance * cosf( angle ) , 0 , satelitesDistance * sinf( angle )  ) ;
        ofDrawSphere( timerPositionCenter + timerPositionGAuge , satelitesRadius );
    }
    ofSetColor( 180 , 200 , 220 );
    float angle = ofMap( machines[0]->getCurrentStatePercentaje() , 0 , 1 , 0 , 2 * PI ) + PI / 2;
    ofVec3f timerPositionGAuge = ofVec3f( satelitesDistance * cosf( angle ) , 0 , satelitesDistance * sinf( angle )  ) ;
    pointLightTime.setPosition( timerPositionCenter + 100 * timerPositionGAuge );
    ofDrawSphere( timerPositionCenter + timerPositionGAuge , satelitesRadius );
    ofSetColor(255);
    

    
       ofDisableLighting();
    
    
    //    ofFill();
    //    ofSetColor(pointLight.getDiffuseColor());
    //    pointLight.draw();
    //    ofSetColor(pointLight2.getDiffuseColor());
    //    pointLight2.draw();
    //    ofSetColor(pointLightTime.getDiffuseColor());
    //    pointLightTime.draw();
    
//    string msg = "\n\nLEFT MOUSE BUTTON DRAG:\nStart dragging INSIDE the yellow circle -> camera XY rotation .\nStart dragging OUTSIDE the yellow circle -> camera Z rotation (roll).\n\n";
//    msg += "LEFT MOUSE BUTTON DRAG + TRANSLATION KEY (" + ofToString(cam.getTranslationKey()) + ") PRESSED\n";
//    msg += "OR MIDDLE MOUSE BUTTON (if available):\n";
//    msg += "move over XY axes (truck and boom).\n\n";
//    msg += "RIGHT MOUSE BUTTON:\n";
//    msg += "move over Z axis (dolly)";
//    ofDrawBitmapString(msg, timerPositionCenter );
//    
    ofDisableDepthTest();
    
    ofFill();
    cam.end();
    
    //drawEqualizer( ( ofGetWidth() / 2 ) - ( equalizerSize.x / 2 ) , ofGetHeight() - ( ofGetHeight() / 6 )  , equalizerSize.x , equalizerSize.y );
    drawEqualizer( ( ofGetWidth() / 2 ) - ( equalizerSize.x / 2 ) , 20  , equalizerSize.x , equalizerSize.y );
    drawGUI( ofPoint( ( ofGetWidth() / 2 ) - ( equalizerSize.x / 2 ) + 20, 130 ) , 20 , 66 );
    //drawInteractionArea();
    
    drawGUIMouse();
}
//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
    ofRectangle vp = ofGetCurrentViewport();
    float r = MIN(vp.width, vp.height) * 0.5f;
    float x = vp.width * 0.5f;
    float y = vp.height * 0.5f;
    
    ofPushStyle();
    ofSetLineWidth(3);
    ofSetColor(255, 255, 0);
    ofNoFill();
    glDepthMask(false);
    ofCircle(x, y, r);
    glDepthMask(true);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::createGUI(){
    cursor.loadImage("GUI/cursor_baton.png");
}
//--------------------------------------------------------------
void ofApp::drawGUI(ofPoint theGUIFirstCirclePosition ,int theGUICircleRadius,
                    int theGUICircleSpacing){
    GUICircleRadius = theGUICircleRadius;
    GUICircleSpacing = theGUICircleSpacing;
    GUIFirstCirclePosition = theGUIFirstCirclePosition;
    ofColor colorOn = ofColor( 120 , 200 , 120 );
    ofColor colorOff = ofColor( 60 , 100 , 60 );
    ofPoint firstCirclePosition;
    for( int m = 0 ; m < machines.size() ; m ++ ){
        if( machines[m]->isAtcive() )
            ofSetColor( colorOn );
        else
            ofSetColor( colorOff );
        ofFill();
        ofCircle( GUIFirstCirclePosition.x + GUICircleSpacing * m , GUIFirstCirclePosition.y , GUICircleRadius );
        ofSetColor( colorOff );
        //ofDrawBitmapString( machines[m]->getName(),
        //                   GUIFirstCirclePosition.x + GUICircleSpacing * m - GUICircleRadius / 2 ,
        //                   GUIFirstCirclePosition.y  -  GUICircleRadius / 5 );
        
        //ofDrawBitmapString( machines[m]->getCurrentStateName(),
        //                   GUIFirstCirclePosition.x + GUICircleSpacing * m - GUICircleRadius / 2,
        //                   GUIFirstCirclePosition.y  + GUICircleRadius / 5 );
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
    cursor.draw(position, 80 , 80 );
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
    switch(key) {
        case 'C':
        case 'c':
            if(cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
            
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            bShowHelp ^=true;
            break;
    }
    
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
