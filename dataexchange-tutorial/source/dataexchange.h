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
static constexpr Steinberg::Vst::DataExchangeBlock InvalidDataExchangeBlock = {
    nullptr, 0, Steinberg::Vst::InvalidDataExchangeBlockID};

//------------------------------------------------------------------------
inline DataBlock* toDataBlock (const Steinberg::Vst::DataExchangeBlock& block)
{
	return reinterpret_cast<DataBlock*> (block.data);
}

//------------------------------------------------------------------------
} // Tutorial
