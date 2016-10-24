/*
 *  OscManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 17/06/15.
 *
 */


#include "OscManager.h"
#include "SettingsManager.h"
#include "AppManager.h"


OscManager::OscManager(): Manager()
{
    //Intentionally left empty
}

OscManager::~OscManager()
{
   ofLogNotice() << "OscManager::destructor";
}


//--------------------------------------------------------------

void OscManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"OscManager::initialized" ;
    this->setupOscReceiver();
    this->setupOscSender();
    this->setupText();
}

void OscManager::setupOscReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getPortReceive();
    
    m_oscReceiver.setup(portReceive);
   
    ofLogNotice() <<"OscManager::setupOscReceiver -> listening for osc messages on port  " << portReceive;
}

void OscManager::setupOscSender()
{
    int portSend = AppManager::getInstance().getSettingsManager().getPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    
    m_oscSender.setup(host, portSend);
    
    ofLogNotice() <<"OscManager::setupOscSender -> open osc connection " << host << ":" << portSend;
}

void OscManager::setupText()
{
    ofVec3f position;
    ofVec2f positionTrackingVisual = AppManager::getInstance().getTrackingManager().getPosition();
    int width = AppManager::getInstance().getTrackingManager().getWidth();
    int fontSize = 12;
    position.x = positionTrackingVisual.x;
    position.y = positionTrackingVisual.y +  AppManager::getInstance().getTrackingManager().getWidth() + LayoutManager::MARGIN;
    int height = fontSize*3;
    
    int portSend = AppManager::getInstance().getSettingsManager().getPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    string text = ">> OSC sending -> Host: " + host + ", Port: " + ofToString(portSend);
    
    m_sendingInformation =  ofPtr<TextVisual> (new TextVisual(position, width, height));
    m_sendingInformation->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_sendingInformation->setColor(ofColor::white);
    m_sendingInformation->setLineHeight(2.5);
    
    AppManager::getInstance().getViewManager().addOverlay(m_sendingInformation);
    
    
    int porReceive = AppManager::getInstance().getSettingsManager().getPortReceive();
    text = ">> OSC receiving -> Port: " + ofToString(porReceive);
    
    position.y += (height + 3*fontSize);
    m_receivingInformation =  ofPtr<TextVisual> (new TextVisual(position, width, height));
    m_receivingInformation->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_receivingInformation->setColor(ofColor::white);
    m_receivingInformation->setLineHeight(2.5);
    
    AppManager::getInstance().getViewManager().addOverlay(m_receivingInformation);
    
}


void OscManager::update()
{
    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(&m);
        
        m_latestOscMessage = m;
        this->updateReceiveText();
        
        if(m.getAddress() == "/MurmurContourTracking/NearClipping"){
            int nearClipping = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiNearClipping(nearClipping);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/FarClipping"){
            int farClipping = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiFarClipping(farClipping);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/AudioVolume"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setAudioVolume(value);
            this->updateReceiveText();
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/Threslhold"){
            int threshold = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiThreshold(threshold);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/BackgroundThreslhold"){
            int backgroundThreslhold = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiBackgroundThreshold(backgroundThreslhold);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/MinArea"){
            int minArea = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiMinArea(minArea);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/MaxArea"){
            int maxArea = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiMaxArea(maxArea);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/BackgroundSubstraction"){
            bool backgroundSubstraction = ( m.getArgAsInt32(0) != 0);
            AppManager::getInstance().getGuiManager().setBackgroundSubstraction(backgroundSubstraction);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/SendAllContours"){
            bool value = ( m.getArgAsInt32(0) != 0);
            AppManager::getInstance().getGuiManager().setSendAllContours(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/ResetBackground"){
            AppManager::getInstance().getTrackingManager().onResetBackground();
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/NearClipping"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiNearClipping(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/FarClipping"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setGuiFarClipping(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/BlurScale"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setGuiBlurScale(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/BlurRotation"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setGuiBlurRotation(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/SimplifyContour"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setGuiSimplifyContour(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/SmoothingSize"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setGuiSmoothingSize(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/SmoothingShape"){
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setGuiSmoothingShape(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/CropBottom"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setCropBottom(value);
        }

        else if(m.getAddress() == "/MurmurContourTracking/CropLeft"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setCropLeft(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/CropRight"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setCropRight(value);
        }
        
        else if(m.getAddress() == "/MurmurContourTracking/CropTop"){
            int value = m.getArgAsInt32(0);
            AppManager::getInstance().getGuiManager().setCropTop(value);
        }

    }
}

void OscManager::draw()
{
    m_sendingInformation->draw();
    m_receivingInformation->draw();
}

void OscManager::sendNumberContours(int num)
{
    ofxOscMessage m;
    m.setAddress("/MurmurRenderer/NumContours");
    m.addIntArg(num);
    m_oscSender.sendMessage(m);
    m_latestOscMessage = m;
    this->updateSendText();
}

void OscManager::sendContour(const ofPolyline& contour, int i)
{
    string contourAddr = "/MurmurRenderer/Contour/" + ofToString(i);
    ofxOscMessage m;
    m.setAddress(contourAddr);
    
    for (ofPoint blobPoint : contour.getVertices()) {
        m.addFloatArg(blobPoint.x / TrackingManager::DEPTH_CAMERA_WIDTH);
        m.addFloatArg(blobPoint.y / TrackingManager::DEPTH_CAMERA_HEIGHT);
    }
    
    m_oscSender.sendMessage(m);
    //m_latestOscMessage = m;
    //this->updateSendText();
}

void OscManager::sendAudioMax(float value)
{
    ofxOscMessage m;
    m.setAddress("/MurmurRenderer/AudioMax");
    m.addFloatArg(value);
    m_oscSender.sendMessage(m);
    m_latestOscMessage = m;
    this->updateSendText();
}

void OscManager::updateSendText()
{
    int portSend = AppManager::getInstance().getSettingsManager().getPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    string text = ">> OSC sending -> Host: " + host + ", Port: " + ofToString(portSend);
    
    text += ("\n   " + this->getMessageAsString(m_latestOscMessage));
    m_sendingInformation->setText(text);
}

void OscManager::updateReceiveText()
{
    int porReceive = AppManager::getInstance().getSettingsManager().getPortReceive();
    string text = ">> OSC receiving -> Port: " + ofToString(porReceive);
    
    text += ("\n   " + this->getMessageAsString(m_latestOscMessage));
    m_receivingInformation->setText(text);
}


string OscManager::getMessageAsString(const ofxOscMessage& m) const
{
    string msg_string;
    msg_string = m.getAddress();
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " ";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    
    return msg_string;
}







