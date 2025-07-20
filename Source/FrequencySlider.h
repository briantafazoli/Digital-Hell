/*
  ==============================================================================

    FrequencySlider.h
    Created: 20 Jul 2025 2:14:49pm
    Author:  mrtaf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FrequencySlider  : public juce::Component
{
public:
    FrequencySlider()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~FrequencySlider() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        /*g.drawRect (getLocalBounds(), 1);*/   // draw an outline around the component

        g.setColour (juce::Colours::white);
        g.setFont (juce::FontOptions (14.0f));
        g.drawText ("FrequencySlider", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text

        Rectangle outerWhiteRectangle = Rectangle(bounds.getX(), bounds.getY() + 30.0f, bounds.getWidth(), 15.0f);
        g.drawRect(outerWhiteRectangle);
        g.fillRect(outerWhiteRectangle);

        Rectangle innerBlackRectangle = Rectangle(bounds.getX() + 7.5f, bounds.getY() + 33.75f, bounds.getWidth() - 15.0f, 7.5f);
        g.setColour(Colours::black);
        g.drawRect(innerBlackRectangle);
        g.fillRect(innerBlackRectangle);

        // FAUX 8 BIT

        Rectangle topLeftBlackSquare = Rectangle(bounds.getX(), bounds.getY() + 30.0f, 7.5f, 3.75f);
        g.drawRect(topLeftBlackSquare);
        g.fillRect(topLeftBlackSquare);

        Rectangle topRightBlackSquare = Rectangle(bounds.getWidth() - 7.5f, bounds.getY() + 30.0f, 7.5f, 3.75f);
        g.drawRect(topRightBlackSquare);
        g.fillRect(topRightBlackSquare);

        Rectangle bottomLeftBlackSquare = Rectangle(bounds.getX(), bounds.getY() + 41.25f, 7.5f, 3.75f);
        g.drawRect(bottomLeftBlackSquare);
        g.fillRect(bottomLeftBlackSquare);

        Rectangle bottomRightBlackSquare = Rectangle(bounds.getWidth() - 7.5f, bounds.getY() + 41.25f, 7.5f, 3.75f);
        g.drawRect(bottomRightBlackSquare);
        g.fillRect(bottomRightBlackSquare);

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

    Range<float> getRange() {
        return Range<float>(minFrequency, maxFrequency);
    }

    Range<float> getDefaultValues() {
        return Range<float>(lowMidCutoff, highMidCutoff);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencySlider)

    float minFrequency = 20.0;
    float maxFrequency = 20000.0;

    float lowMidCutoff = 500.0;
    float highMidCutoff = 5000.0;

};
