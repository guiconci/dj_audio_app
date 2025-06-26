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




/*
  ==============================================================================
    Main.cpp

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
class OtoDecksApplication  : public JUCEApplication
{
public:
    //==============================================================================
    OtoDecksApplication() {}
    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            // Set the minimum and maximum window size
            setResizeLimits(800, 640, 10000, 10000); // Minimum size: 600x400, Maximum size: 10000x10000
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (OtoDecksApplication)
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



/*
  ==============================================================================
    Main Component.h
    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "../Thirdparty/nlohmann/json.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    /** Function to expose the Decks to allow tracks loaded from playlist **/
	DeckGUI* getDeckGUI(int deckNum);


private:
    //==============================================================================
    // Your private member variables go here...
     
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache}; 

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache}; 

    MixerAudioSource mixerSource; 

    PlaylistComponent playlistComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Aug 2024 10:20:37pm
    Author:  guico

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI& leftDeck, DeckGUI& rightDeck)
	: leftDeck(leftDeck), rightDeck(rightDeck)
{
    // Read and parse the JSON file
	//--Get the path to the dataFiles folder and the playlist file
    juce::File dataFilesFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
        .getParentDirectory().getChildFile("dataFiles");
    juce::File playlistFile = dataFilesFolder.getChildFile("playlist.json");

    // Create the dataFiles folder if it doesn't exist
    if (!dataFilesFolder.exists())
        dataFilesFolder.createDirectory();

	//--Read from JSON file
    std::ifstream f(playlistFile.getFullPathName().toStdString());
    if (f.is_open())
    {
        try
        {
			//Parse the JSON file
            playlistInfoJSON = nlohmann::json::parse(f);
			//Populate the track titles array
            updateTrackTitles();
        }
        catch (const nlohmann::json::parse_error&)
        {

        }
    }

	//Code for table component
    tableComponent.getHeader().addColumn("Track title", 1, 260);
    tableComponent.getHeader().addColumn("Duration", 2, 70);
    tableComponent.getHeader().addColumn("Left Deck", 3, 150);
    tableComponent.getHeader().addColumn("Right Deck", 4, 150);
    tableComponent.getHeader().addColumn("Remove", 5, 80);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);

    //Add Addtrack button to the GUI and add listener
    addAndMakeVisible(addTrackButton);
    addTrackButton.addListener(this);

	//Register audio format
	formatManager.registerBasicFormats();

}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(0xFF2E2E2E).withAlpha(0.6f));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
	//Set bounds for table component
    tableComponent.setBounds(0, 0, getWidth(), getHeight());

	//Set bounds for add track button
	addTrackButton.setBounds(150,1, 70, 25);

}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if(rowIsSelected)
	{
		g.fillAll(juce::Colour(0xFF1DB954).withAlpha(0.5f));
	}
	else
	{
		g.fillAll(juce::Colours::lightslategrey.withAlpha(0.3f));
	}

}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < trackTitles.size())
    {
        std::string const &trackName = trackTitles[rowNumber];
        std::string const &trackDuration = playlistInfoJSON[trackName]["Duration"];

        if (columnId == 1)
        {
            g.drawText(trackName,
                2, 0,
                width, height,
                Justification::centredLeft, true);
        }

        if (columnId == 2)
        {
            g.drawText(trackDuration,
                2, 0,
                width, height,
                Justification::centredLeft, true);
        }
    }

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Load Left");
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);


            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Load Right");
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);


            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("X");
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            // Set button properties
            btn->setColour(TextButton::buttonColourId, juce::Colours::red); // Set background color to red

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;

}

void PlaylistComponent::buttonClicked(Button* button)
{
	if (button == &addTrackButton)
	{
        //File chooser to choose track
		auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
		fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
			{
				File file = chooser.getResult();
				if (file.exists())
				{
					//Create a reader for the file
                    auto* reader = formatManager.createReaderFor(file);

                    if (reader != nullptr)
                    {
						std::cout << "Reder not null" << std::endl;
                        //Get file name
                        std::string fileName = file.getFileName().toStdString();
                        //Calculate duration in seconds
                        double durationInSecs = reader->lengthInSamples / reader->sampleRate;
						std::string formatedDuration = Utilities::formatTotalTime(durationInSecs);
                        //Get path for the file
						std::string path = file.getFullPathName().toStdString();

                        playlistInfoJSON[fileName]["Duration"] = formatedDuration;
						playlistInfoJSON[fileName]["Path"] = path;
                        savePlaylistToFile();

                        //Update track titles
                        updateTrackTitles();

                        // Update the table component
                        tableComponent.updateContent();
                        repaint();
                    }
				}

			});
	}
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
		//Remove button - OWN code
        if (button->getButtonText() == "X")
        {
            // Remove the track from the JSON
            std::string trackName = trackTitles[id];
            playlistInfoJSON.erase(trackName);

            // Save the updated JSON to the file
            savePlaylistToFile();

            // Update track titles
            updateTrackTitles();

            // Update the table component
            tableComponent.updateContent();
            repaint();
        }
        else
        {
			// Load the track to the left or right deck
			int deckNumber = button->getButtonText() == "Load Left" ? 1 : 2;
			loadTrackToDeck(deckNumber, button->getButtonText().toStdString(), id);
        }
    }
}

//OWN code, reference from JUCE File class documentation and nlohmann json library
//https://docs.juce.com/master/classFile.html
//	--Write and read json
//https://www.reddit.com/r/cpp/comments/ovhrhn/what_json_library_do_you_suggest/
//https://stackoverflow.com/questions/70684671/how-do-i-read-write-json-with-c
// --nlohman json documentation
// https://github.com/nlohmann/json/blob/develop/README.md#creating-json-objects-from-json-literals 
// --Importing nlohman / json to JUCE
//https://forum.juce.com/t/importing-third-party-libraries-in-a-juce-project/36389/2
void PlaylistComponent::savePlaylistToFile()
{
    try {
		// Get the path to the dataFiles folder and the playlist file
        juce::File dataFilesFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
            .getParentDirectory().getChildFile("dataFiles");

		// Create the dataFiles folder if it doesn't exist
        if (!dataFilesFolder.exists())
            dataFilesFolder.createDirectory();

        juce::File playlistFile = dataFilesFolder.getChildFile("playlist.json");

        std::ofstream o(playlistFile.getFullPathName().toStdString());
        if (!o.is_open())
            throw std::runtime_error("Failed to open file");

        o << playlistInfoJSON.dump(4);
        o.close();
    }
    catch (const std::exception& e) {
        // Log error
    }
}

void PlaylistComponent::updateTrackTitles()
{
    trackTitles.clear(); // Clear the existing titles
    for (auto& [key, value] : playlistInfoJSON.items())
    {
        trackTitles.push_back(key); // Add track title to the list
    }
}


void PlaylistComponent::loadTrackToDeck(int deckNumber, std::string btnName, int btnId)
{
	std::string trackName = trackTitles[btnId];
	std::string trackPath = playlistInfoJSON[trackName]["Path"];
    //from string to juce url
    juce::File trackFile(trackPath);
    juce::URL url = juce::URL(trackFile);

	if (deckNumber == 1)
	{
		leftDeck.loadURL(url);
	}
	else
	{
		rightDeck.loadURL(url);
	}
	

}
/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Aug 2024 10:20:37pm
    Author:  guico

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "../Thirdparty/nlohmann/json.hpp"
#include "Utilities.h"
#include "DeckGUI.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public Button::Listener
{
public:
    PlaylistComponent(DeckGUI& leftDeck, DeckGUI& rightDeck);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground (juce::Graphics&, 
                             int rowNumber, 
                             int width, 
                             int height, 
                             bool rowIsSelected) override;

    void paintCell (juce::Graphics&,
                    int rowNumber,
					int columnId,
					int width,
					int height,
					bool rowIsSelected) override;

    Component*  refreshComponentForCell(int rowNumber, 
                                        int columnId, 
                                        bool isRowSelected, 
                                        Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    /** Function to save the added track to the json file**/
	void savePlaylistToFile();

    /** Function repopulate the track titles array**/
    void updateTrackTitles();

	/** Function to load track to left-right decks**/
	void loadTrackToDeck(int deckNumber, std::string btnName, int btnId);

private:
    // References to the deck components
    DeckGUI& leftDeck;
    DeckGUI& rightDeck;

    AudioFormatManager formatManager;
    juce::FileChooser fChooser{ "Select a file..." };

    TextButton addTrackButton{ "+ Add Track" };

    TableListBox tableComponent;
   
	//json to store playlist info - OWN code
    nlohmann::json playlistInfoJSON{};

	//vector to store track titles
    std::vector<std::string> trackTitles{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
/*
  ==============================================================================

    Utilities.cpp
    Created: 9 Sep 2024 12:11:17am
    Author:  guico

  ==============================================================================
*/

#include "Utilities.h"

std::string Utilities::formatTotalTime(double timeInSeconds)
{
	//total time display
	int minutes = static_cast<int>(timeInSeconds) / 60; //get min without decimals
	int seconds = static_cast<int>(timeInSeconds) % 60; //get seconds without decimals
	std::string timeStringTotal =
		(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + //ternary operator + min
		(seconds < 10 ? "0" : "") + std::to_string(seconds);        //ternary operator + sec

	return timeStringTotal;
}

std::string Utilities::formatCurrentTime(double timeInSeconds, double pos)
{
	//current time display
	int minutesCurrent = static_cast<int>(pos * timeInSeconds) / 60; //get min without decimals
	int secondsCurrent = static_cast<int>(pos * timeInSeconds) % 60; //get seconds without decimals
	std::string timeStringCurrent =
		(minutesCurrent < 10 ? "0" : "") + std::to_string(minutesCurrent) + ":" + //ternary operator + min
		(secondsCurrent < 10 ? "0" : "") + std::to_string(secondsCurrent);        //ternary operator + sec

	return timeStringCurrent;
}
/*
  ==============================================================================

    Utilities.h
    Created: 9 Sep 2024 12:11:17am
    Author:  guico

  ==============================================================================
*/

#pragma once
#include <string>

class Utilities
{
    public:
        /** Static function to transform the total seconds in formated min:sec **/
		static std::string formatTotalTime(double timeInSeconds);

        /** Static function to transform the current reprodution 
        time from seconds to formated min:sec **/
		static std::string formatCurrentTime(double timeInSeconds, double pos);

};
/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0),
	                             //initialize the playhead width as 0 and then set it in the resized function
	                             playHeadWidth(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (Colours::black.withAlpha(0.5f));

    // Draw an outline around the component
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);  

	// Draw the waveform if the file is loaded
    g.setColour (Colours::whitesmoke.withAlpha(0.7f));
    if(fileLoaded)
    {
        juce::Rectangle<int> thumbArea = getLocalBounds();
		thumbArea.removeFromBottom(10);
        audioThumb.drawChannel(g, 
        thumbArea,
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
	  // Draw the playhead, outline and fill
      g.setColour(Colours::lightgreen.withAlpha(0.8f));
      g.drawRect(position * getWidth() - getPlayHeadWidth() /2, 0, playHeadWidth, getHeight() - getHeight() / 5);
      g.setColour(Colours::lightgreen.withAlpha(0.15f));
      g.fillRect(position * getWidth() - getPlayHeadWidth() / 2, 0, playHeadWidth, getHeight() - getHeight() / 5);

      //Draw the time display
      // ---- Own code with references:
      //https://www.geeksforgeeks.org/static_cast-in-cpp/,
      //https://www.w3schools.com/cpp/cpp_conditions_shorthand.asp
      g.setColour(Colours::darkgrey.withAlpha(0.3f));
	  g.fillRect(0, getHeight() - getHeight() / 5, getWidth(), getHeight() / 5); //draw the rectangle for the time display
	  g.setColour(Colours::lightgreen.withAlpha(0.5f));
	  g.setFont(static_cast<float>(getHeight() / 5));
      double totalLength = audioThumb.getTotalLength();
      //current time display
      std::string timeStringCurrent = Utilities::formatCurrentTime(totalLength, position); //current time display
	  //total time display
	  std::string timeStringTotal = Utilities::formatTotalTime(totalLength);
	  //combine the strings and draw the time string
	  std::string timeStringCombined = timeStringCurrent + " / " + timeStringTotal; //combine the strings
	  std::string timeAndFileName = fileName + " - " + timeStringCombined;
	  g.drawText(timeAndFileName, getLocalBounds().withLeft(4), Justification::bottomLeft, true); //draw the time 
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
	//set the playhead width
    playHeadWidth = getWidth() / 35;
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  fileName = audioURL.getFileName().toStdString();
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }

  
}




/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "Utilities.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** Get the playhead width**/
	int getPlayHeadWidth() { return playHeadWidth; }

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
	std::string fileName = "";
    bool fileLoaded; 
    double position;
    double playHeadWidth;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
