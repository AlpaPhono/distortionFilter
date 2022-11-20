/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UserInterface.h"


//==============================================================================
/**
*/
class ProgrammingIIAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ProgrammingIIAudioProcessorEditor (ProgrammingIIAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~ProgrammingIIAudioProcessorEditor() override;
    
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment BoxAttachment;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    

   
    juce::Slider cutOffSlider;
    juce::Label cutOffLabel;
    std::unique_ptr<SliderAttachment> cutOffAttachment;
    
    
    juce::Slider resonanceSlider;
    juce::Label resonanceLabel;
    std::unique_ptr<SliderAttachment> resonanceAttachment;

    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<SliderAttachment> driveAttachment;
    
    juce::Slider outputSlider;
    juce::Label outputLabel;
    std::unique_ptr<SliderAttachment> outputAttachment;
    

    juce::ComboBox filterMenu;
    juce::ComboBox presetMenu;
    std::unique_ptr<BoxAttachment> boxAttachment;
    


   
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ProgrammingIIAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    juce::TextButton save, load;
    
    UserInterface Ui;
    
    juce::Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgrammingIIAudioProcessorEditor)
};
