/*
 *  GuiManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxGui.h"
//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application«s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth()  {return m_gui.getWidth();}
    
    int getHeight()  {return m_gui.getHeight();}
    
    void setAudioVolume(float& value){m_audioVolume = value;}
    
    void setGuiNearClipping(int value) {m_nearClipping = value;}
    
    void setGuiFarClipping(int value) {m_farClipping = value;}
    
    void setGuiBlurScale(float value) {m_blurScale = value;}
    
    void setGuiBlurRotation(float value) {m_blurRotation = value;}
    
    void setGuiSmoothingSize(float value) {m_smoothingSize = value;}
    
    void setGuiSmoothingShape(float value) {m_smoothingShape = value;}
    
    void setGuiThreshold(int value) {m_threshold = value;}
    
    void setGuiBackgroundThreshold(int value) {m_backgroundThreshold = value;}
    
    void setGuiMinArea(int value) {m_minArea = value;}
    
    void setGuiMaxArea(int value) {m_maxArea = value;}
    
    void setGuiSimplifyContour(float value) {m_simplifyContour = value;}
    
    void setBackgroundSubstraction(bool value) {m_backgroundSubstraction = value;}
    
    void setSendAllContours(bool value) {m_sendAllContours = value;}
    
    void setCropBottom(int value) {m_cropBottom = value;}
    
    void setCropLeft(int value) {m_cropLeft = value;}
    
    void setCropRight(int value) {m_cropRight = value;}
    
    void setCropTop(int value) {m_cropTop = value;}
    
private:
    
    void setupCameraGui();
    
    void setupTrackingGui();
    
    void setupAudioGui();
    
public:
    
    static const int GUI_WIDTH;
    
private:
    
    // Fluid GUI
    ofxPanel            m_gui;
    ofParameterGroup    m_parametersCamera;
    ofParameterGroup    m_parametersTracking;
    ofParameterGroup    m_parametersAudio;
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
    
    ofParameter<float>	 m_guiFPS;
    ofParameter<int>	 m_nearClipping;
    ofParameter<int>	 m_farClipping;
    ofParameter<int>	 m_threshold;
    ofParameter<int>	 m_backgroundThreshold;
    ofParameter<int>	 m_minArea;
    ofParameter<int>	 m_maxArea;
    ofParameter<float>	 m_blurScale;
    ofParameter<float>	 m_blurRotation;
    ofParameter<float>   m_simplifyContour;
    ofParameter<float>   m_smoothingSize;
    ofParameter<float>   m_smoothingShape;
    ofParameter<bool>	 m_backgroundSubstraction;
    ofParameter<bool>	 m_sendAllContours;
    
    ofParameter<float>   m_audioVolume;
    ofParameter<int>     m_audioNumPeaks;
    
    ofParameter<int>     m_cropLeft, m_cropRight, m_cropTop, m_cropBottom;
    
};

//==========================================================================


