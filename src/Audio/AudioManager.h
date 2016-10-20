/*
 *  AudioManager.h
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/08/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"

#include "ofxFFTLive.h"

//========================== class AudioManager ==============================
//============================================================================
/** \class AudioManager AudioManager.h
 *	\brief Class managing the audio input
 *	\details It reads from the input microphone and calculates the energy.
 */


class AudioManager: public Manager
{
    
public:
    
    //! Constructor
    AudioManager();
    
    //! Destructor
    ~AudioManager();
    
    //! Setup the Audio Manager
    void setup();
    
    //! Update the Audio Manager
    void update();
    
    //! Draw the Audio Manager
    void draw();
    
    float getMaxSound();
    
    void onChangeVolume(float& value);
    
    void onChangeNumPeaks(int& value){m_numPeaks = value;}
    
private:
    
    void setupFFT();
    
    
private:
    
    float           m_volume;
    int             m_numPeaks;
    ofxFFTLive      m_fft;
    
};


