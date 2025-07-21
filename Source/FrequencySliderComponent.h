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
#include "PluginEditor.h"

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

        redSlider = ImageCache::getFromMemory(BinaryData::red_slider_png, BinaryData::red_slider_pngSize);
        DBG("Red Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

        blueSlider = ImageCache::getFromMemory(BinaryData::blue_slider_png, BinaryData::blue_slider_pngSize);
        DBG("Blue Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

    }

    ~FrequencySliderComponent() override
    {

        mFrequencySlider.setLookAndFeel(nullptr);

    }

    void paint(Graphics& g) override {

        Rectangle<int> bounds = getLocalBounds();
        g.setColour(Colours::white);
        g.drawRect(bounds, 1.0f);

        scaleImages();

        Rectangle<int> redSliderIndicator = Rectangle(bounds.getX(), bounds.getCentreY() - 25, bounds.getWidth() / 2, 60);
        g.setColour(Colours::red);
        g.drawRect(redSliderIndicator, 1.0f);

        g.drawImage(redSlider, redSliderIndicator.getX(), redSliderIndicator.getY(), sliderImageWidth, sliderImageHeight, 0.0f, 0.0f, redSlider.getWidth(), redSlider.getHeight());

        Rectangle<int> blueSliderIndicator = Rectangle(bounds.getWidth() / 2, bounds.getHeight() / 2 - 25, bounds.getWidth() / 2, 60);
        g.setColour(Colours::blue);
        g.drawRect(blueSliderIndicator, 1.0f);

        g.drawImage(blueSlider, blueSliderIndicator.getX(), blueSliderIndicator.getY(), sliderImageWidth, sliderImageHeight, 0.0f, 0.0f, redSlider.getWidth(), redSlider.getHeight());

        g.setColour(Colours::white);
        g.setFont(get8BitFont().withHeight(20.0f));

        g.drawText("\t \t  determines the separation between the LOW", redSliderIndicator, Justification::topLeft);
        g.drawText("and MID bands", redSliderIndicator, Justification::left);

        g.drawText("\t \t  determines the separation between the MID", blueSliderIndicator, Justification::topLeft);
        g.drawText("and HIGH bands", blueSliderIndicator, Justification::left);

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

    void scaleImages() {

        sliderImageWidth = redSlider.getWidth() * scaleFactor;
        sliderImageHeight = redSlider.getHeight() * scaleFactor;

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

    Image redSlider;
    Image blueSlider;

    const float scaleFactor = 0.175; // How much i'm scaling the image by

    float sliderImageWidth;
    float sliderImageHeight;

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

};
