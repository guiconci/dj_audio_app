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
