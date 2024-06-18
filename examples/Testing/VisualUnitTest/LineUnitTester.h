// LineUnitTester.h

#ifndef _LINE_UNIT_TESTER_h
#define _LINE_UNIT_TESTER_h

#include <ArduinoGraphicsDrawer.h>

class LineUnitTester : public ElementDrawer
{
public:
	enum class LineTestEnum : uint8_t
	{
		RightUp,
		LeftUp,
		LeftDown,
		RightDown,
		UpLeft,
		UpRight,
		DownRight,
		DownLeft,
		LineTestEnumCount
	};

private:
	static constexpr uint32_t ResizeDuration = 3000000;
	static constexpr uint8_t Offset = 5;

	RgbColor Color{ UINT8_MAX, UINT8_MAX, UINT8_MAX };

	LineTestEnum LineTest = LineTestEnum::LineTestEnumCount;
	const LineTestEnum FixedTest = LineTestEnum::LineTestEnumCount;

public:
	LineUnitTester(IFrameBuffer* frame, const LineTestEnum fixedTest = LineTestEnum::LineTestEnumCount)
		: ElementDrawer(frame, 1)
		, FixedTest(fixedTest)
	{}

	virtual void DrawCall(DrawState* drawState) final
	{
		const uint16_t progress = drawState->GetProgress<ResizeDuration>();
		const uint16_t sectionProgress = drawState->GetProgress<ResizeDuration * (uint32_t)LineTestEnum::LineTestEnumCount>();

		const uint8_t move = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)min(Frame->GetHeight() - 1, Frame->GetWidth() - 1));
		LineTestEnum section = (LineTestEnum)ProgressScaler::ScaleProgress(sectionProgress, (uint8_t)LineTestEnum::LineTestEnumCount);

		if (FixedTest == LineTestEnum::LineTestEnumCount)
		{
			if (LineTest != section)
			{
				LineTest = section;

				Serial.print(F("Line Test\t"));
				switch (section)
				{
				case LineTestEnum::RightUp:
					Serial.println(F("RightUp"));
					break;
				case LineTestEnum::RightDown:
					Serial.println(F("RightDown"));
					break;
				case LineTestEnum::LeftUp:
					Serial.println(F("LeftUp"));
					break;
				case LineTestEnum::LeftDown:
					Serial.println(F("LeftDown"));
					break;
				case LineTestEnum::UpRight:
					Serial.println(F("UpRight"));
					break;
				case LineTestEnum::UpLeft:
					Serial.println(F("UpLeft"));
					break;
				case LineTestEnum::DownRight:
					Serial.println(F("DownRight"));
					break;
				case LineTestEnum::DownLeft:
					Serial.println(F("DownLeft"));
					break;
				default:
					break;
				}
			}
		}
		else
		{
			section = FixedTest;
		}

		switch (section)
		{
		case LineTestEnum::RightUp:
			Frame->Line(Color, 0, 0, Frame->GetWidth() - 1, move);
			break;
		case LineTestEnum::RightDown:
			Frame->Line(Color, 0, move, Frame->GetWidth() - 1, 0);
			break;
		case LineTestEnum::LeftUp:
			Frame->Line(Color, Frame->GetWidth() - 1, 0, 0, move);
			break;
		case LineTestEnum::LeftDown:
			Frame->Line(Color, Frame->GetWidth() - 1, move, 0, 0);
			break;
		case LineTestEnum::UpRight:
			Frame->Line(Color, 0, 0, move, Frame->GetHeight() - 1);
			break;
		case LineTestEnum::UpLeft:
			Frame->Line(Color, move, 0, 0, Frame->GetHeight() - 1);
			break;
		case LineTestEnum::DownRight:
			Frame->Line(Color, 0, Frame->GetHeight() - 1, move, 0);
			break;
		case LineTestEnum::DownLeft:
			Frame->Line(Color, move, Frame->GetHeight() - 1, 0, 0);
			break;
		default:
			break;
		}
	}
};
#endif

