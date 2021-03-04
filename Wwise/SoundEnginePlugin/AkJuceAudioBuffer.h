#pragma once

#include<AK/SoundEngine/Common/AkCommonDefs.h>
#include "AK/SoundEngine/Common/IAkPluginMemAlloc.h"


class AkJuceAudioBuffer : public AkAudioBuffer
{
public:

	AkJuceAudioBuffer();
	AkJuceAudioBuffer(int numChannelsToAllocate, int numSamplesToAllocate, AK::IAkPluginMemAlloc* in_pAllocator);
	
	~AkJuceAudioBuffer();

	AkSampleType getSample(int channel, int sampleIndex);
	
	void setSample(int destChannel, int sampleIndex, float newValue);
	
	int getNumSamples();

	AkSampleType* getWritePointer(int channel);

	void clear();
};