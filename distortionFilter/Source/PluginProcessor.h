/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ProgrammingIIAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ProgrammingIIAudioProcessor();
    ~ProgrammingIIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void savePreset(juce::File name);
    void loadPreset(juce::File name);
    void loadProgPreset(int selection);
    void setMode(int selection);
    
    //Creating Value tree state
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

private:
    
   
    
    int currentFilterMode;
    
    
    //=========Creating DSP Objects================
    juce::dsp::LadderFilter<float> moogFilter;
    juce::dsp::Gain<float> driveGain;
  
  //  juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
    juce::AudioProcessorValueTreeState parameterVTS;
    std::atomic<float>* cutFreq = nullptr;
    std::atomic<float>* resQ = nullptr;
    std::atomic<float>* driveParameter = nullptr;
    std::atomic<float>* outputParameter = nullptr;
    std::atomic<float>* modeParameter = nullptr;

    
    
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgrammingIIAudioProcessor)
};
