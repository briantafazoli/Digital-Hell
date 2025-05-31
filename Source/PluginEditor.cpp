/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

Copyright 2025 Brian Tafazoli

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BrianTPFinalDigitalHellAudioProcessorEditor::BrianTPFinalDigitalHellAudioProcessorEditor (BrianTPFinalDigitalHellAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
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

    // LOW LPF, MID HPF FREQUENCY

    auto* lowLPFcParam = audioProcessor.getAPVTS().getParameter("LoLPFCutoffFreq");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(lowLPFcParam))
    {
        mLowLPMidHPFcSlider.setRange(floatParam->range.start, floatParam->range.end);
        mLowLPMidHPFcSlider.setSkewFactorFromMidPoint(500);
    }

    mLowLPMidHPFcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mLowLPMidHPFcSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    mLowLPMidHPFcSlider.setTextValueSuffix(" Hz");
    mLowLPMidHPFcSlider.setDoubleClickReturnValue(true, 500);
    addAndMakeVisible(mLowLPMidHPFcSlider);

    mLowLPMidHPFcAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "LoLPFCutoffFreq", mLowLPMidHPFcSlider);

    // MID LPF, HIGH HPF FREQUENCY

    auto* highHPFcParam = audioProcessor.getAPVTS().getParameter("HiHPFCutoffFreq");
    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(highHPFcParam))
    {
        mMidLPHighHPFcSlider.setRange(floatParam->range.start, floatParam->range.end);
        mMidLPHighHPFcSlider.setSkewFactorFromMidPoint(5000);
    }

    mMidLPHighHPFcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mMidLPHighHPFcSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    mMidLPHighHPFcSlider.setTextValueSuffix(" Hz");
    mMidLPHighHPFcSlider.setDoubleClickReturnValue(true, 5000);
    addAndMakeVisible(mMidLPHighHPFcSlider);

    mMidLPHighHPFcAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "HiHPFCutoffFreq", mMidLPHighHPFcSlider);

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
        mLowDBSlider.setSkewFactorFromMidPoint(-10);
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
        mMidDBSlider.setSkewFactorFromMidPoint(-10);
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
        mHighDBSlider.setSkewFactorFromMidPoint(-10);
    }

    mHighDBSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mHighDBSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
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

}

BrianTPFinalDigitalHellAudioProcessorEditor::~BrianTPFinalDigitalHellAudioProcessorEditor()
{
}

//==============================================================================
void BrianTPFinalDigitalHellAudioProcessorEditor::paint (juce::Graphics& g)
{


    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (Colours::lightgrey);
    g.setFont(Font("Courier New", 25.0f, Font::bold));

    Rectangle<float> logoRectangle = Rectangle<float>::Rectangle(0, 0, 600, 100);
    g.drawImage(logo, logoRectangle);
    
    // DIVIDERS AND LARGE LABELS

    Line<float> lowDividerVert(Point<float>(300, 100), Point<float>(300, 575));
    g.drawLine(lowDividerVert);

    Line<float> highDividerVert(Point<float>(575, 100), Point<float>(575, 575));
    g.drawLine(highDividerVert);

    Line<float> lowDividerHorz(Point<float>(0, 140), Point<float>(875, 140));
    g.drawLine(lowDividerHorz);

    Line<float> highDividerHorz(Point<float>(0, 100), Point<float>(875, 100));
    g.drawLine(highDividerHorz);

    juce::Rectangle<int> lowLabelRectangle = juce::Rectangle<int>::Rectangle(0, 100, 300, 40);
    g.drawFittedText("LOW", lowLabelRectangle, juce::Justification::centred, 1);

    juce::Rectangle<int> midLabelRectangle = juce::Rectangle<int>::Rectangle(300, 100, 275, 40);
    g.drawFittedText("MID", midLabelRectangle, juce::Justification::centred, 1);

    juce::Rectangle<int> highLabelRectangle = juce::Rectangle<int>::Rectangle(575, 100, 300, 40);
    g.drawFittedText("HIGH", highLabelRectangle, juce::Justification::centred, 1);

    // SMALLER LABLES

    // CUTOFFS

    g.setColour(Colours::mediumpurple);
    g.setFont(Font("Courier New", 35.0f, Font::plain));

    juce::Rectangle<int> lowCutoffLabel = juce::Rectangle<int>::Rectangle(15, 150, 150, 100);
    g.drawFittedText("Cutoff:", lowCutoffLabel, Justification::left, 1);

    /*juce::Rectangle<int> midCutoffLabel = juce::Rectangle<int>::Rectangle(315, 140, 100, 100);
    g.drawFittedText("Cutoff:", midCutoffLabel, Justification::left, 1);*/

    juce::Rectangle<int> highCutoffLabel = juce::Rectangle<int>::Rectangle(590, 150, 150, 100);
    g.drawFittedText("Cutoff:", highCutoffLabel, Justification::left, 1);

    g.setColour(Colours::lightgrey);
    g.setFont(Font("Arial", 17.0f, Font::plain));

    // CUTOFF FILTER LABELS

    Rectangle<int> lowCutoffHPFLabel = Rectangle<int>::Rectangle(180, 200, 115, 50);
    g.drawFittedText("Hi Pass", lowCutoffHPFLabel, Justification::centred, 1);
    
    Rectangle<int> midCutoffHPFLabel = Rectangle<int>::Rectangle(315, 200, 115, 50);
    g.drawFittedText("Hi Pass", midCutoffHPFLabel, Justification::centred, 1);

    Rectangle<int> midCutoffLPFLabel = Rectangle<int>::Rectangle(430, 200, 115, 50);
    g.drawFittedText("Lo Pass", midCutoffLPFLabel, Justification::centred, 1);

    Rectangle<int> highCutoffLPFLabel = Rectangle<int>::Rectangle(755, 200, 115, 50);
    g.drawFittedText("Lo Pass", highCutoffLPFLabel, Justification::centred, 1);

    g.setFont(Font("Arial", 11.0f, Font::plain));

    Rectangle<int> lowCutoffLPFLabel = Rectangle<int>::Rectangle(315, 215, 115, 50);
    g.drawFittedText("+ Lo Pass (LOW)", lowCutoffLPFLabel, Justification::centred, 1);

    Rectangle<int> highCutoffHPFLabel = Rectangle<int>::Rectangle(430, 215, 115, 50);
    g.drawFittedText("+ Hi Pass (HIGH)", highCutoffHPFLabel, Justification::centred, 1);

    // BIT DEPTH

    g.setColour(Colours::mediumpurple);
    g.setFont(Font("Courier New", 35.0f, Font::plain));

    Rectangle<int> lowCrushLabel = Rectangle<int>::Rectangle(5, 270, 130, 50);
    g.drawFittedText("Crush:", lowCrushLabel, Justification::centred, 1);

    Rectangle<int> midCrushLabel = Rectangle<int>::Rectangle(310, 270, 130, 50);
    g.drawFittedText("Crush:", midCrushLabel, Justification::centred, 1);

    Rectangle<int> highCrushLabel = Rectangle<int>::Rectangle(580, 270, 130, 50);
    g.drawFittedText("Crush:", highCrushLabel, Justification::centred, 1);

    // DWSP

    g.setColour(Colours::mediumpurple);
    g.setFont(Font("Courier New", 30.0f, Font::plain));

    Rectangle<int> lowDWSPLabel = Rectangle<int>::Rectangle(-25, 325, 300, 100);
    g.drawFittedText("Downsample:", lowDWSPLabel, Justification::centred, 1);

    Rectangle<int> midDWSPLabel = Rectangle<int>::Rectangle(275, 325, 275, 100);
    g.drawFittedText("Downsample:", midDWSPLabel, Justification::centred, 1);

    Rectangle<int> highDWSPLabel = Rectangle<int>::Rectangle(550, 325, 300, 100);
    g.drawFittedText("Downsample:", highDWSPLabel, Justification::centred, 1);

    // GAIN

    g.setFont(Font("Courier New", 32.0f, Font::plain));

    Rectangle<int> lowGainLabel = Rectangle<int>::Rectangle(125, 525, 100, 100);
    g.drawFittedText("Gain:", lowGainLabel, Justification::left, 1);

    Rectangle<int> midGainLabel = Rectangle<int>::Rectangle(400, 525, 100, 100);
    g.drawFittedText("Gain:", midGainLabel, Justification::left, 1);

    Rectangle<int> highGainLabel = Rectangle<int>::Rectangle(700, 525, 100, 100);
    g.drawFittedText("Gain:", highGainLabel, Justification::left, 1);


    
    // OBJECTS

    // DWSP KNOB



}

void BrianTPFinalDigitalHellAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // FREQUENCIES

    mLowHPFcSlider.setBounds(200, 140, 80, 80);
    mLowLPMidHPFcSlider.setBounds(335, 140, 80, 80);
    mMidLPHighHPFcSlider.setBounds(450, 140, 80, 80);
    mHighLPFcSlider.setBounds(775, 140, 80, 80);

    // BIT DEPTH KNOBS

    Rectangle<int> lowCrushKnobRectangle = getSquare(252, 305, 115);
    mLowCrushSlider.setBounds(lowCrushKnobRectangle);

    Rectangle<int> midCrushKnobRectangle = getSquare(526, 305, 115);
    mMidCrushSlider.setBounds(midCrushKnobRectangle);

    Rectangle<int> highCrushKnobRectangle = getSquare(825, 305, 115);
    mHighCrushSlider.setBounds(highCrushKnobRectangle);

    // DWSP KNOBS

    Rectangle<int> lowDWSPKnobRectangle = getSquare(125, 460, 150);
    mLowDWSPSlider.setBounds(lowDWSPKnobRectangle);

    Rectangle<int> midDWSPKnobRectangle = getSquare(412, 460, 150);
    mMidDWSPSlider.setBounds(midDWSPKnobRectangle);

    Rectangle<int> highDWSPKnobRectangle = getSquare(700, 460, 150);
    mHighDWSPSlider.setBounds(highDWSPKnobRectangle);

    // GAIN SLIDERS

    mLowDBSlider.setBounds(225, 390, 50, 200);
    mMidDBSlider.setBounds(500, 390, 50, 200);
    mHighDBSlider.setBounds(800, 390, 50, 200);

    // SOLO BUTTONS

    mLowSoloEnable.setBounds(15, 550, 100, 50);
    mMidSoloEnable.setBounds(315, 550, 100, 50);
    mHighSoloEnable.setBounds(590, 550, 100, 50);

    mSavePresetButton.setBounds(625, 30, 100, 50);
    mLoadPresetButton.setBounds(725, 30, 100, 50);

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

