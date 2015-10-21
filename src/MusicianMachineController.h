//
//  MusicianMachineController.h
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//

#ifndef MusicianMachineController_h
#define MusicianMachineController_h

#include <stdio.h>
#include "ofMain.h"

class MusicianMachine;
class MusicianMachineView;

class MusicianMachineController{
private:
    MusicianMachine* machineReference;
    MusicianMachineView* machineView;
    ifstream* fileIn;
    void clear();
    
public:
    MusicianMachineController( MusicianMachine* theMachineReference , string fileName);
    ~MusicianMachineController();
    void update();
    void drawView();


    

};

#endif 
