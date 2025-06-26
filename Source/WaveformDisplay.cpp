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




