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
	ScreenDriverTester()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
		, Color(UINT8_MAX, UINT8_MAX, UINT8_MAX)
	{
	}

	static constexpr uint32_t ResizeDuration = 5000000;

	static constexpr uint8_t ScaleSkip = 2;

protected:
	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		const uint16_t progress = ProgressScaler::GetProgress<ResizeDuration>(frameTime);
		const uint8_t x = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(frame->GetWidth() - 1));
		const uint8_t y = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(frame->GetHeight() - 1));
		uint8_t section = ProgressScaler::ScaleProgress(ProgressScaler::GetProgress<ResizeDuration * 4>(frameTime), (uint8_t)4);

		frame->Line(Color, x, 0, x, frame->GetHeight() - 1);
		frame->Line(Color, 0, y, frame->GetWidth() - 1, y);

		switch (section)
		{
		case 0:
			for (size_t i = 0; i < frame->GetHeight() / ScaleSkip; i++)
			{
				frame->Line(Color, 0, i * ScaleSkip, x, (frame->GetHeight() / 2) - 1);
			}
			break;
		case 1:
			for (size_t i = 0; i < frame->GetHeight() / ScaleSkip; i++)
			{
				frame->Line(Color, frame->GetWidth() - 1, i * ScaleSkip, x, (frame->GetHeight() / 2) - 1);
			}
			break;
		case 2:
			for (size_t i = 0; i < frame->GetWidth() / ScaleSkip; i++)
			{
				frame->Line(Color, (frame->GetWidth() / 2) - 1, y, i * ScaleSkip, frame->GetHeight() - 1);
			}
			break;
		case 3:
			for (size_t i = 0; i < frame->GetWidth() / ScaleSkip; i++)
			{
				frame->Line(Color, i * ScaleSkip, 0, (frame->GetWidth() / 2) - 1, y);
			}
			break;
		default:
			break;
		}
	}
};
#endif

