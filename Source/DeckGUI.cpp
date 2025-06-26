/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Set slider colors
    auto setSliderColors = [](juce::Slider& slider) {
        slider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey.withAlpha(0.6f)); // Track color
        //set track color for rotary slider
		slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey.withAlpha(0.6f));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFF1DB954)); // Thumb color
		slider.setColour(juce::Slider::rotarySliderFillColourId, 
                         juce::Colour(0xFF1DB954).withAlpha(0.25f)); // Rotary fill color
        //set fill color for horizontal slider
		slider.setColour(juce::Slider::trackColourId, juce::Colour(0xFF1DB954).withAlpha(0.25f));
        };

    setSliderColors(volSlider);
    setSliderColors(speedSlider);
    setSliderColors(lowGainSlider);
    setSliderColors(midGainSlider);
    setSliderColors(highGainSlider);

	//Add buttons and sliders to the GUI
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);
	addAndMakeVisible(eqButton);

    // Initialize labels
    addAndMakeVisible(volLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);

    addAndMakeVisible(lowGainLabel);
    lowGainLabel.setText("Low Gain", juce::dontSendNotification);
    lowGainLabel.attachToComponent(&lowGainSlider, false);

    addAndMakeVisible(midGainLabel);
    midGainLabel.setText("Mid Gain", juce::dontSendNotification);
    midGainLabel.attachToComponent(&midGainSlider, false);

    addAndMakeVisible(highGainLabel);
    highGainLabel.setText("High Gain", juce::dontSendNotification);
    highGainLabel.attachToComponent(&highGainSlider, false);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(lowGainSlider);
	addAndMakeVisible(midGainSlider);
	addAndMakeVisible(highGainSlider);

    addAndMakeVisible(waveformDisplay);

    // Set the EQ button to toggle its state
    eqButton.setClickingTogglesState(true);

    // Set default colors for buttons
    playButton.setColour(TextButton::buttonColourId, Colours::green.withAlpha(0.2f));
    playButton.setColour(TextButton::buttonOnColourId, Colours::green.withAlpha(0.8f));
    stopButton.setColour(TextButton::buttonColourId, Colours::red.withAlpha(0.2f));
    stopButton.setColour(TextButton::buttonOnColourId, Colours::red.withAlpha(0.7f));
	stopButton.setToggleState(true, dontSendNotification);
    eqButton.setColour(TextButton::buttonColourId, juce::Colour(0xFF1DB954).withAlpha(0.06f));
    eqButton.setColour(TextButton::buttonOnColourId, juce::Colour(0xFF1DB954).withAlpha(0.6f));
    loadButton.setColour(TextButton::buttonColourId, Colours::orange.withAlpha(0.3f));

	//Add listeners to buttons and sliders
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
	eqButton.addListener(this);
	waveformDisplay.addMouseListener(this, false);

    volSlider.addListener(this);
    speedSlider.addListener(this);
	lowGainSlider.addListener(this);
	midGainSlider.addListener(this);
	highGainSlider.addListener(this);

	//Set slider ranges
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 5.0);
	lowGainSlider.setRange(0.0, 1.0);
	midGainSlider.setRange(0.0, 1.0);
	highGainSlider.setRange(0.0, 1.0);

	//Set slider default values
	volSlider.setValue(0.5);
	speedSlider.setValue(1.0);
	lowGainSlider.setValue(1.0);
	midGainSlider.setValue(1.0);
	highGainSlider.setValue(1.0);

    //Set sliders style
	lowGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	midGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	highGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

    // Set text box style and precision
    lowGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    midGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    highGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);

    volSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    lowGainSlider.setNumDecimalPlacesToDisplay(2);
    midGainSlider.setNumDecimalPlacesToDisplay(2);
    highGainSlider.setNumDecimalPlacesToDisplay(2);

    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{

    g.fillAll (juce::Colour(0xFF2E2E2E).withAlpha(0.6f));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    //Set bounds for waveform display
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

	//Set bounds for play and stop buttons
    playButton.setBounds(0, rowH*2, getWidth() / 3, rowH);
    stopButton.setBounds(getWidth() / 3, rowH * 2, getWidth() / 3, rowH);
    //Set bounds for load button
    loadButton.setBounds((getWidth() / 3) * 2, rowH * 2, getWidth() / 3, rowH);

	//Set bounds for sliders
	int rotarySliderWidth = getWidth() / 4;
    volSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 4, getWidth(), rowH);
	lowGainSlider.setBounds(0, rowH * 5, rotarySliderWidth, rowH * 3);
    midGainSlider.setBounds(rotarySliderWidth, rowH * 5, rotarySliderWidth, rowH * 3);
	highGainSlider.setBounds(rotarySliderWidth * 2, rowH * 5, rotarySliderWidth, rowH * 3);

    // Position labels
    volLabel.setBounds(getWidth() * 0.85, rowH * 3 - 3, getWidth(), 20);
    speedLabel.setBounds(getWidth() * 0.85, rowH * 4 - 3, getWidth(), 20);
    lowGainLabel.setBounds(0, rowH * 5 - 8, rotarySliderWidth, 20);
    midGainLabel.setBounds(rotarySliderWidth, rowH * 5 - 8, rotarySliderWidth, 20);
    highGainLabel.setBounds(rotarySliderWidth * 2, rowH * 5 - 8, rotarySliderWidth, 20);

	//Set bounds for eq button
	eqButton.setBounds(rotarySliderWidth * 3, rowH * 5 + rowH/2, rotarySliderWidth, rowH);




}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
        playButton.setToggleState(true, dontSendNotification);
        stopButton.setToggleState(false, dontSendNotification);
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
        playButton.setToggleState(false, dontSendNotification);
        stopButton.setToggleState(true, dontSendNotification);
    }

	 if (button == &eqButton)
	 {
		 std::cout << "EQ button was clicked " << std::endl;
		 player->toggleEQ();
	 }

    if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
				  loadURL(URL{ chosenFile });
            }
        });
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

	if (slider == &lowGainSlider)
	{
		player->setLowGain(slider->getValue());
	}

	if (slider == &midGainSlider)
	{
		player->setMidGain(slider->getValue());
	}

	if (slider == &highGainSlider)
	{
		player->setHighGain(slider->getValue());
	}
    
}

void DeckGUI::mouseDown(const MouseEvent& event)
{
    //Waveform display mouse events
    if (event.eventComponent == &waveformDisplay)
    {
        double waveWidth = waveformDisplay.getWidth();
        double clickX = event.getMouseDownX();
		//new position after clicking on the waveform
		double pos = (clickX / waveWidth);
		player->setPositionRelative(pos);
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

void DeckGUI::loadURL(URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    //adjust buttons toggle state
    playButton.setToggleState(false, dontSendNotification);
    stopButton.setToggleState(true, dontSendNotification);
    eqButton.setToggleState(false, dontSendNotification);

    //Reset slider default values
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);
    lowGainSlider.setValue(1.0);
    midGainSlider.setValue(1.0);
    highGainSlider.setValue(1.0);
}


    

