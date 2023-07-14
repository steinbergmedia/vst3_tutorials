//------------------------------------------------------------------------
// Copyright(c) 2023 Steinberg Media Technologies.
//------------------------------------------------------------------------

#pragma once

#include "dataexchange.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Tutorial {

//------------------------------------------------------------------------
//  DataExchangeProcessor
//------------------------------------------------------------------------
class DataExchangeProcessor : public Steinberg::Vst::AudioEffect
{
public:
	using tresult = Steinberg::tresult;

	DataExchangeProcessor ();
	~DataExchangeProcessor () override;

	static Steinberg::FUnknown* createInstance (void* /*context*/)
	{
		return (Steinberg::Vst::IAudioProcessor*)new DataExchangeProcessor;
	}

	tresult PLUGIN_API initialize (Steinberg::FUnknown* context) override;
	tresult PLUGIN_API connect (Steinberg::Vst::IConnectionPoint* other) override;
	tresult PLUGIN_API disconnect (Steinberg::Vst::IConnectionPoint* other) override;
	tresult PLUGIN_API setActive (Steinberg::TBool state) override;
	tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) override;
	tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) override;
//------------------------------------------------------------------------
protected:
	DataBlock* getDataBlock () const;
	bool aquireNewExchangeBlock ();

	std::unique_ptr<Steinberg::Vst::DataExchangeHandler> dataExchange;
	Steinberg::Vst::DataExchangeBlock currentExchangeBlock {InvalidDataExchangeBlock};
	uint16_t numChannels {0};
};

//------------------------------------------------------------------------
} // namespace Tutorial
