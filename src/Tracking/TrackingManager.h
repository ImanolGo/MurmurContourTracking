/*
 *  TrackingManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#pragma once

#include "Manager.h"

#include "ofxMultiKinectV2.h"
#include "ofxCv.h"


//========================== class TrackingManager ==============================
//============================================================================
/** \class TrackingManager TrackingManager.cpp
 *	\brief Class managing Murmur�s floor tracking
 *	\details It reads the IR sensor of the Kinect camera and sends the blob tracking information
 */

class TrackingManager: public Manager
{
    
    static const int DEPTH_CAMERA_WIDTH;
    static const int DEPTH_CAMERA_HEIGHT;
    static const float SCALE;
    static const int TRACKING_PERSISTANCY;
    static const int LEARNING_TIME;
    
public:
    
    //! Constructor
    TrackingManager();
    
    //! Destructor
    ~TrackingManager();
    
    //! Set-up the kinect camera tracking
    void setup();
    
    //! Update the kinect camera tracking
    void update();
    
    //! Draw kinect camera tracking
    void draw();
    
    //! Return the tracking visual height
    int getHeight() const;
    
    //! Return the tracking visual hewidthight
    int getWidth() const;
    
    //! Return the tracking visual position
    ofVec2f getPosition() const;

    
    //! Near clipping change controlled by GUI
    void onNearClippingChange(int & value);
    
    //! Far clipping change controlled by GUI
    void onFarClippingChange(int & value);
    
    //! Threshold change controlled by GUI
    void onThresholdChange(int & value);
    
    //! Background Threshold change controlled by GUI
    void onBackgroundThresholdChange(int & value);
    
    //! Minimum area change controlled by GUI
    void onMinAreaChange(int & value);
    
    //! Maximum area change controlled by GUI
    void onMaxAreaChange(int & value);
    
    //! Background Subtraction togle change controlled by GUI
    void onBackgroundSubstractionChange(bool & value);
    
    //! Reset Backround for background substraction
    void onResetBackground();
   
private:
    
    void setupKinectCamera();
    
    void setupContourTracking();
    
    void updateKinectCamera();
    
    void updateContourTracking();
    
    void drawTracking();
    
    void drawIrCamera();
    
    void drawContourTracking();
    
private:
    
    
    ofShader                m_depthShader;                 ///< Shader class handling the depth camera capture
    static const string     m_depthFragmentShader;         ///< Fragment shader handling the depth camera capture
    
    ofxMultiKinectV2        m_kinect;                      ///< Mircrosoft Kinect v2 class
    ofTexture               m_depthTexture;                ///< The texture holding every new depth captured frame
    ofFbo                   m_depthFbo;                    ///< The fbo holding the depth frame after applying shader
    int                     m_depthNearClipping;           ///< Near cliping of the depth camera (in mm)
    int                     m_depthFarClipping;            ///< Far cliping of the depth camera (in mm)
    
    ofxCv::ContourFinder        m_contourFinder;            ///< threshold used for the contour tracking
    ofxCv::RunningBackground    m_background;               ///< used for background substraction
    int                         m_threshold;                ///< threshold used for the contour tracking
    int                         m_thresholdBackground;      ///< threshold used for the backround substraction
    int                         m_contourMinArea;           ///< contour minimum area
    int                         m_contourMaxArea;           ///< blcontourob's maxmimum area
    bool                        m_substractBackground;      ///< defines whether to extract or not the background
    
};

//==========================================================================


