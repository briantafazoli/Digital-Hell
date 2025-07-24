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

        minParam = apvtsRef.getParameter(minParamID);
        maxParam = apvtsRef.getParameter(maxParamID);

        jassert(minParam && maxParam);

        //Set Initial Values
        slider.setMinAndMaxValues(minParam->getValue(), maxParam->getValue());

       /* Sync Slider to parameters*/
        this->sliderRef.onValueChange = [this]() {
            if (this->updating) return;
            this->updating = true;

            if (this->minParam != nullptr)
                this->minParam->setValueNotifyingHost(this->minParam->convertTo0to1((float)this->sliderRef.getMinValue()));

            if (this->maxParam != nullptr)
                this->maxParam->setValueNotifyingHost(this->maxParam->convertTo0to1((float)this->sliderRef.getMaxValue()));

            this->updating = false;
            };



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

    RangedAudioParameter* minParam = nullptr;
    RangedAudioParameter* maxParam = nullptr;

    bool updating = false;

};