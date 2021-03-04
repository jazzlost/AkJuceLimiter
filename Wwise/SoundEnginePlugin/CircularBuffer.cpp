/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 28 Nov 2020 1:12:29pm
    Author:  lyc

  ==============================================================================
*/

#include "CircularBuffer.h"
#include "AkJuceAudioBuffer.h"

CircularBuffer::CircularBuffer()
{
	buffer = new AkJuceAudioBuffer();
	writeIndex = readIndex = delayLength = 0;
}

CircularBuffer::CircularBuffer(int bufferSize, int delayLength, AK::IAkPluginMemAlloc* in_pAllocator)
{
	buffer = new AkJuceAudioBuffer(1, bufferSize, in_pAllocator);
	//buffer->clear();
	writeIndex = delayLength;
	readIndex = 0;
	this->delayLength = delayLength;
}

float CircularBuffer::getData()
{
	return buffer->getSample(0, readIndex);
}

void CircularBuffer::setData(float data)
{
	buffer->setSample(0, writeIndex, data);
}

void CircularBuffer::nextSample()
{
	int bufferLength = buffer->getNumSamples();
	readIndex = ((bufferLength + writeIndex) - delayLength) % bufferLength;
	writeIndex = (writeIndex + 1) % bufferLength;
}
