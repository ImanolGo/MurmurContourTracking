/*
 *  TrackingManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */

#include "AppManager.h"

#include "TrackingManager.h"

using namespace ofxCv;
using namespace cv;

#define STRINGIFY(x) #x


const string TrackingManager::m_depthFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          uniform float nearClipping;
          uniform float farClipping;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r;
              float low1 = nearClipping;
              float high1 = farClipping;
              float low2 = 1.0;
              float high2 = 0.0;
              float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
              if (d == 1.0) {
                  d = 0.0;
              }
              gl_FragColor = vec4(vec3(d), 1.0);
          }
);

const int TrackingManager::DEPTH_CAMERA_WIDTH = 512;
const int TrackingManager::DEPTH_CAMERA_HEIGHT = 424;
const float TrackingManager::SCALE = 1.35;
const int TrackingManager::TRACKING_PERSISTANCY = 5*30;
const int TrackingManager::LEARNING_TIME = 10*30;


TrackingManager::TrackingManager(): Manager(), m_threshold(80), m_contourMinArea(50), m_contourMaxArea(1000), m_thresholdBackground(10), m_substractBackground(true),
m_depthNearClipping(0.0), m_depthFarClipping(5000.0), m_blurScale(0.0), m_blurRotation(0.0), m_simplifyTolerance(0.0), m_smoothingShape(0.0),m_smoothingSize(0.0),
m_sendAllContours(false),m_cropLeft(0), m_cropRight(0), m_cropTop(0), m_cropBottom(0)
{
    //Intentionally left empty
}


TrackingManager::~TrackingManager()
{
    ofLogNotice() <<"TrackingManager::Destructor";
}


void TrackingManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"TrackingManager::initialized";
    
    Manager::setup();
    
    this->setupCamera();
    this->setupFbos();
    this->setupContourTracking();
}

void TrackingManager::setupContourTracking()
{
    m_contourFinder.setMinAreaRadius(m_contourMinArea);
    m_contourFinder.setMaxAreaRadius(m_contourMaxArea);
    m_contourFinder.setTargetColor(ofColor::white, TRACK_COLOR_RGB);
    m_contourFinder.setThreshold(m_threshold);
    m_contourFinder.getTracker().setPersistence(TRACKING_PERSISTANCY);
    m_contourFinder.setFindHoles(true);
    //m_contourFinder.getTracker().setMaximumDistance(32);
    
    m_background.setLearningTime(LEARNING_TIME);
    m_background.setThresholdValue(m_threshold);
    m_background.reset();
}

void TrackingManager::setupCamera()
{
    #ifdef KINECT_CAMERA
        this->setupKinectCamera();
    #else
        this->setupWebCamera();
    #endif
}

void TrackingManager::setupFbos()
{
    m_depthFbo.allocate(DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT, GL_RGB);
    m_depthFbo.begin();
        ofClear(0,0,0,0);
    m_depthFbo.end();
    
    m_blurredFbo.allocate(DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT, GL_RGB);
    m_blurredFbo.begin();
        ofClear(0,0,0,0);
    m_blurredFbo.end();
    
    float radius = 4; float shape = .2; float passes = 1;
    m_blur.setup(DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT, radius, shape, passes);

}

void TrackingManager::setupKinectCamera()
{
    m_depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, m_depthFragmentShader);
    m_depthShader.linkProgram();
    
    m_kinect.open(true, true, 0, 2);
    m_kinect.start();
    
    // Note :
    // Default OpenCL device might not be optimal.
    // e.g. Intel HD Graphics will be chosen instead of GeForce.
    // To avoid it, specify OpenCL device index manually like following.
    // m_kinect.open(true, true, 0, 1); // GeForce on MacBookPro Retina
}

void TrackingManager::setupWebCamera()
{
    m_vidGrabber.setDeviceID(0);
    //m_vidGrabber.setDesiredFrameRate(60);
    m_vidGrabber.initGrabber(DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT);
}


void TrackingManager::update()
{
    this->updateCamera();
    this->updateContourTracking();
}

void TrackingManager::updateCamera()
{
    #ifdef KINECT_CAMERA
        this->updateKinectCamera();
    #else
        this->updateWebCamera();
    #endif
}

void TrackingManager::updateKinectCamera()
{
    m_kinect.update();
    if (m_kinect.isFrameNew()) {
        m_depthTexture.loadData(m_kinect.getDepthPixelsRef());
    
        if (m_depthTexture.isAllocated()) {
            m_depthFbo.begin();
            m_depthShader.begin();
            m_depthShader.setUniform1f("nearClipping", m_depthNearClipping);
            m_depthShader.setUniform1f("farClipping", m_depthFarClipping);
            m_depthTexture.draw(0, 0, DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT);
            m_depthShader.end();
            
            ofPushStyle();
            ofSetColor(0);
            ofFill();
            ofRect(0,0,m_cropLeft,DEPTH_CAMERA_HEIGHT);
            ofRect(0,0,DEPTH_CAMERA_WIDTH,m_cropTop);
            ofRect(DEPTH_CAMERA_WIDTH-m_cropRight,0, m_cropRight, DEPTH_CAMERA_HEIGHT);
            ofRect(0,DEPTH_CAMERA_HEIGHT-m_cropBottom,DEPTH_CAMERA_WIDTH,m_cropBottom);
            ofPopStyle();

            
            m_depthFbo.end();
            
            m_blur.begin();
                m_depthFbo.draw(0,0);
            m_blur.end();
            
            m_blurredFbo.begin();
                m_blur.draw();
            m_blurredFbo.end();
        }
    }
}


void TrackingManager::updateWebCamera()
{
    
    m_vidGrabber.update();
    if (m_vidGrabber.isFrameNew()) {
        m_depthTexture.loadData(m_vidGrabber.getPixelsRef());
        
        if (m_depthTexture.isAllocated()) {
            m_depthFbo.begin();
            
                m_depthTexture.draw(0, 0, DEPTH_CAMERA_WIDTH, DEPTH_CAMERA_HEIGHT);
                
                ofPushStyle();
                ofSetColor(0);
                ofFill();
                ofRect(0,0,m_cropLeft,DEPTH_CAMERA_HEIGHT);
                ofRect(0,0,DEPTH_CAMERA_WIDTH,m_cropTop);
                ofRect(DEPTH_CAMERA_WIDTH-m_cropRight,0, m_cropRight, DEPTH_CAMERA_HEIGHT);
                ofRect(0,DEPTH_CAMERA_HEIGHT-m_cropBottom,DEPTH_CAMERA_WIDTH,m_cropBottom);
                ofPopStyle();
            
            m_depthFbo.end();
            
            m_blur.begin();
            m_depthFbo.draw(0,0);
            m_blur.end();
            
            m_blurredFbo.begin();
            m_blur.draw();
            m_blurredFbo.end();
        }
    }

    
}

void TrackingManager::updateContourTracking()
{
    if (m_kinect.isFrameNew() || m_vidGrabber.isFrameNew())
    {
        ofImage image;
        ofPixels pixels;
        m_blurredFbo.readToPixels(pixels);
        image.setFromPixels(pixels);
        
        if(m_substractBackground){
            ofImage thresholded;
            m_background.update(image, thresholded);
            thresholded.update();
            m_contourFinder.findContours(thresholded);
        }
        else{
            m_contourFinder.findContours(image);
        }
        
        this->updateTrackedContour();
        
        if(m_sendAllContours){
            this->sendAllContours();
        }
        else{
            this->sendTrackedContour();
        }
    }
}

void TrackingManager::updateTrackedContour()
{
    double contourArea = 0;
    m_trackedContour.clear();
    for(int i = 0; i < m_contourFinder.size(); i++) {
        
        if(contourArea < m_contourFinder.getContourArea(i)){
            contourArea =  m_contourFinder.getContourArea(i);
            m_trackedContour =  m_contourFinder.getPolyline(i);
        }
    }
}

void TrackingManager::sendTrackedContour()
{
    AppManager::getInstance().getOscManager().sendNumberContours(1);
    
    ofPolyline p = m_trackedContour.getSmoothed(m_smoothingSize, m_smoothingShape);
    p.simplify(m_simplifyTolerance);
    AppManager::getInstance().getOscManager().sendContour(p, 0);
}

void TrackingManager::sendAllContours()
{
    AppManager::getInstance().getOscManager().sendNumberContours(m_contourFinder.size());
    for(int i = 0; i < m_contourFinder.size(); i++) {
        ofPolyline p = m_contourFinder.getPolyline(i).getSmoothed(m_smoothingSize, m_smoothingShape);
        p.simplify(m_simplifyTolerance);
        AppManager::getInstance().getOscManager().sendContour(p, i);
    }
}


void TrackingManager::draw()
{
    this->drawTracking();
}


void TrackingManager::drawTracking()
{
    ofVec2f pos = this->getPosition();
    ofPushMatrix();
        ofTranslate( pos.x , pos.y );
        ofScale(SCALE,SCALE);
        this->drawCamera();
        this->drawContourTracking();
    ofPopMatrix();
}


void TrackingManager::drawCamera()
{
    ofPushStyle();
    ofSetColor(255);
        ofRect(0, 0, DEPTH_CAMERA_WIDTH + LayoutManager::PADDING*2, DEPTH_CAMERA_HEIGHT + LayoutManager::PADDING*2);
        m_blurredFbo.draw(LayoutManager::PADDING,LayoutManager::PADDING);
    ofPopStyle();
}


void TrackingManager::drawContourTracking()
{
    ofPushMatrix();
        ofTranslate( LayoutManager::PADDING , LayoutManager::PADDING);
        if(m_sendAllContours){
            this->drawAllContours();
        }
        else{
            this->drawTrackedContour();
        }
    ofPopMatrix();
}

void TrackingManager::drawTrackedContour()
{
    ofPolyline p = m_trackedContour.getSmoothed(m_smoothingSize, m_smoothingShape);
    p.simplify(m_simplifyTolerance);
    p.draw();
}

void TrackingManager::drawAllContours()
{
    for(int i = 0; i < m_contourFinder.size(); i++)
    {
        ofPolyline p = m_contourFinder.getPolyline(i).getSmoothed(m_smoothingSize, m_smoothingShape);
        p.simplify(m_simplifyTolerance);
        p.draw();
    }
}

//--------------------------------------------------------------

void TrackingManager::onResetBackground(){
    m_background.reset();
}


void TrackingManager::onNearClippingChange(int & value){
    m_depthNearClipping = ofClamp(value,0,12000);
}

void TrackingManager::onFarClippingChange(int & value){
    m_depthFarClipping = ofClamp(value,m_depthNearClipping,12000);
}

void TrackingManager::onThresholdChange(int & value){
    m_threshold = ofClamp(value,0,255);
    m_contourFinder.setThreshold(m_threshold);
}

void TrackingManager::onBackgroundThresholdChange(int & value){
    m_thresholdBackground = ofClamp(value,0,30);
    m_background.setThresholdValue(m_thresholdBackground);
}


void TrackingManager::onMinAreaChange(int & value){
    m_contourMinArea = ofClamp(value,0,100);
    m_contourFinder.setMinAreaRadius(m_contourMinArea);
}

void TrackingManager::onMaxAreaChange(int & value){
    m_contourMaxArea = ofClamp(value,0,500);
    m_contourFinder.setMaxAreaRadius(m_contourMaxArea);
}

void TrackingManager::onBackgroundSubstractionChange(bool & value)
{
    m_substractBackground = value;
}

void TrackingManager::onSendAllContoursChange(bool & value)
{
    m_sendAllContours = value;
}

void TrackingManager::onBlurScaleChange(float & value)
{
    m_blurScale = ofClamp(value,0.0,2.0);
    m_blur.setScale(m_blurScale);
}

void TrackingManager::onBlurRotationChange(float & value)
{
    m_blurRotation = ofClamp(value,-PI,PI);
    m_blur.setRotation(m_blurRotation);
}

void TrackingManager::onSimplifyChange(float & value)
{
    m_simplifyTolerance = ofClamp(value,0.0,2.0);
}

void TrackingManager::onSmoothingSizeChange(float & value)
{
    m_smoothingSize = ofClamp(value,0.0,5.0);
}

void TrackingManager::onSmoothingShapeChange(float & value)
{
    m_smoothingShape = ofClamp(value,0.0,1.0);
}

int TrackingManager::getHeight() const
{
    return (DEPTH_CAMERA_HEIGHT + LayoutManager::PADDING*2)*SCALE;
}

int TrackingManager::getWidth() const
{
    return (DEPTH_CAMERA_HEIGHT + LayoutManager::PADDING*2)*SCALE;
}

ofVec2f TrackingManager::getPosition() const
{
    ofVec2f pos;
    pos.y = LayoutManager::MARGIN;
    pos.x = GuiManager::GUI_WIDTH + 2*LayoutManager::MARGIN;
    return pos;
}




