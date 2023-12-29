// IFrameSink.h

#ifndef _I_FRAME_SINK_h
#define _I_FRAME_SINK_h

#include "IFrame.h"

class IFrameSink : public virtual IFrame
{
public:
	IFrameSink() : IFrame() {}

public:
	virtual const bool CanPushFrame() { return false; }

	virtual const bool PushFrame(const uint8_t* frameBuffer) { return false; }

	virtual const bool Start() { return false; }

	virtual void Stop() {}
};
#endif