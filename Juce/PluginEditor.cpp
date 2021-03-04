/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LimitAudioProcessorEditor::LimitAudioProcessorEditor (LimitAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	addAndMakeVisible(tsLabel);
	tsLabel.setText("Threshold", juce::dontSendNotification);

	addAndMakeVisible(atLabel);
	atLabel.setText("AttackTime", juce::dontSendNotification);

	addAndMakeVisible(rtLabel);
	rtLabel.setText("ReleaseTime", juce::dontSendNotification);

	addAndMakeVisible(threshold);
	threshold.setValue(0);
	threshold.setRange(-60.f, 10.f, 0.001);
	threshold.onValueChange = [this] 
	{
		audioProcessor.limiterThresh = std::pow(10, (threshold.getValue() / 20));
	};

	addAndMakeVisible(at);
	at.setRange(0.f, 10.f, 0.001);
	at.onValueChange = [this]
	{
		audioProcessor.attackTime = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / audioProcessor.getSampleRate()) * -2.2f) / at.getValue());
	};

	addAndMakeVisible(rt);
	rt.setRange(0.f, 10.f, 0.001);
	rt.onValueChange = [this]
	{
		audioProcessor.releaseTime = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / audioProcessor.getSampleRate()) * -2.2f) / rt.getValue());
	};
}

LimitAudioProcessorEditor::~LimitAudioProcessorEditor()
{
}

//==============================================================================
void LimitAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void LimitAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	tsLabel.setBounds(50, 30, 200, 50);
	threshold.setBounds(50, 50, 200, 50);
	atLabel.setBounds(50, 130, 200, 50);
	at.setBounds(50, 150, 200, 50);
	rtLabel.setBounds(50, 230, 200, 50);
	rt.setBounds(50, 250, 200, 50);
}
