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

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FrequencySlider.h"

//==============================================================================
/**
*/
class BrianTPFinalDigitalHellAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    BrianTPFinalDigitalHellAudioProcessorEditor (BrianTPFinalDigitalHellAudioProcessor&);
    ~BrianTPFinalDigitalHellAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BrianTPFinalDigitalHellAudioProcessor& audioProcessor;

    Slider mLowHPFcSlider;
    Slider mLowLPMidHPFcSlider;
    Slider mMidLPHighHPFcSlider;
    Slider mHighLPFcSlider;

    FrequencySlider frequencySlider;

    Slider mLowCrushSlider;
    Slider mMidCrushSlider;
    Slider mHighCrushSlider;

    Slider mLowDWSPSlider;
    Slider mMidDWSPSlider;
    Slider mHighDWSPSlider;

    Slider mLowDBSlider;
    Slider mMidDBSlider;
    Slider mHighDBSlider;

    ToggleButton mLowSoloEnable;
    ToggleButton mMidSoloEnable;
    ToggleButton mHighSoloEnable;

    TextButton mSavePresetButton{ "Save Preset" };
    TextButton mLoadPresetButton{ "Load Preset" };

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowHPFcAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowLPMidHPFcAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidLPHighHPFcAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighLPFcAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowDWSPAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidDWSPAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighDWSPAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowCrushAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidCrushAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighCrushAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mLowDBAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mMidDBAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mHighDBAttachment;

    juce::Rectangle<int> getSquare(float centerX, float centerY, float sideLength) {
        // Calculate and return the rectangle
        return juce::Rectangle<int>(
            centerX - (sideLength / 2), // Top-left X
            centerY - (sideLength / 2), // Top-left Y
            sideLength,                // Width
            sideLength                 // Height
        );
    };

    Image logo = ImageCache::getFromMemory(BinaryData::digitalHellLogo_png, BinaryData::digitalHellLogo_pngSize);

    static const Font& get8BitFont()
    {
        static Font pixelOperator(Font(Typeface::createSystemTypefaceFor(BinaryData::PixelOperator_ttf,
            BinaryData::PixelOperator_ttfSize)));
        return pixelOperator;
    }

    static const Font& get8BitFont_Bold()
    {
        static Font pixelOperator(Font(Typeface::createSystemTypefaceFor(BinaryData::PixelOperatorBold_ttf,
            BinaryData::PixelOperatorBold_ttfSize)));
        return pixelOperator;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrianTPFinalDigitalHellAudioProcessorEditor)
};
