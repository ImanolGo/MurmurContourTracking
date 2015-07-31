/*
 *  GuiManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "TrackingManager.h"
#include "LayoutManager.h"

#include "GuiManager.h"

const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "MurmurContourTracking";
const int GuiManager::GUI_WIDTH = 250;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"GuiManager::initialized";
    
    Manager::setup();
    
    m_gui.setDefaultWidth(GUI_WIDTH);
    m_gui.setup(GUI_SETTINGS_NAME, GUI_SETTINGS_FILE_NAME);
    m_gui.setPosition(LayoutManager::MARGIN, LayoutManager::MARGIN);
    m_gui.add(m_guiFPS.set("FPS", 0, 0, 60));
    ofxGuiSetFont( "fonts/open-sans/OpenSans-Semibold.ttf", 9 );
    
    this->setupCameraGui();
    this->setupTrackingGui();
    m_gui.loadFromFile(GUI_SETTINGS_FILE_NAME);
 
   
}


void GuiManager::setupCameraGui()
{
    m_parametersCamera.setName("Camera");
    
    TrackingManager* trackingManager = &AppManager::getInstance().getTrackingManager();
   
    m_nearClipping.set("NearClipping", 0, 0, 12000);
    m_nearClipping.addListener(trackingManager, &TrackingManager::onNearClippingChange);
    m_parametersCamera.add(m_nearClipping);
    
    m_farClipping.set("FarClipping", 5000, 0, 12000);
    m_farClipping.addListener(trackingManager, &TrackingManager::onFarClippingChange);
    m_parametersCamera.add(m_farClipping);
    
    m_blurScale.set("BlurScale", 0.0, 0.0, 2.0);
    m_blurScale.addListener(trackingManager, &TrackingManager::onBlurScaleChange);
    m_parametersCamera.add(m_blurScale);
    
    m_blurRotation.set("BlurRotation", 0.0, -PI, PI);
    m_blurRotation.addListener(trackingManager, &TrackingManager::onBlurRotationChange);
    m_parametersCamera.add(m_blurRotation);
    
    m_gui.add(m_parametersCamera);

}

void GuiManager::setupTrackingGui()
{
    TrackingManager* trackingManager = &AppManager::getInstance().getTrackingManager();
    
    m_parametersTracking.setName("Tracking");
    
    m_threshold.set("Threshold", 40, 0, 255);
    m_threshold.addListener(trackingManager, &TrackingManager::onThresholdChange);
    m_parametersTracking.add(m_threshold);
    
    m_backgroundThreshold.set("BackgroundThreshold", 10, 0, 30);
    m_backgroundThreshold.addListener(trackingManager, &TrackingManager::onBackgroundThresholdChange);
    m_parametersTracking.add(m_backgroundThreshold);
    
    m_minArea.set("MinArea", 20, 0, 100);
    m_minArea.addListener(trackingManager, &TrackingManager::onMinAreaChange);
    m_parametersTracking.add(m_minArea);
    
    m_maxArea.set("MaxArea", 100, 100, 500);
    m_maxArea.addListener(trackingManager, &TrackingManager::onMaxAreaChange);
    m_parametersTracking.add(m_maxArea);
    
    m_simplifyContour.set("SimplifyContour", 0.0, 0.0, 2.0);
    m_simplifyContour.addListener(trackingManager, &TrackingManager::onSimplifyChange);
    m_parametersTracking.add(m_simplifyContour);
    
    m_smoothingSize.set("SmoothingSize", 0.0, 0.0, 5.0);
    m_smoothingSize.addListener(trackingManager, &TrackingManager::onSmoothingSizeChange);
    m_parametersTracking.add(m_smoothingSize);
    
    m_smoothingShape.set("SmoothingShape", 0.0, 0.0, 1.0);
    m_smoothingShape.addListener(trackingManager, &TrackingManager::onSmoothingShapeChange);
    m_parametersTracking.add(m_smoothingShape);
    
    m_backgroundSubstraction.set("BackgroundSubstraction", true);
    m_backgroundSubstraction.addListener(trackingManager, &TrackingManager::onBackgroundSubstractionChange);
    m_parametersTracking.add(m_backgroundSubstraction);
    
    m_sendAllContours.set("SendAllContours", true);
    m_sendAllContours.addListener(trackingManager, &TrackingManager::onSendAllContoursChange);
    m_parametersTracking.add(m_sendAllContours);
    
    m_gui.add(m_parametersTracking);
    
    ofxButton * resetBackground = new ofxButton();
    resetBackground->setup("ResetBackground");
    resetBackground->addListener(trackingManager, &TrackingManager::onResetBackground);
    m_gui.add(resetBackground);
    
}

void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    m_gui.draw();
    m_guiFPS = ofGetFrameRate();
}


void GuiManager::saveGuiValues()
{
    m_gui.saveToFile(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    m_gui.loadFromFile(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::toggleGui()
{
    ofLogNotice() <<"GuiManager::toggleGui -> show GUI "<< m_showGui;
    m_showGui = !m_showGui;
}



