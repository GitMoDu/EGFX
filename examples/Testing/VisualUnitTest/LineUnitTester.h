// LineUnitTester.h

#ifndef _LINE_UNIT_TESTER_h
#define _LINE_UNIT_TESTER_h

#include <ArduinoGraphicsDrawer.h>

class LineUnitTester : public IFrameDraw
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
	LineUnitTester(const LineTestEnum fixedTest = LineTestEnum::LineTestEnumCount)
		: IFrameDraw()
		, FixedTest(fixedTest)
	{}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		const uint16_t progress = ProgressScaler::GetProgress<ResizeDuration>(frameTime);
		const uint16_t sectionProgress = ProgressScaler::GetProgress<ResizeDuration * (uint32_t)LineTestEnum::LineTestEnumCount>(frameTime);

		const uint8_t move = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)min(frame->GetFrameHeight() - 1, frame->GetFrameWidth() - 1));
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
			frame->Line(Color, 0, 0, frame->GetFrameWidth() - 1, move);
			break;
		case LineTestEnum::RightDown:
			frame->Line(Color, 0, move, frame->GetFrameWidth() - 1, 0);
			break;
		case LineTestEnum::LeftUp:
			frame->Line(Color, frame->GetFrameWidth() - 1, 0, 0, move);
			break;
		case LineTestEnum::LeftDown:
			frame->Line(Color, frame->GetFrameWidth() - 1, move, 0, 0);
			break;
		case LineTestEnum::UpRight:
			frame->Line(Color, 0, 0, move, frame->GetFrameHeight() - 1);
			break;
		case LineTestEnum::UpLeft:
			frame->Line(Color, move, 0, 0, frame->GetFrameHeight() - 1);
			break;
		case LineTestEnum::DownRight:
			frame->Line(Color, 0, frame->GetFrameHeight() - 1, move, 0);
			break;
		case LineTestEnum::DownLeft:
			frame->Line(Color, move, frame->GetFrameHeight() - 1, 0, 0);
			break;
		default:
			break;
		}

		return true;
	}
};
#endif

