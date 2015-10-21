//
//  MusicianMachineController.cpp
//  theMusiciam
//
//  Created by Julio Lucio on 9/27/15.
//
//
#include "MusicianMachineController.h"
#include "MusicianMachine.h"
#include "MusicianMachineView.h"

//-------------------------------------------------------------
MusicianMachineController::MusicianMachineController( MusicianMachine* theMachineReference , string fileName ){
    machineReference = theMachineReference;
    machineView = new MusicianMachineView( theMachineReference->getName() );
    machineView->load(fileName);
}
//-------------------------------------------------------------
MusicianMachineController::~MusicianMachineController(){
    clear();
}
//-------------------------------------------------------------
void MusicianMachineController::update(){
    machineView->setActive(machineReference->isAtcive());
    if( machineReference->justChangedState())
        machineView->setCurrentState(machineReference->getCurrentStateName());
}
//-------------------------------------------------------------
void MusicianMachineController::drawView(){
    machineView->draw();
}
//-------------------------------------------------------------
void MusicianMachineController::clear(){
    delete machineView;
}