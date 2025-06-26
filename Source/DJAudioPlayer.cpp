/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    //code for state variable filter________________________________
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2; //stereo

    //initialize low pass filter
    lowPassFilter.prepare(spec);
    lowPassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowPassFilter.setCutoffFrequency(150.0f);
    lowPassFilter.reset();

	//initialize mid band filter
	midBandFilter.prepare(spec);
	midBandFilter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
	midBandFilter.setCutoffFrequency(1000.0f);
	midBandFilter.reset();

	//initialize high pass filter
	highPassFilter.prepare(spec);
	highPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	highPassFilter.setCutoffFrequency(4000.0f);
	highPassFilter.reset();

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    //____________________________________________________________________
	//Code for filters together__________________________________________
    juce::dsp::AudioBlock<float> audioBlock(*bufferToFill.buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    // Create separate buffers for each band
    juce::AudioBuffer<float> lowBuffer, midBuffer, highBuffer;
    lowBuffer.makeCopyOf(*bufferToFill.buffer);
    midBuffer.makeCopyOf(*bufferToFill.buffer);
    highBuffer.makeCopyOf(*bufferToFill.buffer);

    // Process low frequencies
    juce::dsp::AudioBlock<float> lowBlock(lowBuffer);
    juce::dsp::ProcessContextReplacing<float> lowContext(lowBlock);
    lowPassFilter.process(lowContext);
    lowBuffer.applyGain(lowGain);

    // Process mid frequencies
    juce::dsp::AudioBlock<float> midBlock(midBuffer);
    juce::dsp::ProcessContextReplacing<float> midContext(midBlock);
    midBandFilter.process(midContext);
    midBuffer.applyGain(midGain);


    // Process high frequencies
    juce::dsp::AudioBlock<float> highBlock(highBuffer);
    juce::dsp::ProcessContextReplacing<float> highContext(highBlock);
    highPassFilter.process(highContext);
    highBuffer.applyGain(highGain);

    if (onOffEQ)
    {
        // Clear the original buffer
        bufferToFill.buffer->clear();
        //Sum the processed buffers
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            bufferToFill.buffer->addFrom(channel, 0, lowBuffer, channel, 0, bufferToFill.numSamples);
            bufferToFill.buffer->addFrom(channel, 0, midBuffer, channel, 0, bufferToFill.numSamples);
            bufferToFill.buffer->addFrom(channel, 0, highBuffer, channel, 0, bufferToFill.numSamples);
        }
    }


    //____________________________________________________________________
 

}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();

}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, 
                                                                                            true)); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());   

		std::cout << "DJAudioPlayer::loadURL - sample rate: " << reader->sampleRate << std::endl;
		std::cout << "DJAudioPlayer::loadURL - lenght in samples: " << reader->lengthInSamples << std::endl;


    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setLowGain(double lowGain)
{
	if (lowGain < 0 || lowGain > 1.0)
	{
		std::cout << "DJAudioPlayer::setLowGain lowGain should be between 0 and 1" << std::endl;
	}
	else {
		this->lowGain = lowGain;
	}
}

void DJAudioPlayer::setMidGain(double midGain)
{
	if (midGain < 0 || midGain > 1.0)
	{
		std::cout << "DJAudioPlayer::setMidGain midGain should be between 0 and 1" << std::endl;
	}
	else {
		this->midGain = midGain;
	}
}

void DJAudioPlayer::setHighGain(double highGain)
{
	if (highGain < 0 || highGain > 1.0)
	{
		std::cout << "DJAudioPlayer::setHighGain highGain should be between 0 and 1" << std::endl;
	}
	else {
		this->highGain = highGain;
	}
}

void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::toggleEQ()
{
	onOffEQ = !onOffEQ;
}