/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include <juce_dsp/juce_dsp.h>

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setLowGain(double lowGain);
    void setMidGain(double midGain);
    void setHighGain(double highGain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
	void toggleEQ();
    

    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};


    //state variable filter
    juce::dsp::StateVariableTPTFilter<float> lowPassFilter;
    juce::dsp::StateVariableTPTFilter<float> midBandFilter;
    juce::dsp::StateVariableTPTFilter<float> highPassFilter;

    float lowGain = 1.0;
    float midGain = 1.0;
    float highGain = 1.0;

	bool onOffEQ = false;

};




