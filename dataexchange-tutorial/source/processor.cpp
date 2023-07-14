//------------------------------------------------------------------------
// Copyright(c) 2023 Steinberg Media Technologies.
//------------------------------------------------------------------------

#include "cids.h"
#include "processor.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace Tutorial {
//------------------------------------------------------------------------
// DataExchangeProcessor
//------------------------------------------------------------------------
DataExchangeProcessor::DataExchangeProcessor ()
{
	setControllerClass (kDataExchangeControllerUID);
}

//------------------------------------------------------------------------
DataExchangeProcessor::~DataExchangeProcessor () = default;

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::connect (Vst::IConnectionPoint* other)
{
	auto result = Vst::AudioEffect::connect (other);
	if (result == kResultTrue)
	{
		auto configCallback = [this] (Vst::DataExchangeHandler::Config& config,
		                              const Vst::ProcessSetup& setup) {
			Vst::SpeakerArrangement arr;
			getBusArrangement (Vst::BusDirections::kOutput, 0, arr);
			numChannels = static_cast<uint16_t> (Vst::SpeakerArr::getChannelCount (arr));
			auto sampleSize = sizeof (float);

			config.blockSize = setup.sampleRate * numChannels * sampleSize + sizeof (DataBlock);
			config.numBlocks = 2;
			config.alignment = 32;
			config.userContextID = 0;
			return true;
		};

		dataExchange = std::make_unique<Vst::DataExchangeHandler> (this, configCallback);
		dataExchange->onConnect (other, getHostContext ());
	}
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::disconnect (Vst::IConnectionPoint* other)
{
	if (dataExchange)
	{
		dataExchange->onDisconnect (other);
		dataExchange.reset ();
	}
	return AudioEffect::disconnect (other);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::setActive (TBool state)
{
	if (state)
		dataExchange->onActivate (processSetup);
	else
		dataExchange->onDeactivate ();
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;
	return kResultFalse;
}

//------------------------------------------------------------------------
DataBlock* DataExchangeProcessor::getDataBlock () const
{
	if (currentExchangeBlock.blockID != Vst::InvalidDataExchangeBlockID)
		return toDataBlock (currentExchangeBlock);
	return nullptr;
}

//------------------------------------------------------------------------
bool DataExchangeProcessor::aquireNewExchangeBlock ()
{
	if (currentExchangeBlock.blockID != Vst::InvalidDataExchangeBlockID)
		return false;
	currentExchangeBlock = dataExchange->getCurrentOrNewBlock ();
	if (currentExchangeBlock.blockID == Vst::InvalidDataExchangeBlockID)
		return false;

	auto block = getDataBlock ();
	block->sampleRate = static_cast<uint32_t> (processSetup.sampleRate);
	block->numChannels = numChannels;
	block->sampleSize = sizeof (float);
	block->numSamples = 0;

	return currentExchangeBlock.blockID != Vst::InvalidDataExchangeBlockID;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DataExchangeProcessor::process (Vst::ProcessData& processData)
{
	if (processData.numSamples <= 0)
		return kResultTrue;

	if (currentExchangeBlock.blockID == Vst::InvalidDataExchangeBlockID)
		aquireNewExchangeBlock ();

	auto input = processData.inputs[0];
	auto output = processData.outputs[0];

	if (currentExchangeBlock.blockID != Vst::InvalidDataExchangeBlockID)
	{
		auto block = getDataBlock ();

		auto numSamples = static_cast<uint32> (processData.numSamples);
		while (numSamples > 0)
		{
			uint32 numSamplesFreeInBlock = block->sampleRate - block->numSamples;
			uint32 numSamplesToCopy = std::min<uint32> (numSamplesFreeInBlock, numSamples);
			for (auto channel = 0; channel < input.numChannels; ++channel)
			{
				auto blockChannelData = &block->samples[0] + block->numSamples;
				auto inputChannel =
				    input.channelBuffers32[channel] + (processData.numSamples - numSamples);
				memcpy (blockChannelData, inputChannel, numSamplesToCopy * sizeof (float));
			}
			block->numSamples += numSamplesToCopy;
			if (block->numSamples == block->sampleRate)
			{
				dataExchange->sendCurrentBlock ();
				currentExchangeBlock = InvalidDataExchangeBlock;
				if (!aquireNewExchangeBlock ())
					break;
				block = getDataBlock ();
			}
			numSamples -= numSamplesToCopy;
		}
	}
	for (auto channel = 0; channel < input.numChannels; ++channel)
	{
		if (output.channelBuffers32[channel] != input.channelBuffers32[channel])
		{
			memcpy (output.channelBuffers32[channel], input.channelBuffers32[channel],
			        processData.numSamples * sizeof (float));
		}
		output.silenceFlags = input.silenceFlags;
	}

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Tutorial
