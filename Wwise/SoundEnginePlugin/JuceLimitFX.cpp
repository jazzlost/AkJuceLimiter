/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2020 Audiokinetic Inc.
*******************************************************************************/

#include "JuceLimitFX.h"
#include "../JuceLimitConfig.h"
#include "AkJuceAudioBuffer.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateJuceLimitFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
	return AK_PLUGIN_NEW(in_pAllocator, JuceLimitFX());
}

AK::IAkPluginParam* CreateJuceLimitFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
	return AK_PLUGIN_NEW(in_pAllocator, JuceLimitFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(JuceLimitFX, AkPluginTypeEffect, JuceLimitConfig::CompanyID, JuceLimitConfig::PluginID)

JuceLimitFX::JuceLimitFX()
	: m_pParams(nullptr)
	, m_pAllocator(nullptr)
	, m_pContext(nullptr)
{
}

JuceLimitFX::~JuceLimitFX()
{
}

AKRESULT JuceLimitFX::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkEffectPluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
	m_pParams = (JuceLimitFXParams*)in_pParams;
	m_pAllocator = in_pAllocator;
	m_pContext = in_pContext;

	limiterThresh = m_pParams->RTPC.fThreshold;
	attackTime = m_pParams->RTPC.fAttack;
	releaseTime = m_pParams->RTPC.fRelease;
	
	gain = 1.0f;
	xPeak = 0.f;

	for (int i = 0; i < in_rFormat.GetNumChannels(); i++)
	{
		allBuffers.push_back(new CircularBuffer(10, 1, in_pAllocator));
	}

	return AK_Success;
}

AKRESULT JuceLimitFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
	AK_PLUGIN_DELETE(in_pAllocator, this);
	allBuffers.clear();
	return AK_Success;
}

AKRESULT JuceLimitFX::Reset()
{
	return AK_Success;
}

AKRESULT JuceLimitFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
	out_rPluginInfo.eType = AkPluginTypeEffect;
	out_rPluginInfo.bIsInPlace = true;
	out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
	return AK_Success;
}

void JuceLimitFX::Execute(AkAudioBuffer* io_pBuffer)
{
	AkJuceAudioBuffer* buffer = static_cast<AkJuceAudioBuffer*>(io_pBuffer);
	const AkUInt32 uNumChannels = buffer->NumChannels();

	AkUInt16 uFramesProcessed;
	float coeff;
	for (AkUInt32 i = 0; i < uNumChannels; ++i)
	{
		AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT)io_pBuffer->GetChannel(i);
		uFramesProcessed = 0;
		coeff = 0.f;
		while (uFramesProcessed < buffer->uValidFrames)
		{
			// Execute DSP in-place here

			CircularBuffer* delayBuffer = allBuffers[i];

			float sample = pBuf[uFramesProcessed];

			float amplitude = abs(sample);

			if (amplitude > xPeak)
			{
				coeff = attackTime;
			}
			else
			{
				coeff = releaseTime;
			}

			xPeak = (1 - coeff) * xPeak + coeff * amplitude;

			float filter = fmin(1.0f, limiterThresh / xPeak);

			if (gain > filter)
			{
				coeff = attackTime;
			}
			else
			{
				coeff = releaseTime;
			}

			gain = (1 - coeff) * gain + coeff * filter;

			float limitedSample = gain * delayBuffer->getData();
			delayBuffer->setData(sample);
			delayBuffer->nextSample();

			pBuf[uFramesProcessed] = limitedSample;
			++uFramesProcessed;
		}
	}
}

AKRESULT JuceLimitFX::TimeSkip(AkUInt32 in_uFrames)
{
	return AK_DataReady;
}