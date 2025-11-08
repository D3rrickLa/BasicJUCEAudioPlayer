/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicJUCEAudioPlayerAudioProcessor::BasicJUCEAudioPlayerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    formatManager.registerBasicFormats();
}

BasicJUCEAudioPlayerAudioProcessor::~BasicJUCEAudioPlayerAudioProcessor()
{
}

//==============================================================================
const juce::String BasicJUCEAudioPlayerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicJUCEAudioPlayerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicJUCEAudioPlayerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicJUCEAudioPlayerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicJUCEAudioPlayerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicJUCEAudioPlayerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicJUCEAudioPlayerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicJUCEAudioPlayerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicJUCEAudioPlayerAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicJUCEAudioPlayerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicJUCEAudioPlayerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);

}

void BasicJUCEAudioPlayerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    transportSource.releaseResources();

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicJUCEAudioPlayerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BasicJUCEAudioPlayerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Let the transportSource fill the buffer
    juce::AudioSourceChannelInfo info(&buffer, 0, buffer.getNumSamples());
    transportSource.getNextAudioBlock(info);
}

//==============================================================================
bool BasicJUCEAudioPlayerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicJUCEAudioPlayerAudioProcessor::createEditor()
{
    return new BasicJUCEAudioPlayerAudioProcessorEditor (*this);
}

//==============================================================================
void BasicJUCEAudioPlayerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicJUCEAudioPlayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void BasicJUCEAudioPlayerAudioProcessor::loadFile(const juce::File& audioFile)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    if (auto* reader = formatManager.createReaderFor(audioFile))
    {
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(),
            0,             // no extra buffering
            nullptr,       // default thread
            reader->sampleRate);
    }
}
void BasicJUCEAudioPlayerAudioProcessor::play() { transportSource.start(); }
void BasicJUCEAudioPlayerAudioProcessor::pause() { transportSource.stop(); }      // retains position
void BasicJUCEAudioPlayerAudioProcessor::stop() { transportSource.stop(); transportSource.setPosition(0.0); }
void BasicJUCEAudioPlayerAudioProcessor::setPosition(double seconds) { transportSource.setPosition(seconds); }
void BasicJUCEAudioPlayerAudioProcessor::setGain(float g) { transportSource.setGain(g); }

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicJUCEAudioPlayerAudioProcessor();
}

