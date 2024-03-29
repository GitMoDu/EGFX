// ScreenDriverTester.h

#ifndef _SCREEN_DRIVER_TESTER_h
#define _SCREEN_DRIVER_TESTER_h

#include <ArduinoGraphicsDrawer.h>

class ScreenDriverTester : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		FullWhite,
		DrawElementsCount
	};

	RgbColor Color;

public:
	ScreenDriverTester(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
		, Color(UINT8_MAX, UINT8_MAX, UINT8_MAX)
	{}

	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::FullWhite:
			if (drawState->GetProgress<500000>() < UINT16_MAX / 2)
			{
				Frame->Fill(Color);
			}

			break;
		default:
			break;
		}
	}
};
#endif

