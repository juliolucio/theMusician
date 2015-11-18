#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(20);
    ofSetSphereResolution(24);
    
    //Textures
    //ofDisableArbTex();
    //texture.loadImage("GUI/earth.png");
    //texture.mirror(1,0);
    //texture.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    //lights
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    pointLightTime.setDiffuseColor(ofFloatColor(.85, .85, .55) );
    pointLightTime.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    //material
    material.setShininess( 120 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    //Camera
    camera = new ofEasyCam();
    camera->setDistance(1200);
    
    //gui
    createEqualizer();
    cursor.loadImage("GUI/cursor_baton.png");
    
    //computer vision
    //vidGrabber.initGrabber(640, 480, true);
    //img = new ofImage();
    //img->allocate( vidGrabber.getWidth() , vidGrabber.getHeight(), OF_IMAGE_COLOR );
    //finder.start();
    
    volume = .6;
}
//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(127,127,127);
    updateEqualizer();
    
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    int distanceToMiddleY = ofGetMouseY() - ( ofGetHeight() / 2);
    if( abs(distanceToMiddleY) > ( ofGetHeight() / 3)){
        directorEnergy -= ( 0.00002 * distanceToMiddleY );
        if( directorEnergy > 1 )
            directorEnergy = 1;
        if( directorEnergy < 0 )
            directorEnergy = 0;
    }

    composition.update( directorEnergy , volume );
    
   /* vidGrabber.update();
    if( vidGrabber.isFrameNew() ){
        unsigned char* pixels = 0;
        pixels = vidGrabber.getPixels();
        if( pixels ){
            img->setFromPixels( pixels , vidGrabber.getWidth() , vidGrabber.getHeight() , OF_IMAGE_COLOR );
            img->update();
            finder.setImage( img );
        }
    }*/
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255,255,255,255);
    ofBackground(127);
    
    camera->begin();
    
    ofEnableDepthTest();
    
    ofEnableLighting();
    
    material.begin();
    
    //pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    pointLightTime.enable();
    
    drawTimer( ofVec3f(0 , 320 , 0));
    
    composition.draw();
    
    drawEnergy( ofVec3f( -700 , 300 , 0 ) , 400 , 20 );
    drawVolumen( ofVec3f( -800 , 300 , 0 ) , 400 , 10 );
    
    material.end();
    
    ofDisableLighting();
    
    drawInstructions();
    
    ofDisableDepthTest();
    
    ofFill();
    
    camera->end();
    
    //drawing equalizer
    ofPoint equalizerSize = ofPoint( 400 , 70 );
    drawEqualizer( ( ofGetWidth() / 2 ) - ( equalizerSize.x / 2 ) , 20  , equalizerSize.x , equalizerSize.y );
    
    composition.drawGUI( ofVec3f(600 , 55 , 0 ) , 10 , 45 ) ;
    
    //finder.draw();
    
    //drawing mouse
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
void ofApp::drawTimer( ofVec3f position){
    ofSetColor( 255 );
    ofVec3f timerPositionCenter = ofVec3f( position );
    int centerRadius = 40;
    int satelitesRadiusBig = 15;
    int satelitesRadiusSmall = 5;
    int satelitesDistance = 400;
    int numBigSteps = 8;
    int numSmallSteps = 32;
    
    ofFill();
    ofDrawSphere( timerPositionCenter , centerRadius );
    
    for( int e = 0 ; e < numBigSteps  ; e ++ ){
        ofSetColor( 127 , 127 , 255 );
        float angle = e * ( ( 2 * PI ) / float(numBigSteps) );
        ofVec3f timerPositionGAuge = ofVec3f( satelitesDistance * cosf( angle ) , 0 , satelitesDistance * sinf( angle )  ) ;
        ofDrawSphere( timerPositionCenter + timerPositionGAuge , satelitesRadiusBig );
    }
    
    for( int e = 0 ; e < numSmallSteps ; e ++ ){
        ofSetColor( 100 , 100 , 200 );
        float angle = e * ( ( 2 * PI ) / float(numSmallSteps) );
        ofVec3f timerPositionGAuge = ofVec3f( satelitesDistance * cosf( angle ) , 0 , satelitesDistance * sinf( angle )  ) ;
        ofDrawSphere( timerPositionCenter + timerPositionGAuge , satelitesRadiusSmall );
    }

    ofSetColor( 255 , 230 , 255 );
    float angle = ofMap( composition.getPosition() , 0 , 1 , 0 , 2 * PI ) + PI / 2;
    ofVec3f timerPositionGAuge = ofVec3f( satelitesDistance * cosf( angle ) , 0 , satelitesDistance * sinf( angle )  ) ;
    ofDrawSphere( timerPositionCenter + timerPositionGAuge , satelitesRadiusSmall+ 5 );
    pointLightTime.setPosition( timerPositionCenter + 100 * timerPositionGAuge );
    ofSetColor(255);
}
//--------------------------------------------------------------
void ofApp::drawEnergy( ofVec3f position ,  int height  , int radius ){
    ofSetColor( 255 );
    ofVec3f timerPositionMax = ofVec3f( position );
    ofVec3f timerPositionMin = ofVec3f( position ) - ofVec3f( 0 , height , 0);
    int centerRadius = radius;
    ofNoFill();
    ofSetColor( 200 , 50 , 50 );
    ofDrawSphere( timerPositionMax , 1.3 * centerRadius );
    ofSetColor( 50 , 50 , 200 );
    ofDrawSphere( timerPositionMin , 1.3 * centerRadius );
    ofFill();
    ofSetColor( ofMap( directorEnergy , 0 , 1 , 0 , 255 ) , 50 , ofMap(directorEnergy , 0 , 1 , 255 , 0 ) );
    ofVec3f timerPositionMid = ofVec3f( timerPositionMin ) + ofVec3f(  0,directorEnergy * height,0);
    ofDrawSphere( timerPositionMid , centerRadius );
    ofLine( timerPositionMin , timerPositionMax);
    string msg = "ENERGUY  = ";
    msg += ofToString( directorEnergy );
    ofDrawBitmapString( msg, timerPositionMid );
    ofSetColor( 255 );
}

//--------------------------------------------------------------
void ofApp::drawVolumen( ofVec3f position ,  int height  , int radius ){
    ofSetColor( 255 );
    ofVec3f timerPositionMax = ofVec3f( position );
    ofVec3f timerPositionMin = ofVec3f( position ) - ofVec3f( 0 , height , 0);
    int centerRadius = radius;
    ofNoFill();
    ofSetColor( 200 , 50 , 50 );
    ofDrawSphere( timerPositionMax , 1.3 * centerRadius );
    ofSetColor( 50 , 200 , 20 );
    ofDrawSphere( timerPositionMin , 1.3 * centerRadius );
    ofFill();
    ofSetColor( ofMap( volume , 0 , 1 , 0 , 255 ) , ofMap(volume , 0 , 1 , 255 , 0 ) , 50 );
    ofVec3f timerPositionMid = ofVec3f( timerPositionMin ) + ofVec3f(  0,volume * height,0);
    ofDrawSphere( timerPositionMid , centerRadius );
    ofLine( timerPositionMin , timerPositionMax);
    string msg = "Vol  = ";
    msg += ofToString( volume );
    ofDrawBitmapString( msg, timerPositionMid );
    ofSetColor( 255 );
}
//--------------------------------------------------------------
void ofApp::drawInstructions(){
    //    string msg = "\n\nLEFT MOUSE BUTTON DRAG:\nStart dragging INSIDE the yellow circle -> camera XY rotation .\nStart dragging OUTSIDE the yellow circle -> camera Z rotation (roll).\n\n";
    //    msg += "LEFT MOUSE BUTTON DRAG + TRANSLATION KEY (" + ofToString(cam.getTranslationKey()) + ") PRESSED\n";
    //    msg += "OR MIDDLE MOUSE BUTTON (if available):\n";
    //    msg += "move over XY axes (truck and boom).\n\n";
    //    msg += "RIGHT MOUSE BUTTON:\n";
    //    msg += "move over Z axis (dolly)";
    //    ofDrawBitmapString(msg, timerPositionCenter );
    //
}
//--------------------------------------------------------------
void ofApp::drawEqualizer( int x , int y , int w , int h ){
    int width = w / nBandsToGet;
    for (int i = 0;i < nBandsToGet; i++){
        ofSetColor(ofMap(fftSmoothed[i] , 0 , 1 , 0 , 255 ),ofMap(fftSmoothed[i] , 0 , 1 , 255 , 0 ),0,255);
        ofFill();
        ofRect( x + i * width , y + h , width ,-(fftSmoothed[i] * h * .5 ));
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
            if(camera->getMouseInputEnabled())
                camera->disableMouseInput();
            else
                camera->enableMouseInput();
            break;
            
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
            
        case 'H':
        case 'h':
            bShowHelp ^=true;
            break;
            
        case '+':
        case '=':
            volume += 0.05;
            if( volume > 2 )
                volume = 2;
            break;
            
        case '-':
        case '_':
            volume -= .05;
            if( volume < 0 )
                volume = 0;
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
    composition.mouseReleased(x,y, button);
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}