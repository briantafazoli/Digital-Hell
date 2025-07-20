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

        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        auto bounds = getLocalBounds().toFloat();

        g.setColour (juce::Colours::white);
        g.setFont (juce::FontOptions (14.0f));
        g.drawText ("FrequencySlider", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text

        Rectangle outerWhiteRectangle = Rectangle(bounds.getX(), bounds.getY() + 27.5f, bounds.getWidth(), 20.0f);
        g.drawRect(outerWhiteRectangle);
        g.fillRect(outerWhiteRectangle);

        

    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

    void setRange(float minFreq, float maxFreq);
    void setValues(float lowMidCutoff, float highMidCutoff);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencySlider)

    float minFrequency = 20.0;
    float maxFrequency = 20000.0;

    float lowMidCutoff = 500.0;
    float highMidCutoff = 5000.0;

};
