/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicJUCEAudioPlayerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicJUCEAudioPlayerAudioProcessorEditor (BasicJUCEAudioPlayerAudioProcessor&);
    ~BasicJUCEAudioPlayerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BasicJUCEAudioPlayerAudioProcessor& audioProcessor;
    juce::TextButton loadButton{ "Load File" }; 
    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    std::unique_ptr<juce::FileChooser> myChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicJUCEAudioPlayerAudioProcessorEditor)
};
