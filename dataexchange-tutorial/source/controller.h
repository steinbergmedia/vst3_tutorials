//------------------------------------------------------------------------
// Copyright(c) 2023 Steinberg Media Technologies.
//------------------------------------------------------------------------

#pragma once

#include "dataexchange.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Tutorial {

//------------------------------------------------------------------------
//  DataExchangeController
//------------------------------------------------------------------------
class DataExchangeController : public Steinberg::Vst::EditControllerEx1,
                               public Steinberg::Vst::IDataExchangeReceiver
{
public:
//------------------------------------------------------------------------
	// Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/)
	{
		return (Steinberg::Vst::IEditController*)new DataExchangeController;
	}

	// EditController
	Steinberg::tresult PLUGIN_API notify (Steinberg::Vst::IMessage* message) override;

	// IDataExchangeReceiver
	void PLUGIN_API queueOpened (Steinberg::Vst::DataExchangeUserContextID userContextID,
	                             Steinberg::uint32 blockSize,
	                             bool& dispatchOnBackgroundThread) override;
	void PLUGIN_API queueClosed (Steinberg::Vst::DataExchangeUserContextID userContextID) override;
	void PLUGIN_API onDataExchangeBlocksReceived (
	    Steinberg::Vst::DataExchangeUserContextID userContextID, Steinberg::uint32 numBlocks,
	    Steinberg::Vst::DataExchangeBlock* blocks, bool onBackgroundThread) override;
	//---Interface---------
	DEFINE_INTERFACES
		// Here you can add more supported VST3 interfaces
		DEF_INTERFACE (Steinberg::Vst::IDataExchangeReceiver)
	END_DEFINE_INTERFACES (EditController)
	DELEGATE_REFCOUNT (EditController)

//------------------------------------------------------------------------
private:
	Steinberg::Vst::DataExchangeReceiverHandler dataExchange {this};
};

//------------------------------------------------------------------------
} // namespace Tutorial
