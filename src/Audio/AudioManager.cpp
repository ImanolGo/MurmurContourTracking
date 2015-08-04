/*
 *  AudioManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/08/15.
 *
 */


#include "AppManager.h"
#include "AudioManager.h"


AudioManager::AudioManager(): m_volumeRange(100)
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
    m_fft.setNumFFTBins(8);
    m_fft.setVolumeRange(m_volumeRange);
    //m_fft.setNormalize(true);
   
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
    m_fft.drawBars();
}

void AudioManager::onChangeVolumeRange(float& value)
{
    m_volumeRange = ofMap(value, 0, 1, 2000, 1);
    m_fft.setVolumeRange(m_volumeRange);

}

float AudioManager::getMaxSound() 
{
    return ofMap(m_fft.getUnScaledLoudestValue(), 0.0, m_volumeRange, 0.0, 1.0, true);
}

