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
#include "ofxBlur.h"

#define KINECT_CAMERA //Comment if you are using the laptop camera


//========================== class TrackingManager ==============================
//============================================================================
/** \class TrackingManager TrackingManager.cpp
 *	\brief Class managing Murmur«s floor tracking
 *	\details It reads the IR sensor of the Kinect camera and sends the blob tracking information
 */

class TrackingManager: public Manager
{
    
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
    
    //! Return the tracking visual width
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
    
    //! Background Subtraction toggle change controlled by GUI
    void onBackgroundSubstractionChange(bool & value);
    
    //! Send all contours toggle change controlled by GUI
    void onSendAllContoursChange(bool & value);
    
    //! Blur Scale change controlled by GUI
    void onBlurScaleChange(float & value);
    
    //! Blur Rotation change controlled by GUI
    void onBlurRotationChange(float & value);
    
    //! Simplify contour controlled by GUI
    void onSimplifyChange(float & value);
    
    //! Smoothing size controlled by GUI
    void onSmoothingSizeChange(float & value);
    
    //! Smoothing shape controlled by GUI
    void onSmoothingShapeChange(float & value);

    //! Reset Backround for background substraction
    void onResetBackground();
    
    void onCropLeft( int & pixels) {m_cropLeft = pixels;}
    
    void onCropRight( int & pixels) {m_cropRight = pixels;}
    
    void onCropTop( int & pixels) {m_cropTop = pixels;}
    
    void onCropBottom( int & pixels){m_cropBottom = pixels;}
    
public:
    
    static const int DEPTH_CAMERA_WIDTH;
    static const int DEPTH_CAMERA_HEIGHT;
   
private:
    
    void setupCamera();
    
    void setupKinectCamera();
    
    void setupWebCamera();
    
    void setupFbos();
    
    void setupContourTracking();
    
    void updateCamera();
    
    void updateWebCamera();
    
    void updateKinectCamera();
    
    void updateContourTracking();
    
    void updateTrackedContour();
    
    void sendTrackedContour();
    
    void sendAllContours();
    
    void drawTracking();
    
    void drawTrackedContour();
    
    void drawAllContours();
    
    void drawCamera();
    
    void drawContourTracking();
    
private:
    
    
    ofShader                m_depthShader;                 ///< Shader class handling the depth camera capture
    static const string     m_depthFragmentShader;         ///< Fragment shader handling the depth camera capture
    
    ofxMultiKinectV2        m_kinect;                      ///< Mircrosoft Kinect v2 class
    ofVideoGrabber          m_vidGrabber;                  ///< Video Grabber
    ofTexture               m_depthTexture;                ///< The texture holding every new depth captured frame
    ofFbo                   m_depthFbo;                    ///< The fbo holding the depth frame after applying shader
    ofFbo                   m_blurredFbo;                  ///< The fbo holding the depth after being blurred
    int                     m_depthNearClipping;           ///< Near cliping of the depth camera (in mm)
    int                     m_depthFarClipping;            ///< Far cliping of the depth camera (in mm)
    
    ofxBlur                 m_blur;                        ///< Blur filter to reduce pixel noise
    float                   m_blurScale;                   ///< Scale corresponds to how much you "stretch" the blur kernel
    float                   m_blurRotation;                ///< Rotation corresponds to the two directions the blur
    
    ofxCv::ContourFinder        m_contourFinder;            ///< threshold used for the contour tracking
    ofxCv::RunningBackground    m_background;               ///< used for background substraction
    ofPolyline                  m_trackedContour;           ///< single contour to be tracked
    int                         m_threshold;                ///< threshold used for the contour tracking
    int                         m_thresholdBackground;      ///< threshold used for the backround substraction
    float                       m_simplifyTolerance;        ///< tolerance for simplifying the contour, removing un-necessary vertices.
    float                       m_smoothingSize;            ///< size of the smoothing window
    float                       m_smoothingShape;           ///< describes whether to use a triangular window (0) or box window (1) or something in between (0.5)
    int                         m_contourMinArea;           ///< contour minimum area
    int                         m_contourMaxArea;           ///< blcontourob's maxmimum area
    bool                        m_substractBackground;      ///< defines whether to extract or not the background
    bool                        m_sendAllContours;          ///< defines whether to send one or all contours
    
    int                         m_cropLeft, m_cropRight, m_cropTop, m_cropBottom;
    
};

//==========================================================================


