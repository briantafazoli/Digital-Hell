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
#include "TwoValueSliderAttachment.h"

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

        // Loading Images

        redSlider = ImageCache::getFromMemory(BinaryData::red_slider_png, BinaryData::red_slider_pngSize);
        DBG("Red Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

        blueSlider = ImageCache::getFromMemory(BinaryData::blue_slider_png, BinaryData::blue_slider_pngSize);
        DBG("Blue Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

        // Default Values and Range

        //Range<double> defaultValues = getDefaultValues();
        //double defaultLowValue = defaultValues.getStart();
        //double defaultHighValue = defaultValues.getEnd();
        //mFrequencySlider.setRange(getRange(), 0);

        //mFrequencySlider.setMinAndMaxValues(minFrequency, maxFrequency);

        mFrequencySlider.setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
        mFrequencySlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
        mFrequencySlider.setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);

        // Sending Values to processor

        addAndMakeVisible(mFrequencySlider);

        twoValueAttachment = std::make_unique<TwoValueSliderAttachment>(
            apvts,
            "LoLPFCutoffFreq",
            "HiHPFCutoffFreq",
            mFrequencySlider);

        mFrequencySlider.setRange(getRange(), 0);

        mFrequencySlider.setMinAndMaxValues(lowMidCutoff, highMidCutoff);

        mFrequencySlider.setSkewFactorFromMidPoint(6324.6);

    }

    ~FrequencySliderComponent() override
    {

        mFrequencySlider.setLookAndFeel(nullptr);

    }

    void paint(Graphics& g) override {

        Rectangle<int> bounds = getLocalBounds();
        //g.setColour(Colours::white);
        //g.drawRect(bounds, 1.0f);

        scaleImages();

        Rectangle<int> redSliderIndicator = Rectangle(bounds.getX(), bounds.getCentreY(), bounds.getWidth() / 2, 60);
        g.setColour(Colours::red);
        //g.drawRect(redSliderIndicator, 1.0f);

        g.drawImage(redSlider, redSliderIndicator.getX(), redSliderIndicator.getY(), sliderImageWidth, sliderImageHeight, 0.0f, 0.0f, redSlider.getWidth(), redSlider.getHeight());

        Rectangle<int> blueSliderIndicator = Rectangle(bounds.getWidth() / 2, bounds.getCentreY(), bounds.getWidth() / 2, 60);
        g.setColour(Colours::blue);
        //g.drawRect(blueSliderIndicator, 1.0f);

        g.drawImage(blueSlider, blueSliderIndicator.getX(), blueSliderIndicator.getY(), sliderImageWidth, sliderImageHeight, 0.0f, 0.0f, redSlider.getWidth(), redSlider.getHeight());

        g.setColour(Colours::white);
        g.setFont(get8BitFont().withHeight(20.0f));

        g.drawText("\t \t  determines the separation between the LOW", redSliderIndicator, Justification::topLeft);
        g.drawText("and MID bands", redSliderIndicator, Justification::centred);

        g.drawText("\t \t  determines the separation between the MID", blueSliderIndicator, Justification::topLeft);
        g.drawText("and HIGH bands", blueSliderIndicator, Justification::centred);

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

    void scaleImages() /* This just scales the images by the scale factor */ {

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

    std::unique_ptr<TwoValueSliderAttachment> twoValueAttachment;

};
