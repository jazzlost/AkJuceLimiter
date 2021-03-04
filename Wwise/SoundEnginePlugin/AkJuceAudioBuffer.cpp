#include "AkJuceAudioBuffer.h"

AkJuceAudioBuffer::AkJuceAudioBuffer() : AkAudioBuffer()
{
}

AkJuceAudioBuffer::AkJuceAudioBuffer(int numChannelsToAllocate, int numSamplesToAllocate, AK::IAkPluginMemAlloc* in_pAllocator) : AkAudioBuffer()
{
	AkSampleType* pData = static_cast<AkSampleType*>(AK_PLUGIN_ALLOC(in_pAllocator, sizeof(AkSampleType) * numSamplesToAllocate * numChannelsToAllocate));
	AkChannelConfig chConfig(numChannelsToAllocate, 0);
	AttachInterleavedData(pData, numSamplesToAllocate, numSamplesToAllocate, chConfig);
	ZeroPadToMaxFrames();
}

AkJuceAudioBuffer::~AkJuceAudioBuffer()
{
}

AkSampleType AkJuceAudioBuffer::getSample(int channel, int sampleIndex)
{
	AkSampleType* Samples = GetChannel(channel);

	if (Samples != nullptr)
	{
		if (sampleIndex <= MaxFrames())
		{
			return AkSampleType(Samples[sampleIndex]);
		}
		else
			return -1;
	}
	else
		return -1;
}

void AkJuceAudioBuffer::setSample(int destChannel, int sampleIndex, float newValue)
{
	AkSampleType* Samples = GetChannel(destChannel);

	if (Samples != nullptr)
	{
		if (sampleIndex <= MaxFrames())
		{
			Samples[sampleIndex] = (AkSampleType)newValue;
		}
	}
}

int AkJuceAudioBuffer::getNumSamples()
{
	return MaxFrames();
}

AkSampleType* AkJuceAudioBuffer::getWritePointer(int channel)
{
	return GetChannel(channel);
}

void AkJuceAudioBuffer::clear()
{
	Clear();
}