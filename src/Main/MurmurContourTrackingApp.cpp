/*
 *  MurmurContourTrackingApp.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 19/06/15.
 *
 */

#include "AppManager.h"

#include "MurmurContourTrackingApp.h"

//--------------------------------------------------------------
void MurmurContourTrackingApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void MurmurContourTrackingApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void MurmurContourTrackingApp::draw(){
    AppManager::getInstance().draw();
}

void MurmurContourTrackingApp::exit()
{
    ofLogNotice() <<"MurmurContourTrackingApp::exit";

}


