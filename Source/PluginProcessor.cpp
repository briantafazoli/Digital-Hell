/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BrianTPFinalDigitalHellAudioProcessor::BrianTPFinalDigitalHellAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{

}

BrianTPFinalDigitalHellAudioProcessor::~BrianTPFinalDigitalHellAudioProcessor()
{
}

//==============================================================================
const juce::String BrianTPFinalDigitalHellAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BrianTPFinalDigitalHellAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BrianTPFinalDigitalHellAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BrianTPFinalDigitalHellAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BrianTPFinalDigitalHellAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BrianTPFinalDigitalHellAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BrianTPFinalDigitalHellAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BrianTPFinalDigitalHellAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BrianTPFinalDigitalHellAudioProcessor::getProgramName (int index)
{
    return {};
}

void BrianTPFinalDigitalHellAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BrianTPFinalDigitalHellAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    mFs = sampleRate;

}

void BrianTPFinalDigitalHellAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BrianTPFinalDigitalHellAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BrianTPFinalDigitalHellAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // JUCE book-keeping to make sure there's no bad data in the output buffers
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    // Update the algorithm params
    calcAlgorithmParams();
    // Get the left and right audio buffers
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    int numSamples = buffer.getNumSamples();

    DBG("Channels: " + String(buffer.getNumChannels()));
    DBG("Samples per block: " + String(buffer.getNumSamples()));

    float* lowChannelDataL = new float[numSamples];
    float* lowChannelDataR = new float[numSamples];
    float* midChannelDataL = new float[numSamples];
    float* midChannelDataR = new float[numSamples];
    float* highChannelDataL = new float[numSamples];
    float* highChannelDataR = new float[numSamples];

    float lowCurrentSampL;
    float lowCurrentSampR;
    float midCurrentSampL;
    float midCurrentSampR;
    float highCurrentSampL;
    float highCurrentSampR;

    int lowSampleCounter = 0;
    int midSampleCounter = 0;
    int highSampleCounter = 0;

    // The inner loop! Step through each sample in the audio buffer
    for (int samp = 0; samp < buffer.getNumSamples(); samp++) {

        if (lowEnabled) {

            if (lowDWSP == 0) {

                // No downsampling: process every sample directly
                // Without this block of code, it will still downsample slightly.
                lowChannelDataL[samp] = tickSampleData(channelDataLeft[samp], &mLowHPFilterL, &mLowLPFilterL) * linearGainLow;
                lowChannelDataR[samp] = tickSampleData(channelDataRight[samp], &mLowHPFilterR, &mLowLPFilterR) * linearGainLow;

            }

            else {

                if (lowSampleCounter == 0) {

                    lowCurrentSampL = channelDataLeft[samp];
                    lowCurrentSampR = channelDataRight[samp];

                }

                lowChannelDataL[samp] = tickSampleData(lowCurrentSampL, &mLowHPFilterL, &mLowLPFilterL) * linearGainLow;
                lowChannelDataR[samp] = tickSampleData(lowCurrentSampR, &mLowHPFilterR, &mLowLPFilterR) * linearGainLow;

                if (lowSampleCounter == lowDWSP) lowSampleCounter = -1;

                lowSampleCounter++;

            }     

        }

        else {

            lowChannelDataL[samp] = 0;
            lowChannelDataR[samp] = 0;

        }

        if (midEnabled) {

            if (midDWSP == 0) {
                // No downsampling: process every sample directly
                // Without this block of code, it will still downsample slightly.
                midChannelDataL[samp] = tickSampleData(channelDataLeft[samp], &mMidHPFilterL, &mMidLPFilterL) * linearGainMid;
                midChannelDataR[samp] = tickSampleData(channelDataRight[samp], &mMidHPFilterR, &mMidLPFilterR) * linearGainMid;
            }

            else {

                if (midSampleCounter == 0) {

                    midCurrentSampL = channelDataLeft[samp];
                    midCurrentSampR = channelDataRight[samp];

                }

                midChannelDataL[samp] = tickSampleData(midCurrentSampL, &mMidHPFilterL, &mMidLPFilterL) * linearGainMid;
                midChannelDataR[samp] = tickSampleData(midCurrentSampR, &mMidHPFilterR, &mMidLPFilterR) * linearGainMid;

                if (midSampleCounter == midDWSP) midSampleCounter = -1;

                midSampleCounter++;

            }

        }

        else {

            midChannelDataL[samp] = 0;
            midChannelDataR[samp] = 0;

        }


        if (highEnabled) {

            if (highDWSP == 0) {
                // No downsampling: process every sample directly
                // Without this block of code, it will still downsample slightly.
                highChannelDataL[samp] = tickSampleData(channelDataLeft[samp], &mHighHPFilterL, &mHighLPFilterL) * linearGainHigh;
                highChannelDataR[samp] = tickSampleData(channelDataRight[samp], &mHighHPFilterR, &mHighLPFilterR) * linearGainHigh;
            }

            else {

                if (highSampleCounter == 0) {

                    highCurrentSampL = channelDataLeft[samp];
                    highCurrentSampR = channelDataRight[samp];

                }

                highChannelDataL[samp] = tickSampleData(highCurrentSampL, &mHighHPFilterL, &mHighLPFilterL) * linearGainHigh;
                highChannelDataR[samp] = tickSampleData(highCurrentSampR, &mHighHPFilterR, &mHighLPFilterR) * linearGainHigh;

                if (highSampleCounter == highDWSP) highSampleCounter = -1;

                highSampleCounter++;

            }

        }

        else {

            highChannelDataL[samp] = 0;
            highChannelDataR[samp] = 0;

        }

        /*Since the bitcrushing can still introduce artifacts, we need to sample them once again.*/
        

        channelDataLeft[samp] = lowChannelDataL[samp] + midChannelDataL[samp] + highChannelDataL[samp];
        channelDataRight[samp] = lowChannelDataR[samp] + midChannelDataR[samp] + highChannelDataR[samp];

        channelDataLeft[samp] = std::round(channelDataLeft[samp] * levels) / levels;
        channelDataRight[samp] = std::round(channelDataRight[samp] * levels) / levels;

        }

}

float calcLinearGain(float rawValue) {
    float linearGain = pow(10, (rawValue / 20));
    return linearGain;
}

int BrianTPFinalDigitalHellAudioProcessor::calcDWSP(float rawValue) {
    int output = static_cast<int> (rawValue + maxDWSP / 100.0f) - 1;
    return output;
}

float BrianTPFinalDigitalHellAudioProcessor::calcBitDepth(float rawValue) {
    float output = maxBitDepth - (maxBitDepth * (rawValue / 100)) + 1;
    return output;
}


void BrianTPFinalDigitalHellAudioProcessor::calcAlgorithmParams() {

    loHPFc = *parameters.getRawParameterValue("LoHPFCutoffFreq");
    loLPFc = *parameters.getRawParameterValue("LoLPFCutoffFreq");
    midHPFc = loLPFc;
    midLPFc = *parameters.getRawParameterValue("HiHPFCutoffFreq");
    hiHPFc = midLPFc;
    hiLPFc = *parameters.getRawParameterValue("HiLPFCutoffFreq");

    float rawGainLow = *parameters.getRawParameterValue("LoGain");
    float rawGainMid = *parameters.getRawParameterValue("MidGain");
    float rawGainHigh = *parameters.getRawParameterValue("HiGain");

    linearGainLow = calcLinearGain(rawGainLow);
    linearGainMid = calcLinearGain(rawGainMid);
    linearGainHigh = calcLinearGain(rawGainHigh);

    float loHPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsHPF(loHPFCoeffs, loHPFc, Q, mFs);

    float loLPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsLPF(loLPFCoeffs, loLPFc, Q, mFs);

    float midHPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsHPF(midHPFCoeffs, midHPFc, Q, mFs);

    float midLPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsLPF(midLPFCoeffs, midLPFc, Q, mFs);

    float hiHPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsHPF(hiHPFCoeffs, hiHPFc, Q, mFs);

    float hiLPFCoeffs[5];
    Mu45FilterCalc::calcCoeffsLPF(hiLPFCoeffs, hiLPFc, Q, mFs);

    mLowHPFilterL.setCoefficients(loHPFCoeffs[0], loHPFCoeffs[1], loHPFCoeffs[2], loHPFCoeffs[3], loHPFCoeffs[4]);
    mLowHPFilterR.setCoefficients(loHPFCoeffs[0], loHPFCoeffs[1], loHPFCoeffs[2], loHPFCoeffs[3], loHPFCoeffs[4]);
    mLowLPFilterL.setCoefficients(loLPFCoeffs[0], loLPFCoeffs[1], loLPFCoeffs[2], loLPFCoeffs[3], loLPFCoeffs[4]);
    mLowLPFilterR.setCoefficients(loLPFCoeffs[0], loLPFCoeffs[1], loLPFCoeffs[2], loLPFCoeffs[3], loLPFCoeffs[4]);

    mMidHPFilterL.setCoefficients(midHPFCoeffs[0], midHPFCoeffs[1], midHPFCoeffs[2], midHPFCoeffs[3], midHPFCoeffs[4]);
    mMidHPFilterR.setCoefficients(midHPFCoeffs[0], midHPFCoeffs[1], midHPFCoeffs[2], midHPFCoeffs[3], midHPFCoeffs[4]);
    mMidLPFilterL.setCoefficients(midLPFCoeffs[0], midLPFCoeffs[1], midLPFCoeffs[2], midLPFCoeffs[3], midLPFCoeffs[4]);
    mMidLPFilterR.setCoefficients(midLPFCoeffs[0], midLPFCoeffs[1], midLPFCoeffs[2], midLPFCoeffs[3], midLPFCoeffs[4]);

    mHighHPFilterL.setCoefficients(hiHPFCoeffs[0], hiHPFCoeffs[1], hiHPFCoeffs[2], hiHPFCoeffs[3], hiHPFCoeffs[4]);
    mHighHPFilterR.setCoefficients(hiHPFCoeffs[0], hiHPFCoeffs[1], hiHPFCoeffs[2], hiHPFCoeffs[3], hiHPFCoeffs[4]);
    mHighLPFilterL.setCoefficients(hiLPFCoeffs[0], hiLPFCoeffs[1], hiLPFCoeffs[2], hiLPFCoeffs[3], hiLPFCoeffs[4]);
    mHighLPFilterR.setCoefficients(hiLPFCoeffs[0], hiLPFCoeffs[1], hiLPFCoeffs[2], hiLPFCoeffs[3], hiLPFCoeffs[4]);

    float rawLoDWSP = *parameters.getRawParameterValue("LoDWSP");
    float rawMidDWSP = *parameters.getRawParameterValue("MidDWSP");
    float rawHiDWSP = *parameters.getRawParameterValue("HiDWSP");

    lowDWSP = calcDWSP(rawLoDWSP);
    midDWSP = calcDWSP(rawMidDWSP);
    highDWSP = calcDWSP(rawHiDWSP);

    int rawBitDepth = *parameters.getRawParameterValue("BitDepth");
    bitDepth = calcBitDepth(rawBitDepth);
    levels = pow(2.0f, bitDepth - 1);

    lowEnabled = *parameters.getRawParameterValue("LoEnable");
    midEnabled = *parameters.getRawParameterValue("MidEnable");
    highEnabled = *parameters.getRawParameterValue("HiEnable");

}

void BrianTPFinalDigitalHellAudioProcessor::soloLowBand(bool isEnabled) {

    if (isEnabled) {
        setBoolParam("LoEnable", true);
        setBoolParam("MidEnable", false);
        setBoolParam("HiEnable", false);
    }
    else {
        setBoolParam("LoEnable", true);
        setBoolParam("MidEnable", true);
        setBoolParam("HiEnable", true);
    }

}

void BrianTPFinalDigitalHellAudioProcessor::soloMidBand(bool isEnabled) {

    if (isEnabled) {
        setBoolParam("LoEnable", false);
        setBoolParam("MidEnable", true);
        setBoolParam("HiEnable", false);
    }
    else {
        setBoolParam("LoEnable", true);
        setBoolParam("MidEnable", true);
        setBoolParam("HiEnable", true);
    }

}

void BrianTPFinalDigitalHellAudioProcessor::soloHighBand(bool isEnabled) {

    if (isEnabled) {
        setBoolParam("LoEnable", false);
        setBoolParam("MidEnable", false);
        setBoolParam("HiEnable", true);
    }
    else {
        setBoolParam("LoEnable", true);
        setBoolParam("MidEnable", true);
        setBoolParam("HiEnable", true);
    }

}


void BrianTPFinalDigitalHellAudioProcessor::setBoolParam(const String& paramID, bool value) {

    if (auto* p = parameters.getRawParameterValue(paramID)) {

        p->store(value ? 1.0f : 0.0f);
        if (auto* param = parameters.getParameter(paramID)) param->sendValueChangedMessageToListeners(p->load());

    }

}

float BrianTPFinalDigitalHellAudioProcessor::tickSampleData(float channelData, stk::BiQuad* HPFilter, stk::BiQuad* LPFilter) {

    float output = HPFilter->tick(channelData);
    output = LPFilter->tick(output);

    return output;

}

//==============================================================================
bool BrianTPFinalDigitalHellAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BrianTPFinalDigitalHellAudioProcessor::createEditor()
{
    return new BrianTPFinalDigitalHellAudioProcessorEditor (*this);
}

//==============================================================================
void BrianTPFinalDigitalHellAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BrianTPFinalDigitalHellAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

AudioProcessorValueTreeState::ParameterLayout BrianTPFinalDigitalHellAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // ========== FILTERS ==========

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LoHPFCutoffFreq", // ID Name
        "Low Band HPF Cutoff Frequency (Hz)", // DAW Name
        juce::NormalisableRange<float>(20.0f, 500.0f), // Range
        50.0f, // Default value
        " Hz", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr                                     // Optional lambda for converting text back to value (not needed for now)
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LoLPFCutoffFreq", // ID Name
        "Low Band LPF / Mid Band HPF Cutoff Frequency (Hz)",
        juce::NormalisableRange<float>(50.0f, 5000.0f), 
        500.0f, // Default Value
        " Hz", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HiHPFCutoffFreq", // ID Name
        "High Band HPF / Mid Band LPF Cutoff Frequency (Hz)",
        juce::NormalisableRange<float>(500.0f, 15000.0f), 
        5000.0f,
        " Hz", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HiLPFCutoffFreq", 
        "High Band LPF Cutoff Frequency (Hz)",
        juce::NormalisableRange<float>(2500.0f, 20000.0f), 
        15000.0f,
        " Hz", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    // ========== BITCRUSH ==========

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LoDWSP", 
        "Low Band Downsample %",
        juce::NormalisableRange<float>(0.0f, 100.0f), 
        20.0f,
        " %", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "MidDWSP", 
        "Mid Band Downsample %",
        juce::NormalisableRange<float>(0.0f, 100.0f), 
        20.0f,
        " %", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HiDWSP", 
        "High Band Downsample %",
        juce::NormalisableRange<float>(0.0f, 100.0f), 
        20.0f,
        " %", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    // ========== GAIN ==========

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LoGain", 
        "Low Band Gain",
        juce::NormalisableRange<float>(-60.0f, 6.0f), 
        0.0f,
        " dB", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "MidGain", 
        "Mid Band Gain",
        juce::NormalisableRange<float>(-60.0f, 6.0f), 
        0.0f,
        " dB", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HiGain", 
        "High Band Gain",
        juce::NormalisableRange<float>(-60.0f, 6.0f), 
        0.0f,
        " dB", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
        ));

    // ========== BIT DEPTH ===============

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "BitDepth",
        "Bit Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        20.0f,
        " %", // Suffix
        juce::AudioProcessorParameter::genericParameter, // Type (leave as generic)
        [](float value, int) {                      // Lambda for displaying value
            return juce::String(static_cast<int>(value));
        },
        nullptr
    ));

    // ========== ENABLE TOGGLES ==========

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "LoEnable", "Low Band Enabled", true));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "MidEnable", "Mid Band Enabled", true));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "HiEnable", "High Band Enabled", true));

    return { params.begin(), params.end() };
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BrianTPFinalDigitalHellAudioProcessor();
}
