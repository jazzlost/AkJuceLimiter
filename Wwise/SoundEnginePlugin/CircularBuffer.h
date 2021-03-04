/*
  ==============================================================================

    CircularBuffer.h
    Created: 28 Nov 2020 1:12:29pm
    Author:  lyc

  ==============================================================================
*/

#pragma once
#include <AK/SoundEngine/Common/IAkPlugin.h>
#include "AkJuceAudioBuffer.h"

class CircularBuffer
{
public:
	CircularBuffer();
	CircularBuffer(int bufferSize, int delayLength, AK::IAkPluginMemAlloc* in_pAllocator);
	float getData();
	void setData(float data);
	void nextSample();

private:
	AkJuceAudioBuffer* buffer;
	int writeIndex;
	int readIndex;
	int delayLength;
};
