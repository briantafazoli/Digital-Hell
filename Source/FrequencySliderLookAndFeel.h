/*
  ==============================================================================

    FrequencySliderLookAndFeel.h
    Created: 20 Jul 2025 8:12:22pm
    Author:  mrtaf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FrequencySliderLookAndFeel  : public juce::LookAndFeel_V4
{
public:

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {

        // Define bounds
        juce::Rectangle<float> bounds((float)x, (float)y, (float)width, (float)height);

        // Outer white rectangle
        juce::Rectangle outerWhiteRectangle = { bounds.getX(), bounds.getY() + 10.0f, bounds.getWidth(), 15.0f };
        g.setColour(juce::Colours::white);
        g.fillRect(outerWhiteRectangle);

        // Inner black rectangle
        juce::Rectangle innerBlackRectangle = { bounds.getX() + 7.5f, bounds.getY() + 13.75f, bounds.getWidth() - 15.0f, 7.5f };
        g.setColour(juce::Colours::black);
        g.fillRect(innerBlackRectangle);

        // 8-bit squares
        g.fillRect(bounds.getX(), bounds.getY() + 10.0f, 7.5f, 3.75f);                        // top-left
        g.fillRect(bounds.getRight() - 7.5f, bounds.getY() + 10.0f, 7.5f, 3.75f);             // top-right
        g.fillRect(bounds.getX(), bounds.getY() + 21.25f, 7.5f, 3.75f);                       // bottom-left
        g.fillRect(bounds.getRight() - 7.5f, bounds.getY() + 21.25f, 7.5f, 3.75f);            // bottom-right

        // Draw thumbs
        const float thumbRadius = 6.0f;
        g.setColour(juce::Colours::red);
        g.fillEllipse(minSliderPos - thumbRadius, bounds.getCentreY() - thumbRadius, thumbRadius * 2, thumbRadius * 2);

        g.setColour(juce::Colours::blue);
        g.fillEllipse(maxSliderPos - thumbRadius, bounds.getCentreY() - thumbRadius, thumbRadius * 2, thumbRadius * 2);

    }

    int getSliderThumbRadius(juce::Slider& slider) override
    {
        return 6;
    }
};

