/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

//Copyright 2025 Brian Tafazoli
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BrianTPFinalDigitalHellAudioProcessorEditor::BrianTPFinalDigitalHellAudioProcessorEditor (BrianTPFinalDigitalHellAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), frequencySliderComponent(p.getAPVTS())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(865, 600);

    auto& params = processor.getParameters();
    AudioParameterFloat* audioParam;

    // LOW HPF FREQUENCY

    auto* lowHPFcParam = audioProcessor.getAPVTS().getParameter("LoHPFCutoffFreq");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowHPFcParam))
    {
        mLowHPFcSlider.setRange(floatParam->range.start, floatParam->range.end);
        mLowHPFcSlider.setSkewFactorFromMidPoint(80);
    }

    mLowHPFcSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mLowHPFcSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    mLowHPFcSlider.setTextValueSuffix(" Hz");
    mLowHPFcSlider.setDoubleClickReturnValue(true, 50);
    addAndMakeVisible(mLowHPFcSlider);

    mLowHPFcAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "LoHPFCutoffFreq", mLowHPFcSlider);

    // HIGH LPF FREQUENCY

    auto* highLPFcParam = audioProcessor.getAPVTS().getParameter("HiLPFCutoffFreq");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highLPFcParam))
    {
        mHighLPFcSlider.setRange(floatParam->range.start, floatParam->range.end);
        mHighLPFcSlider.setSkewFactorFromMidPoint(15000);
    }

    mHighLPFcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mHighLPFcSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 20);
    mHighLPFcSlider.setTextValueSuffix(" Hz");
    mHighLPFcSlider.setDoubleClickReturnValue(true, 12000);
    addAndMakeVisible(mHighLPFcSlider);

    mHighLPFcAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "HiLPFCutoffFreq", mHighLPFcSlider);

    // LOW DWSP

    auto* lowDWSPParam = audioProcessor.getAPVTS().getParameter("LoDWSP");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowDWSPParam))
    {
        mLowDWSPSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mLowDWSPSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mLowDWSPSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mLowDWSPSlider.setTextValueSuffix(" %");
    mLowDWSPSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mLowDWSPSlider);

    mLowDWSPAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "LoDWSP", mLowDWSPSlider);

    // MID DWSP

    auto* midDWSPParam = audioProcessor.getAPVTS().getParameter("MidDWSP");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(midDWSPParam))
    {
        mMidDWSPSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mMidDWSPSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mMidDWSPSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mMidDWSPSlider.setTextValueSuffix(" %");
    mMidDWSPSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mMidDWSPSlider);

    mMidDWSPAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "MidDWSP", mMidDWSPSlider);
    
    // HIGH DWSP

    auto* highDWSPParam = audioProcessor.getAPVTS().getParameter("HiDWSP");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highDWSPParam))
    {
        mHighDWSPSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mHighDWSPSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mHighDWSPSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mHighDWSPSlider.setTextValueSuffix(" %");
    mHighDWSPSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mHighDWSPSlider);

    mHighDWSPAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "HiDWSP", mHighDWSPSlider);

    // LOW GAIN

    auto* lowDBParam = audioProcessor.getAPVTS().getParameter("LoGain");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowDBParam))
    {
        mLowDBSlider.setRange(floatParam->range.start, floatParam->range.end);
        mLowDBSlider.setSkewFactorFromMidPoint(-6);
    }

    mLowDBSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mLowDBSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 35);
    mLowDBSlider.setTextValueSuffix(" dB");
    mLowDBSlider.setDoubleClickReturnValue(true, 0);
    addAndMakeVisible(mLowDBSlider);

    mLowDBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "LoGain", mLowDBSlider);

    // MID GAIN

    auto* midDBParam = audioProcessor.getAPVTS().getParameter("MidGain");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowDBParam))
    {
        mMidDBSlider.setRange(floatParam->range.start, floatParam->range.end);
        mMidDBSlider.setSkewFactorFromMidPoint(-6);
    }

    mMidDBSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mMidDBSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 35);
    mMidDBSlider.setTextValueSuffix(" dB");
    mMidDBSlider.setDoubleClickReturnValue(true, 0);
    addAndMakeVisible(mMidDBSlider);

    mMidDBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "MidGain", mMidDBSlider);

    // HIGH GAIN

    auto* highDBParam = audioProcessor.getAPVTS().getParameter("HiGain");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highDBParam))
    {
        mHighDBSlider.setRange(floatParam->range.start, floatParam->range.end);
        
    }
    mHighDBSlider.setSkewFactorFromMidPoint(-6);
    mHighDBSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mHighDBSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 35);
    mHighDBSlider.setTextValueSuffix(" dB");
    mHighDBSlider.setDoubleClickReturnValue(true, 0);
    addAndMakeVisible(mHighDBSlider);

    mHighDBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "HiGain", mHighDBSlider);

    // LOW CRUSH

    auto* lowCrushParam = audioProcessor.getAPVTS().getParameter("LoBitDepth");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowCrushParam))
    {
        mLowCrushSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mLowCrushSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mLowCrushSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mLowCrushSlider.setTextValueSuffix(" %");
    mLowCrushSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mLowCrushSlider);

    mLowCrushAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "LoBitDepth", mLowCrushSlider);

    // MID CRUSH

    auto* midCrushParam = audioProcessor.getAPVTS().getParameter("MidBitDepth");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(midCrushParam))
    {
        mMidCrushSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mMidCrushSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mMidCrushSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mMidCrushSlider.setTextValueSuffix(" %");
    mMidCrushSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mMidCrushSlider);

    mMidCrushAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "MidBitDepth", mMidCrushSlider);

    // HIGH CRUSH

    auto* highCrushParam = audioProcessor.getAPVTS().getParameter("HighBitDepth");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highCrushParam))
    {
        mHighCrushSlider.setRange(floatParam->range.start, floatParam->range.end);
    }

    mHighCrushSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mHighCrushSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 25);
    mHighCrushSlider.setTextValueSuffix(" %");
    mHighCrushSlider.setDoubleClickReturnValue(true, 20);
    addAndMakeVisible(mHighCrushSlider);

    mHighCrushAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "HiBitDepth", mHighCrushSlider);

    // LOW SOLO ENABLE

    //audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
    mLowSoloEnable.setButtonText("Solo");
    mLowSoloEnable.setClickingTogglesState(true);
    mLowSoloEnable.addListener(this);
    addAndMakeVisible(mLowSoloEnable);

    // MID SOLO ENABLE

    mMidSoloEnable.setButtonText("Solo");
    mMidSoloEnable.setClickingTogglesState(true);
    mMidSoloEnable.addListener(this);
    addAndMakeVisible(mMidSoloEnable);

    // HIGH SOLO ENABLE

    mHighSoloEnable.setButtonText("Solo");
    mHighSoloEnable.setClickingTogglesState(true);
    mHighSoloEnable.addListener(this);
    addAndMakeVisible(mHighSoloEnable);

    // SAVE PRESET

    mSavePresetButton.setButtonText("Save Preset");
    mSavePresetButton.addListener(this);
    addAndMakeVisible(mSavePresetButton);

    // LOAD PRESET

    mLoadPresetButton.setButtonText("Load Preset");
    mLoadPresetButton.addListener(this);
    addAndMakeVisible(mLoadPresetButton);

    // Frequency Slider 

    addAndMakeVisible(frequencySliderComponent);

}

BrianTPFinalDigitalHellAudioProcessorEditor::~BrianTPFinalDigitalHellAudioProcessorEditor()
{
}

//==============================================================================
void BrianTPFinalDigitalHellAudioProcessorEditor::paint (juce::Graphics& g)
{


    // (Our component is opaque, so we must completely fill the background with a solid colour)
    /*g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));*/
    g.fillAll(Colours::black);

    g.setColour (Colours::lightgrey);
    g.setFont(get8BitFont_Bold().withHeight(25.0f));

    Rectangle<float> logoRectangle = Rectangle<float>::Rectangle(0, 0, 600, 100);
    g.drawImage(logo, logoRectangle);
    
    // DIVIDERS AND LARGE LABELS

    Line<float> lowDividerVert(Point<float>(300, 250), Point<float>(300, 590));
    g.drawLine(lowDividerVert);

    Line<float> highDividerVert(Point<float>(575, 250), Point<float>(575, 575));
    g.drawLine(highDividerVert);

    juce::Rectangle<int> lowLabelRectangle = juce::Rectangle<int>::Rectangle(0, 250, 300, 40);
    g.drawRect(lowLabelRectangle);
    g.drawFittedText("LOW", lowLabelRectangle, juce::Justification::centred, 1);

    juce::Rectangle<int> midLabelRectangle = juce::Rectangle<int>::Rectangle(300, 250, 275, 40);
    g.drawRect(midLabelRectangle);
    g.drawFittedText("MID", midLabelRectangle, juce::Justification::centred, 1);

    juce::Rectangle<int> highLabelRectangle = juce::Rectangle<int>::Rectangle(575, 250, 300, 40);
    g.drawRect(highLabelRectangle);
    g.drawFittedText("HIGH", highLabelRectangle, juce::Justification::centred, 1);

    // SMALLER LABLES

    // CRUSH KNOBS

    g.setColour(Colours::mediumpurple);
    g.setFont(get8BitFont().withHeight(45.0f));

    Rectangle<int> lowCrushLabel = Rectangle<int>::Rectangle(20, 310, 130, 50);
    g.drawFittedText("Crush:", lowCrushLabel, Justification::centred, 1);

    Rectangle<int> midCrushLabel = Rectangle<int>::Rectangle(310, 310, 130, 50);
    g.drawFittedText("Crush:", midCrushLabel, Justification::centred, 1);

    Rectangle<int> highCrushLabel = Rectangle<int>::Rectangle(580, 310, 130, 50);
    g.drawFittedText("Crush:", highCrushLabel, Justification::centred, 1);

    // DWSP

    g.setColour(Colours::mediumpurple);
    g.setFont(get8BitFont().withHeight(30.0f));

    Rectangle<int> lowDWSPLabel = Rectangle<int>::Rectangle(45, 355, 150, 100);
    g.drawFittedText("Downsample:", lowDWSPLabel, Justification::left, 1);

    Rectangle<int> midDWSPLabel = Rectangle<int>::Rectangle(330, 355, 150, 100);
    g.drawFittedText("Downsample:", midDWSPLabel, Justification::centred, 1);

    Rectangle<int> highDWSPLabel = Rectangle<int>::Rectangle(615, 355, 150, 100);
    g.drawFittedText("Downsample:", highDWSPLabel, Justification::left, 1);

    // GAIN

    g.setFont(get8BitFont().withHeight(30.0f));

    Rectangle<int> lowGainLabel = Rectangle<int>::Rectangle(163, 515, 100, 100);
    g.drawFittedText("Gain:", lowGainLabel, Justification::left, 1);

    Rectangle<int> midGainLabel = Rectangle<int>::Rectangle(450, 515, 100, 100);
    g.drawFittedText("Gain:", midGainLabel, Justification::left, 1);

    Rectangle<int> highGainLabel = Rectangle<int>::Rectangle(735, 515, 100, 100);
    g.drawFittedText("Gain:", highGainLabel, Justification::left, 1);
    
}

void BrianTPFinalDigitalHellAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // BIT DEPTH KNOBS

    Rectangle<int> lowCrushKnobRectangle = getSquare(255, 345, 110);
    mLowCrushSlider.setBounds(lowCrushKnobRectangle);

    Rectangle<int> midCrushKnobRectangle = getSquare(526, 345, 110);
    mMidCrushSlider.setBounds(midCrushKnobRectangle);

    Rectangle<int> highCrushKnobRectangle = getSquare(825, 345, 110);
    mHighCrushSlider.setBounds(highCrushKnobRectangle);

    // DWSP KNOBS

    Rectangle<int> lowDWSPKnobRectangle = getSquare(115, 490, 150);
    mLowDWSPSlider.setBounds(lowDWSPKnobRectangle);

    Rectangle<int> midDWSPKnobRectangle = getSquare(405, 490, 150);
    mMidDWSPSlider.setBounds(midDWSPKnobRectangle);

    Rectangle<int> highDWSPKnobRectangle = getSquare(690, 490, 150);
    mHighDWSPSlider.setBounds(highDWSPKnobRectangle);

    // GAIN SLIDERS

    mLowDBSlider.setBounds(230, 410, 50, 175);
    mMidDBSlider.setBounds(515, 410, 50, 175);
    mHighDBSlider.setBounds(800, 410, 50, 175);

    // SOLO BUTTONS

    mLowSoloEnable.setBounds(15, 540, 100, 50);
    mMidSoloEnable.setBounds(315, 540, 100, 50);
    mHighSoloEnable.setBounds(590, 540, 100, 50);

    mSavePresetButton.setBounds(625, 30, 100, 50);
    mLoadPresetButton.setBounds(725, 30, 100, 50);

    // Frequency Slider

    frequencySliderComponent.setBounds(frequencyComponentPosition);

    // 50, 125, 765, 75
    mFrequencySlider.setBounds(getBoundsForFreqSlider());

}

void BrianTPFinalDigitalHellAudioProcessorEditor::sliderValueChanged(Slider* slider) {

    auto& params = processor.getParameters();
    AudioParameterFloat* audioParam;

    if (slider == &mLowHPFcSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mLowHPFcSlider.getValue();
    }

    else if (slider == &mLowLPMidHPFcSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mLowLPMidHPFcSlider.getValue();
    }
    
    else if (slider == &mMidLPHighHPFcSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mMidLPHighHPFcSlider.getValue();
    }

    else if (slider == &mHighLPFcSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mHighLPFcSlider.getValue();
    }

    else if (slider == &mLowDWSPSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = mLowDWSPSlider.getValue();
    }

    else if (slider == &mMidDWSPSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = mMidDWSPSlider.getValue();
    }

    else if (slider == &mHighDWSPSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(6);
        *audioParam = mHighDWSPSlider.getValue();
    }

    else if (slider == &mLowDBSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(7);
        *audioParam = mLowDBSlider.getValue();
    }

    else if (slider == &mMidDBSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(8);
        *audioParam = mMidDBSlider.getValue();
    }

    else if (slider == &mHighDBSlider) {
        audioParam = (AudioParameterFloat*)params.getUnchecked(9);
        *audioParam = mHighDBSlider.getValue();
    }

}

void BrianTPFinalDigitalHellAudioProcessorEditor::buttonClicked(Button* button) {

    auto& params = processor.getParameters();
    AudioParameterFloat* audioParam;

    if (button == &mLowSoloEnable) {
        bool lowSolo = mLowSoloEnable.getToggleStateValue().getValue();
        audioProcessor.soloLowBand(lowSolo);
        mMidSoloEnable.setToggleState(false, dontSendNotification);
        mHighSoloEnable.setToggleState(false, dontSendNotification);
    }

    else if (button == &mMidSoloEnable) {
        bool midSolo = mMidSoloEnable.getToggleStateValue().getValue();
        audioProcessor.soloMidBand(midSolo);
        mLowSoloEnable.setToggleState(false, dontSendNotification);
        mHighSoloEnable.setToggleState(false, dontSendNotification);
    }

    else if (button == &mHighSoloEnable) {
        bool highSolo = mHighSoloEnable.getToggleStateValue().getValue();
        audioProcessor.soloHighBand(highSolo);
        mLowSoloEnable.setToggleState(false, dontSendNotification);
        mMidSoloEnable.setToggleState(false, dontSendNotification);
    }

    else if (button == &mSavePresetButton) {
        FileChooser chooser("Save Preset", File::getSpecialLocation(File::userDocumentsDirectory), "*.preset");
        if (chooser.browseForFileToSave(true))
        {
            audioProcessor.savePreset(chooser.getResult());
        }
    }

    else if (button == &mLoadPresetButton) {

        juce::FileChooser chooser("Load Preset", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.preset");
        if (chooser.browseForFileToOpen())
        {
            audioProcessor.loadPreset(chooser.getResult());
        }

    }

}

