/*
  ==============================================================================

    FrequencySliderComponent.h
    Created: 20 Jul 2025 2:14:49pm
    Author:  mrtaf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FrequencySliderLookAndFeel.h"

//==============================================================================
/*
*/
class FrequencySliderComponent  : public juce::Component
{
public:
    FrequencySliderComponent(AudioProcessorValueTreeState& state) : apvts(state)
    {

        frequencySliderLookandFeel = std::make_unique<FrequencySliderLookAndFeel>();
        mFrequencySlider.setLookAndFeel(frequencySliderLookandFeel.get());
        mFrequencySlider.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);

        Range<double> defaultValues = getDefaultValues();
        double defaultLowValue = defaultValues.getStart();
        double defaultHighValue = defaultValues.getEnd();
        mFrequencySlider.setMinAndMaxValues(minFrequency, maxFrequency);

        mFrequencySlider.setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
        mFrequencySlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
        mFrequencySlider.setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);

        mFrequencySlider.onValueChange = [this]() {
            if (auto* loParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("LoLPFCutoffFreq")))
            {
                loParam->setValueNotifyingHost(loParam->convertTo0to1((float)mFrequencySlider.getMinValue()));
            }

            if (auto* hiParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("HiHPFCutoffFreq")))
            {
                hiParam->setValueNotifyingHost(hiParam->convertTo0to1((float)mFrequencySlider.getMaxValue()));
            }
            };

        addAndMakeVisible(mFrequencySlider);

    }

    ~FrequencySliderComponent() override
    {

        mFrequencySlider.setLookAndFeel(nullptr);

    }

    void paint(Graphics& g) override {

        Rectangle<int> bounds = getLocalBounds();

        Rectangle<int> redSliderIndicator = Rectangle(bounds.getX(), bounds.getHeight() - 40, bounds.getWidth() / 2, 40);
        g.setColour(Colours::red);
        g.drawRect(redSliderIndicator, 1.0f);

        Rectangle<int> blueSliderIndicator = Rectangle(bounds.getWidth() / 2, bounds.getHeight() - 40, bounds.getWidth() / 2, 40);
        g.setColour(Colours::blue);
        g.drawRect(blueSliderIndicator, 1.0f);

    }

    void resized() override
    {
        mFrequencySlider.setBounds(getLocalBounds());

    }

    Range<double> getRange() /*Returns the minimum and maximum frequencies declared in this class*/ {
        return Range<double>(minFrequency, maxFrequency);
    }

    Range<double> getDefaultValues()/*Returns the default values for frequency cutoffs declared in this class*/ {
        return Range<double>(lowMidCutoff, highMidCutoff);
    }

    Rectangle<float> getBounds() {
        return bounds;
    }

    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencySliderComponent)

    Slider mFrequencySlider;

    std::unique_ptr<LookAndFeel> frequencySliderLookandFeel;

    AudioProcessorValueTreeState& apvts;

    double minFrequency = 20.0;
    double maxFrequency = 20000.0;

    float lowMidCutoff = 500.0;
    float highMidCutoff = 5000.0;

    Rectangle<float> bounds;

};
