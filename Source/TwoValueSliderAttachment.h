/*
  ==============================================================================

    TwoValueSliderAttachment.h
    Created: 21 Jul 2025 2:54:05pm
    Author:  mrtaf

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TwoValueSliderAttachment : AudioProcessorValueTreeState::Listener
{
public:
    TwoValueSliderAttachment(AudioProcessorValueTreeState& apvts,
        const String& minParamID,
        const String& maxParamID,
        Slider& slider)
        : apvtsRef(apvts), minParamID(minParamID), maxParamID(maxParamID), sliderRef(slider)
    {

        jassert(slider.getSliderStyle() == Slider::SliderStyle::TwoValueHorizontal);

        auto* minParam = apvts.getParameter(minParamID);
        auto* maxParam = apvts.getParameter(maxParamID);

        jassert(minParam && maxParam);

        //Set Initial Values
        slider.setMinAndMaxValues(minParam->getValue(), maxParam->getValue());

        //Sync Slider to parameters
        //slider.onValueChange = [this]() {
        //    if (updating) return;
        //    updating = true;
        //    apvtsRef.getParameter(minParamID)->setValueNotifyingHost((float)sliderRef.getMinValue());
        //    apvtsRef.getParameter(maxParamID)->setValueNotifyingHost((float)sliderRef.getMaxValue());
        //    updating = false;
        //    };

        apvtsRef.addParameterListener(minParamID, this);
        apvtsRef.addParameterListener(maxParamID, this);

    }

    ~TwoValueSliderAttachment() {

        apvtsRef.removeParameterListener(minParamID, this);
        apvtsRef.removeParameterListener(maxParamID, this);

    }

    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        if (updating) return;
        updating = true;

        if (parameterID == minParamID)
            sliderRef.setMinValue(newValue, juce::dontSendNotification);
        else if (parameterID == maxParamID)
            sliderRef.setMaxValue(newValue, juce::dontSendNotification);

        updating = false;
    }

private:

    AudioProcessorValueTreeState& apvtsRef;
    String minParamID;
    String maxParamID;
    Slider& sliderRef;

    bool updating = false;

};