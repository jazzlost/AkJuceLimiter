/*
  ==============================================================================

    CircularBuffer.h
    Created: 28 Nov 2020 1:12:29pm
    Author:  lyc

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class CircularBuffer
{
public:
	CircularBuffer();
	CircularBuffer(int bufferSize, int delayLength);
	float getData();
	void setData(float data);
	void nextSample();

private:
	juce::AudioSampleBuffer buffer;
	int writeIndex;
	int readIndex;
	int delayLength;
};
