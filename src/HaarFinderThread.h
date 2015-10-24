//
//  HaarFinderThread.h
//  theMusiciam
//
//  Created by Julio Lucio on 10/24/15.
//
//
#pragma once

#include "ofThread.h"

class HaarFinderThread: public ofThread{
private:
    //computer vision
    ofxCvHaarFinder finder;
    unsigned long long finderDelay;
    unsigned long long finderlastUpdate;
    
protected:
    ofPoint handPosition;
    ofImage* image;
    
public:
    HaarFinderThread(){
        finder.setup("cascades/HandCascade.xml");
        finderDelay = 10;
        finderlastUpdate = 0;
    }
    
    void start(){
        startThread();
    }
    
    void stop(){
        stopThread();
    }
    
    void threadedFunction(){
        while( isThreadRunning() ){
            if(lock()){
                if( image )
                    if( ofGetElapsedTimeMillis() - finderlastUpdate  > finderDelay ){
                        //finder.findHaarObjects( *image );
                        finderlastUpdate = ofGetElapsedTimeMillis();
                    }
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void draw(){
        //drawing blobs
        ofPushMatrix();
        ofScale( .2 , .2 );
        if( image )
            image->draw(100,100);
        ofNoFill();
        for(unsigned int i = 0; i < finder.blobs.size(); i++) {
            ofRectangle cur = finder.blobs[i].boundingRect;
            ofRect(cur.x, cur.y, cur.width, cur.height);
        }
        ofPopMatrix();
    }
    
    ofPoint getHandPosition(){
        ofScopedLock lock(mutex);
        for(unsigned int i = 0; i < finder.blobs.size(); i++)
            if( i == 0 ){
                handPosition = finder.blobs[i].centroid;
                return handPosition;
            }
    }
    
    ofPoint setImage( ofImage* theImage ){
        ofScopedLock lock(mutex);
        image = theImage;
    }
};
