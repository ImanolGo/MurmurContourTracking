/*
 *  AudioManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/08/15.
 *
 */


#include "AppManager.h"
#include "AudioManager.h"


AudioManager::AudioManager(): m_volume(0.5)
{
    //Intentionaly left empty
}


AudioManager::~AudioManager()
{
    ofLogNotice() <<"AudioManager::Destructor" ;
}


void AudioManager::setup()
{
    Manager::setup();
    
    ofLogNotice() <<"AudioManager::initialized" ;
    
    this->setupFFT();
}


void AudioManager::setupFFT()
{
    m_fft.setup();
    m_fft.setMirrorData(false);
    m_fft.setPeakDecay(0.915);
    m_fft.setMaxDecay(0.995);
    m_fft.setThreshold(1.0);
    m_fft.setVolume(m_volume);
   
}


void AudioManager::update()
{
    m_fft.update();
    
    AppManager::getInstance().getOscManager().sendAudioMax(getMaxSound());
    //ofLogNotice() <<"AudioManager::update: " << m_fft.getUnScaledLoudestValue();
    //ofLogNotice() <<"AudioManager::update2: " << m_fft.getLoudBand();
}

void AudioManager::draw()
{
    m_fft.draw(1040,370);
}

void AudioManager::onChangeVolume(float& value)
{
    m_volume = value;
    m_fft.setVolume(m_volume);
}

float AudioManager::getMaxSound() 
{
    return ofMap(m_fft.getAveragePeak(), 0.0, 0.6, 0.0, 1.0, true);
}

