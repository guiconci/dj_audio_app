/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
	               //Added for waveform display mouse events
	               public MouseListener,   
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    /** implement mouse events **/
	void mouseDown(const MouseEvent& event) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

    /**Function to expose the load track function from player to allow loading from playlist**/
	void loadURL(URL audioURL);

private:
    juce::FileChooser fChooser{"Select a file..."};

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    TextButton eqButton{"EQ \n ON-OFF"};

  
    Slider volSlider; 
    Slider speedSlider;
	Slider lowGainSlider;
	Slider midGainSlider;
	Slider highGainSlider;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 

    // Labels for sliders
    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label lowGainLabel;
    juce::Label midGainLabel;
    juce::Label highGainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
