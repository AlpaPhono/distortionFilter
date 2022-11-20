/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProgrammingIIAudioProcessor::ProgrammingIIAudioProcessor()
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
,parameterVTS(*this, nullptr, juce::Identifier("ProgrammingII"), createParameters())
{
    cutFreq = parameterVTS.getRawParameterValue("cFreq");
    resQ = parameterVTS.getRawParameterValue("Q");
    driveParameter = parameterVTS.getRawParameterValue("DRIVE");
    outputParameter = parameterVTS.getRawParameterValue("OUTPUT");
    modeParameter = parameterVTS.getRawParameterValue("FILTERMODE");
    
    //highPassParameter=parameterVTS.getParameterAsValue("HP");

}

ProgrammingIIAudioProcessor::~ProgrammingIIAudioProcessor()
{
}

//==============================================================================
const juce::String ProgrammingIIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ProgrammingIIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ProgrammingIIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ProgrammingIIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ProgrammingIIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ProgrammingIIAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ProgrammingIIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ProgrammingIIAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ProgrammingIIAudioProcessor::getProgramName (int index)
{
    return {};
}

void ProgrammingIIAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ProgrammingIIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    //====Creating and populating process spec=======
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
  
    //=========Reset and preparing filter=============
    moogFilter.reset();
    moogFilter.prepare(spec);
    setMode(modeParameter->load());
    currentFilterMode = modeParameter->load();
    
    //============Reset and preparing Gain===========
    driveGain.reset();
    driveGain.prepare(spec);
    driveGain.setRampDurationSeconds(0.1);

   
}

void ProgrammingIIAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ProgrammingIIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ProgrammingIIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    
    //++++++++++++++++++++++++Filter Processing +++++++++++++++++++++++++++
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (currentFilterMode != modeParameter->load())
    {
        setMode(modeParameter->load());
        currentFilterMode = modeParameter->load();
    }
    moogFilter.setCutoffFrequencyHz(cutFreq->load());
    moogFilter.setResonance(resQ->load());
    moogFilter.setDrive(driveParameter->load() / 20.0 + 1.0);
    
    
    
    //moogFilter.setMode(juce::dsp::LadderFilterMode::BPF24);
    
    
    auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
   
    moogFilter.process(context);
    
    //+++++++++++++++++++++++++ Distortion/dsp code++++++++++++++++++++++++++++++
    
    driveGain.setGainDecibels(outputParameter->load());
    driveGain.process(context);
    
    //float output = (outputParameter->load()/100);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample< buffer.getNumSamples(); sample++)
        {
            float headroom = 0.5;
            channelData[sample]= tanh(headroom * channelData[sample]) / tanh(headroom);
        }
    }
}

//==============================================================================
bool ProgrammingIIAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ProgrammingIIAudioProcessor::createEditor()
{
    return new ProgrammingIIAudioProcessorEditor (*this, parameterVTS);
}

//==============================================================================
void ProgrammingIIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    /*juce::MemoryOutputStream(destData, true).writeFloat(driveParameter->load());
    juce::MemoryOutputStream(destData, true).writeFloat(resQ->load());
    juce::MemoryOutputStream(destData, true).writeInt(cutFreq->load());*/
    auto state = parameterVTS.copyState();
            std::unique_ptr<juce::XmlElement> xml (state.createXml());
            copyXmlToBinary (*xml, destData);
    
}

void ProgrammingIIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
     
            if (xmlState.get() != nullptr)
                if (xmlState->hasTagName (parameterVTS.state.getType()))
                    parameterVTS.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProgrammingIIAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout ProgrammingIIAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>("cFreq",
                                                         "cutOffFrequency",
                                                         juce::NormalisableRange<float>(50, 22000, 1, 0.5),
                                                         500));
    params.add(std::make_unique<juce::AudioParameterFloat>("Q",
                                                           "Resonance",
                                                           0.0f,
                                                           1.0f,
                                                           0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("DRIVE",
                                                           "Drive",
                                                           0.0,
                                                           100.0f,
                                                           20.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("OUTPUT",
                                                           "Output",
                                                           -12.0,
                                                           12.0,
                                                           0.0));
   params.add(std::make_unique<juce::AudioParameterChoice>("FILTERMODE",
                                                          "FilterMode",
                                                          juce::StringArray("HP12", "HP24", "LP12", "LP24", "BP12", "BP24"),
                                                            0));
    return params;
}

void ProgrammingIIAudioProcessor::setMode(int selection)
{
    switch (selection)
    {
        case 1:
            moogFilter.setMode(juce::dsp::LadderFilterMode::HPF24);
            break;
        case 2:
            moogFilter.setMode(juce::dsp::LadderFilterMode::LPF12);
            break;
        case 3:
            moogFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
            break;
        case 4:
            moogFilter.setMode(juce::dsp::LadderFilterMode::BPF12);
            break;
        case 5:
            moogFilter.setMode(juce::dsp::LadderFilterMode::BPF24);
            break;
        default:
            moogFilter.setMode(juce::dsp::LadderFilterMode::HPF12);
            break;
    }
}

void ProgrammingIIAudioProcessor::savePreset(juce::File fileName)
{
    if (fileName.existsAsFile())                                     //checking if file exsists
    {
        fileName.deleteFile();                                      //if it exsists delete
    }
    fileName.create();                                              //creating file to write to
    juce::FileOutputStream outputStream(fileName);
    parameterVTS.copyState().writeToStream(outputStream);           //Writing the value tree to that file
}

void ProgrammingIIAudioProcessor::loadPreset(juce::File fileName)
{
    if (fileName.existsAsFile())                                             //Checking presets exsits
    {
                                                                            //Reading from saved file
        juce::FileInputStream inputStream(fileName);
        parameterVTS.replaceState(juce::ValueTree::readFromStream(inputStream));
        
    }
}
   /* void ProgrammingIIAudioProcessor::loadProgPreset(int selection)
    {
        switch(selection)
        {
            case 0:
        parameterVTS.replaceState(juce::ValueTree::readFromData(BinaryData::dirtyVocal, BinaryData::dirtyVocalSize));
                break;
            
            case 1:
        parameterVTS.replaceState(juce::ValueTree::readFromData(BinaryData::Underwater_Party, BinaryData::Underwater_PartySize));
                break;
            case 2:
        parameterVTS.replaceState(juce::ValueTree::readFromData(BinaryData::phonefilter, BinaryData::phonefilterSize));
                break;
            default:
                break;
            
        }
        } */
        



