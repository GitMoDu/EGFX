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

	static constexpr uint32_t ResizeDuration = 5000000;

	static constexpr uint8_t ScaleSkip = 2;

	virtual void DrawCall(DrawState* drawState) final
	{
		const uint16_t progress = drawState->GetProgress<ResizeDuration>();
		const uint8_t x = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(Frame->GetWidth() - 1));
		const uint8_t y = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(Frame->GetHeight() - 1));
		uint8_t section = ProgressScaler::ScaleProgress(drawState->GetProgress<ResizeDuration * 4>(), (uint8_t)4);

		//section = 2;

		Frame->Line(Color, x, 0, x, Frame->GetHeight() - 1);
		Frame->Line(Color, 0, y, Frame->GetWidth() - 1, y);

		switch (section)
		{
		case 0:
			for (size_t i = 0; i < Frame->GetHeight() / ScaleSkip; i++)
			{
				Frame->Line(Color, 0, i * ScaleSkip, x, (Frame->GetHeight() / 2) - 1);
			}
			//Frame->Line(Color, x, 0, x, Frame->GetHeight() - 1);
			break;
		case 1:
			for (size_t i = 0; i < Frame->GetHeight() / ScaleSkip; i++)
			{
				Frame->Line(Color, Frame->GetWidth() - 1, i * ScaleSkip, x, (Frame->GetHeight() / 2) - 1);
			}
			//Frame->Line(Color, x, 0, x, Frame->GetHeight() - 1);
			break;
		case 2:
			for (size_t i = 0; i < Frame->GetWidth() / ScaleSkip; i++)
			{
				Frame->Line(Color, (Frame->GetWidth() / 2) - 1, y, i * ScaleSkip, Frame->GetHeight() - 1);
			}
			//Frame->Line(Color, 0, y, Frame->GetWidth() - 1, y);
			break;
		case 3:
			for (size_t i = 0; i < Frame->GetWidth() / ScaleSkip; i++)
			{
				Frame->Line(Color, i * ScaleSkip, 0, (Frame->GetWidth() / 2) - 1, y);
			}
			//Frame->Line(Color, 0, y, Frame->GetWidth() - 1, y);
			break;
		default:
			break;
		}
		//switch (drawState->ElementIndex)
		//{
		//case (uint8_t)DrawElementsEnum::FullWhite:

		//	//if (drawState->GetProgress<500000>() < UINT16_MAX / 2)
		//	//{
		//	//	Frame->Fill(Color);
		//	//}

		//	break;
		//default:
		//	break;
		//}
	}
};
#endif

