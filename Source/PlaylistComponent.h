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
