/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Mu45FilterCalc/Mu45FilterCalc.h>
#include <StkLite-4.6.2/StkLite-4.6.2/BiQuad.h>

//==============================================================================
/**
*/
class BrianTPFinalDigitalHellAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BrianTPFinalDigitalHellAudioProcessor();
    ~BrianTPFinalDigitalHellAudioProcessor() override;

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

    void savePreset(const File& file);
    void loadPreset(const File& file);

    //==============================================================================

    void calcAlgorithmParams();

    int calcDWSP(float rawValue);
    int calcBitDepth(float rawValue);

    void setBoolParam(const String& paramID, bool value);

    void soloLowBand(bool isEnabled);
    void soloMidBand(bool isEnabled);
    void soloHighBand(bool isEnabled);

    float tickSampleData(float channelData,  stk::BiQuad* HPFilter, stk::BiQuad* LPFilter);

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    AudioProcessorValueTreeState& getAPVTS() { return parameters; }


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrianTPFinalDigitalHellAudioProcessor)

    stk::BiQuad mLowHPFilterL; stk::BiQuad mLowHPFilterR;
    stk::BiQuad mLowLPFilterL; stk::BiQuad mLowLPFilterR;
    stk::BiQuad mMidHPFilterL; stk::BiQuad mMidHPFilterR;
    stk::BiQuad mMidLPFilterL; stk::BiQuad mMidLPFilterR;
    stk::BiQuad mHighHPFilterL; stk::BiQuad mHighHPFilterR;
    stk::BiQuad mHighLPFilterL; stk::BiQuad mHighLPFilterR;

    bool lowEnabled = true;
    bool midEnabled = true;
    bool highEnabled = true;

    float loHPFc; // Low Band Hi Pass Cutoff Frequency
    float loLPFc; // Low Band Lo Pass Cutoff Frequency
    float midHPFc; // Mid Band Hi Pass Cutoff Frequency
    float midLPFc; // Mid Band Hi Pass Cutoff Frequency
    float hiHPFc; // High Band Hi Pass Cutoff Frequency
    float hiLPFc; // High Band Lo Pass Cutoff Frequency

    int lowDWSP; // Represents the amount of samples skipped by the processor for every tick on the low band
    int midDWSP; // Represents the amount of samples skipped by the processor for every tick on the mid band
    int highDWSP; // Represents the amount of samples skipped by the processor for every tick on the high band

    float lowBitDepth;
    float midBitDepth;
    float highBitDepth;

    float lowLevels;
    float midLevels;
    float highLevels;

    float linearGainLow;
    float linearGainMid;
    float linearGainHigh;

    static constexpr int maxDWSP = 127; // The max possible downsample rate;
    static constexpr int maxBitDepth = 11; // The max possible Bit Depth;

    float mFs; // Inital Sample Rate

    float Q = 0.707f; // Resonance for filters

    AudioProcessorValueTreeState parameters;

};
