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

    FrequencySliderLookAndFeel() {

        redSlider = ImageCache::getFromMemory(BinaryData::red_slider_png, BinaryData::red_slider_pngSize);
        DBG("Red Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

        blueSlider = ImageCache::getFromMemory(BinaryData::blue_slider_png, BinaryData::blue_slider_pngSize);
        DBG("Blue Slider Loaded: " << (redSlider.isValid() ? "yes" : "no"));

    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {

        // Define bounds
        juce::Rectangle<float> bounds((float)x, (float)y, (float)width, (float)height);

        // Outer white rectangle
        juce::Rectangle outerWhiteRectangle = { bounds.getX(), bounds.getY() + 40.0f, bounds.getWidth(), 15.0f };
        g.setColour(juce::Colours::white);
        g.fillRect(outerWhiteRectangle);

        // Inner black rectangle
        juce::Rectangle innerBlackRectangle = { bounds.getX() + 7.5f, bounds.getY() + 43.75f, bounds.getWidth() - 15.0f, 7.5f };
        g.setColour(juce::Colours::black);
        g.fillRect(innerBlackRectangle);

        // 8-bit squares
        g.fillRect(bounds.getX(), bounds.getY() + 40.0f, 7.5f, 3.75f);                        // top-left
        g.fillRect(bounds.getRight() - 7.5f, bounds.getY() + 40.0f, 7.5f, 3.75f);             // top-right
        g.fillRect(bounds.getX(), bounds.getY() + 51.25f, 7.5f, 3.75f);                       // bottom-left
        g.fillRect(bounds.getRight() - 7.5f, bounds.getY() + 51.25f, 7.5f, 3.75f);            // bottom-right
               
        if (style == juce::Slider::TwoValueHorizontal || style == juce::Slider::ThreeValueHorizontal)
        {

            g.drawImage(redSlider,
                minSliderPos - sliderImageWidth / 2.0f, bounds.getY() + 33.0f,
                sliderImageWidth, sliderImageHeight,
                0, 0, redSlider.getWidth(), redSlider.getHeight());

            g.drawImage(blueSlider,
                maxSliderPos - sliderImageWidth / 2.0f, bounds.getY() + 33.0f,
                sliderImageWidth, sliderImageHeight,
                0, 0, blueSlider.getWidth(), blueSlider.getHeight());

            int minValue = slider.getMinValue();
            int maxValue = slider.getMaxValue();

            String lowMidValue = String(minValue) + " Hz";
            String midHighValue = String(maxValue) + " Hz";

            Rectangle <int> minLabel = Rectangle<int>{
                (int)minSliderPos - 35, (int)bounds.getY() + 5,
                70, 20
            };
            g.setColour(Colours::white);
            g.drawRect(minLabel, 1.0f);

            g.drawFittedText(lowMidValue, minLabel, Justification::centred, 1);

            Rectangle <int> maxLabel = Rectangle<int>{
                (int)minSliderPos - 35, (int)bounds.getY() + 5,
                70, 20
            };
            g.setColour(Colours::white);
            g.drawRect(minLabel, 1.0f);

        }

        else {
            g.drawImage(redSlider,
                bounds.getX() + 5.0f, bounds.getY() + 3.0f,
                sliderImageWidth, sliderImageHeight,
                0, 0, redSlider.getWidth(), redSlider.getHeight());
        }


    }

    /*Slider::SliderLayout getSliderLayout(Slider& slider) override {

        Slider::SliderLayout layout;

        auto bounds = slider.getLocalBounds();

        const int textBoxWidth = 50;
        const int textBoxHeight = 20;

        int sliderPos = (int)slider.getPositionOfValue(slider.getValue());

        layout.textBoxBounds = Rectangle<int>{
            sliderPos - textBoxWidth / 2,
            bounds.getCentreY() - 30,
            textBoxWidth,
            textBoxHeight
        };

        layout.sliderBounds = bounds;
        
        return layout;

    }*/

    int getSliderThumbRadius(juce::Slider& slider) override
    {
        scaleImages();
        return sliderImageWidth;
    }

    void scaleImages() {

        sliderImageWidth = redSlider.getWidth() * scaleFactor;
        sliderImageHeight = redSlider.getHeight() * scaleFactor;

    }

private:

    Image redSlider;
    Image blueSlider;

    const float scaleFactor = 0.25; // How much i'm scaling the image by

    float sliderImageWidth;
    float sliderImageHeight;

};

