/*
  ==============================================================================

    UserInterface.h
    Created: 18 May 2021 2:40:09pm
    Author:  A.O

  ==============================================================================
*/

#pragma once

class UserInterface : public juce::LookAndFeel_V4
{
    
    
public:
    UserInterface()
    {
        
    }
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override 
    {
        
         
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto fill    = slider.findColour (juce::Slider::rotarySliderFillColourId);

        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);

        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;
        
        auto centreX = x + width / 2.0;
        auto centreY = y + height / 2.0;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        
        juce::Rectangle<float> newBounds (rx, ry, radius*2, radius*2);
        
    
        
        
        g.setColour(juce::Colours::transparentBlack); 
        //g.drawRect(newBounds);
        g.fillEllipse(newBounds);
        
        
        g.setColour(juce::Colours::palegoldenrod);
       
        juce::Path dialTick;
        dialTick.addRectangle(0,-radius,0.5f,radius);
        
        //juce::DropShadow shadow(juce::Colours::black.withAlpha(0.2f),width * 0.1f, juce::Point<int>(width * 0.05f, height * 0.05f));
        
        g.fillPath(dialTick, juce::AffineTransform::rotation(toAngle).translated(centreX, centreY));
        
        g.setColour(juce::Colours::white);
        g.drawEllipse(rx, ry, radius*2, radius*2, 1.0f);
    }
    };
        
        
        /*

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);

        
        g.setColour (outline);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);

            g.setColour (juce::Colours::red);
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
          
        auto thumbWidth = lineW * 2.0f;
        juce::Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - juce::MathConstants<float>::halfPi),
                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - juce::MathConstants<float>::halfPi));

        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
    }*/

