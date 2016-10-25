/*
 *  AudioManager.cpp
 *  Murmur
 *
 *  Created by Imanol Gomez on 04/08/15.
 *
 */


#include "AppManager.h"
#include "AudioManager.h"


AudioManager::AudioManager(): m_volume(0.5), m_numPeaks(5.0)
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
    
    ofLogNotice() <<"AudioManager::setupFFT" ;
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
   
    float avrPeak = 0;
    //vector<float> peakData = m_fft.getFftPeakData();
    vector<float> peakData = m_fft.getFftRawData();
    
    
    if(peakData.size() <= m_numPeaks){
        return avrPeak;
    }
    
    for(int i=0; i<= m_numPeaks; i++)
    {
         avrPeak+=peakData[i];
    }
   
    
    avrPeak/=m_numPeaks;
    
    //float maxSound = ofMap(m_fft.getAveragePeak(), 0.0, 0.6, 0.0, 1.0, true);
    float maxSound = ofMap(avrPeak, 0.0, 0.6, 0.0, 1.0, true);
    return maxSound;
}

