/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProgrammingIIAudioProcessorEditor::ProgrammingIIAudioProcessorEditor (ProgrammingIIAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    
    juce::LookAndFeel::setDefaultLookAndFeel(&Ui);
    background = juce::ImageCache::getFromMemory(BinaryData::WallpaperDog17248257_jpg, BinaryData::WallpaperDog17248257_jpgSize);
    
//=================Creating cutOff Slider and Label=============================
    addAndMakeVisible(cutOffSlider);
    addAndMakeVisible(cutOffLabel);
    cutOffAttachment.reset(new SliderAttachment(valueTreeState,"cFreq", cutOffSlider));
    cutOffLabel.setText("CUTOFF", juce::dontSendNotification);
    //cutOffLabel.set
    cutOffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    cutOffSlider.setTextValueSuffix("Hz");
    cutOffLabel.attachToComponent(&cutOffSlider, false);
//=================Creating resonance Slider and Label=============================
    addAndMakeVisible(resonanceSlider);
    addAndMakeVisible(resonanceLabel);
    resonanceAttachment.reset(new SliderAttachment(valueTreeState,"Q", resonanceSlider));
   resonanceLabel.setText("RESONANCE", juce::dontSendNotification);
   resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
   resonanceLabel.attachToComponent(&resonanceSlider, false);
    
//=================Creating Drive Slider and Label=============================
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(driveLabel);
    driveAttachment.reset(new SliderAttachment(valueTreeState,"DRIVE", driveSlider));
    driveLabel.setText("DRIVE", juce::dontSendNotification);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    driveSlider.setTextValueSuffix("%");
    driveLabel.attachToComponent(&driveSlider, false);
    
    //=================Creating Output Slider and Label=============================
        addAndMakeVisible(outputSlider);
        addAndMakeVisible(outputLabel);
        outputAttachment.reset(new SliderAttachment(valueTreeState,"OUTPUT", outputSlider));
        outputLabel.setText("OUTPUT", juce::dontSendNotification);
        outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
        outputSlider.setTextValueSuffix("dB");
        outputLabel.attachToComponent(&outputSlider, false);
    
    //=======================Save and Load Buttons==========================
   /* addAndMakeVisible(save);
    save.setButtonText("save");
    save.onClick=[this] {
        juce::FileChooser fileChooser("Save As", juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory));
        if (fileChooser.browseForFileToSave(true))
        {
            audioProcessor.savePreset(fileChooser.getResult());
        }
            };
    
    addAndMakeVisible(load);
    load.setButtonText("load");
    load.onClick=[this] {
        juce::FileChooser fileChooser("load", juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory));
        if (fileChooser.browseForFileToOpen())
        {
        
            audioProcessor.loadPreset(fileChooser.getResult());
        }
            };
    
    */
//====================ComboBox===========================
    addAndMakeVisible(filterMenu);
    
    
    filterMenu.addSectionHeading("HighPass");
    filterMenu.addItem("High Pass 12", 1);
    filterMenu.addItem("High Pass 24", 2);
    filterMenu.addSeparator();
    
    filterMenu.addSectionHeading("LowPass");
    filterMenu.addItem("Low Pass 12", 3);
    filterMenu.addItem("Low Pass 24", 4);
    filterMenu.addSeparator();
    
    filterMenu.addSectionHeading("BandPass");
    filterMenu.addItem("Band Pass 12", 5);
    filterMenu.addItem("Band Pass 24", 6);
    filterMenu.addSeparator();
    
    boxAttachment.reset(new BoxAttachment(valueTreeState,"FILTERMODE", filterMenu)); 
    
    //====preset comboBox =====
    
  /*  addAndMakeVisible(presetMenu);
    
    
    presetMenu.addItem("Dirty Vocal", 1);
    presetMenu.addItem("Underwater Party", 2);
    presetMenu.addItem("Phone Filter", 3);
    
    
    boxAttachment.reset(new BoxAttachment(valueTreeState,"PRESETS", filterMenu)); */
    
    
    
    //=======================+Backgroun==============================
    
    
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (250, 500);
}
ProgrammingIIAudioProcessorEditor::~ProgrammingIIAudioProcessorEditor()
{
  
}

//==============================================================================
void ProgrammingIIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.drawImage(background, getLocalBounds().toFloat());
    

 
  
}

void ProgrammingIIAudioProcessorEditor::resized()
{
// sets the position and size of the slieer with arguments (x, y, width, height)
    

    cutOffSlider.setBounds(100, 275, 125, 125);
    resonanceSlider.setBounds(25,400,80,80);
    driveSlider.setBounds(20, 150, 80, 80);
    outputSlider.setBounds(160 , 150, 90, 90);
    save.setBoundsRelative(0.59,0.01,0.2,0.05);
    load.setBoundsRelative(0.8, 0.01, 0.2, 0.05);
    filterMenu.setBoundsRelative(0.01, 0.01, 0.5, 0.05);
    
    
}
