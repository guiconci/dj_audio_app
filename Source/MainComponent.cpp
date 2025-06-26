/*
  ==============================================================================

	MainComponent.cpp
    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
	: playlistComponent(deckGUI1, deckGUI2)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(playlistComponent);


    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	//Pass samples per block and sample rate into players
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xFF2E2E2E).withAlpha(0.6f));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight() / 2);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight() / 2);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);

}

DeckGUI* MainComponent::getDeckGUI(int deckNum)
{
	if (deckNum == 1)
	{
		return &deckGUI1;
	}
	else if (deckNum == 2)
	{
		return &deckGUI2;
	}
	else
	{
		return nullptr;
	}
}



