/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicJUCEAudioPlayerAudioProcessorEditor::BasicJUCEAudioPlayerAudioProcessorEditor (BasicJUCEAudioPlayerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    

    loadButton.onClick = [this]
        {
            myChooser = std::make_unique<juce::FileChooser>(
                "Select an audio file...",
                juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                "*.wav;*.aiff;*.flac;*.mp3"
            );
            auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories | juce::FileBrowserComponent::canSelectFiles;

            myChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& fc)
                {
                    auto f = fc.getResult();
                    if (f.existsAsFile())
                        audioProcessor.loadFile(f); // Calls the Transport load function in processor

                }
            );
        };
    playButton.onClick = [this] {audioProcessor.play();};
    stopButton.onClick = [this] {audioProcessor.stop();};

    setSize (400, 300);
}

BasicJUCEAudioPlayerAudioProcessorEditor::~BasicJUCEAudioPlayerAudioProcessorEditor()
{
}

//==============================================================================
void BasicJUCEAudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void BasicJUCEAudioPlayerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto buttonHeight = 40;

    // Split the width into three equal sections
    auto buttonWidth = area.getWidth() / 3;

    loadButton.setBounds(area.removeFromLeft(buttonWidth).withHeight(buttonHeight));
    playButton.setBounds(area.removeFromLeft(buttonWidth).withHeight(buttonHeight));
    stopButton.setBounds(area.removeFromLeft(buttonWidth).withHeight(buttonHeight));

}
