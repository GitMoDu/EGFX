// TextFlowDemo.h

#ifndef _TEXT_FLOW_DEMO_h
#define _TEXT_FLOW_DEMO_h

#include <ArduinoGraphicsCore.h>

class TextFlowDemo : public FrameElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DrawElementsCount
	};

public:
	TextFlowDemo(IFramePrimitives* frame)
		: FrameElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		default:
			break;
		}
	}
};
#endif