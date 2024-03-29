// TextFlowDemo.h

#ifndef _TEXT_FLOW_DEMO_h
#define _TEXT_FLOW_DEMO_h

#include <ArduinoGraphicsDrawer.h>

class TextFlowDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DrawElementsCount
	};

public:
	TextFlowDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(DrawState* drawState) final
	{}
};
#endif