// IFrameEngine.h

#ifndef _I_FRAME_ENGINE_h
#define _I_FRAME_ENGINE_h

#include "IFrameDraw.h"

class IFrameEngine
{
public:
	struct EngineStatusStruct
	{
		uint32_t ClearDuration = 0;
		uint32_t RenderDuration = 0;
		uint32_t PushDuration = 0;
		uint32_t LongestDrawCall = 0;
		uint32_t FrameDuration = 0;
		uint16_t FrameCounter = 0;

		void CopyTo(EngineStatusStruct& target)
		{
			target.ClearDuration = ClearDuration;
			target.RenderDuration = RenderDuration;
			target.PushDuration = PushDuration;
			target.LongestDrawCall = LongestDrawCall;
			target.FrameDuration = FrameDuration;
			target.FrameCounter = FrameCounter;
		}
	};

public:
	virtual const bool Start() { return false; }
	virtual void Stop() {}
	virtual void SetDrawer(IFrameDraw* drawer) {}

public:
	virtual void GetEngineStatus(EngineStatusStruct& status) {}
};
#endif