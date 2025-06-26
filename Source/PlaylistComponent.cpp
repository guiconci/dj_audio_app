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