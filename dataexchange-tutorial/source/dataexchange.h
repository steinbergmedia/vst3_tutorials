#pragma once

#include "public.sdk/source/vst/utility/dataexchange.h"
#include <cstdint>

//------------------------------------------------------------------------
namespace Tutorial {

//------------------------------------------------------------------------
struct DataBlock
{
	uint32_t sampleRate;
	uint16_t sampleSize;
	uint16_t numChannels;
	uint32_t numSamples;
	float samples[0];
};

//------------------------------------------------------------------------
inline DataBlock* toDataBlock (const Steinberg::Vst::DataExchangeBlock& block)
{
	if (block.blockID != Steinberg::Vst::InvalidDataExchangeBlockID)
		return reinterpret_cast<DataBlock*> (block.data);
	return nullptr;
}

//------------------------------------------------------------------------
} // Tutorial
