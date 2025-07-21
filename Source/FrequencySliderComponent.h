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
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

        frequencySliderLookandFeel = std::make_unique<FrequencySliderLookAndFeel>();
        mFrequencySlider.setLookAndFeel(frequencySliderLookandFeel.get());

        mFrequencySlider.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
        Range<double> defaultValues = getDefaultValues();
        double defaultLowValue = defaultValues.getStart();
        double defaultHighValue = defaultValues.getEnd();
        mFrequencySlider.setMinAndMaxValues(defaultLowValue, defaultHighValue);
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

    //void paint (juce::Graphics& g) override
    //{
    //    /* This demo code just fills the component's background and
    //       draws some placeholder text to get you started.

    //       You should replace everything in this method with your own
    //       drawing code..
    //    */

    //    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //    g.setColour (juce::Colours::grey);
    //    /*g.drawRect (getLocalBounds(), 1);*/   // draw an outline around the component

    //    g.setColour (juce::Colours::white);
    //    g.setFont (juce::FontOptions (14.0f));
    //    g.drawText ("FrequencySlider", getLocalBounds(),
    //                juce::Justification::centred, true);   // draw some placeholder text

    //    Rectangle<float> bounds = getLocalBounds().toFloat();
    //    setBoundsWithinFile(bounds);

    //    Rectangle outerWhiteRectangle = Rectangle(bounds.getX(), bounds.getY() + 30.0f, bounds.getWidth(), 15.0f);
    //    g.drawRect(outerWhiteRectangle);
    //    g.fillRect(outerWhiteRectangle);

    //    Rectangle innerBlackRectangle = Rectangle(bounds.getX() + 7.5f, bounds.getY() + 33.75f, bounds.getWidth() - 15.0f, 7.5f);
    //    g.setColour(Colours::black);
    //    g.drawRect(innerBlackRectangle);
    //    g.fillRect(innerBlackRectangle);

    //    // FAUX 8 BIT

    //    Rectangle topLeftBlackSquare = Rectangle(bounds.getX(), bounds.getY() + 30.0f, 7.5f, 3.75f);
    //    g.drawRect(topLeftBlackSquare);
    //    g.fillRect(topLeftBlackSquare);

    //    Rectangle topRightBlackSquare = Rectangle(bounds.getWidth() - 7.5f, bounds.getY() + 30.0f, 7.5f, 3.75f);
    //    g.drawRect(topRightBlackSquare);
    //    g.fillRect(topRightBlackSquare);

    //    Rectangle bottomLeftBlackSquare = Rectangle(bounds.getX(), bounds.getY() + 41.25f, 7.5f, 3.75f);
    //    g.drawRect(bottomLeftBlackSquare);
    //    g.fillRect(bottomLeftBlackSquare);

    //    Rectangle bottomRightBlackSquare = Rectangle(bounds.getWidth() - 7.5f, bounds.getY() + 41.25f, 7.5f, 3.75f);
    //    g.drawRect(bottomRightBlackSquare);
    //    g.fillRect(bottomRightBlackSquare);

    //}

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

        mFrequencySlider.setBounds(getLocalBounds());

    }

    Range<double> getRange() {
        return Range<double>(minFrequency, maxFrequency);
    }

    Range<double> getDefaultValues() {
        return Range<double>(lowMidCutoff, highMidCutoff);
    }

    void setBoundsWithinFile(Rectangle<float> newBounds) {
        bounds = newBounds;
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
