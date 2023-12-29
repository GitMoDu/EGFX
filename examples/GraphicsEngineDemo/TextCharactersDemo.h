// TextCharactersDemo.h

#ifndef _TEXT_CHARACTERS_DEMO_h
#define _TEXT_CHARACTERS_DEMO_h

#include <ArduinoGraphicsCore.h>

class TextCharactersDemo : public FrameElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		FixedText,
		RoamingText,
		DrawElementsCount
	};

	static constexpr uint32_t TextColorPeriodMicros = 3000000;
	static constexpr uint32_t NumberPeriodMicros = UINT32_MAX;
	static constexpr uint32_t NumberRange = 100000;


	RgbColor Color{};
	FontStyle SmallFont;
	FontStyle DynamicFont;

	const uint8_t SmallTextDimension = 8;
	const uint8_t RemainderDimension;

public:
	TextCharactersDemo(IFramePrimitives* frame)
		: FrameElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
		, RemainderDimension(Height - ((SmallTextDimension + 1) * 3))
	{
		SmallFont.SetStyle(0, 0, 0, SmallTextDimension);
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::FixedText:
			DrawFixedText(frameTime);
			break;
		case (uint8_t)DrawElementsEnum::RoamingText:
			DrawRoamingText(frameTime);
			break;
		default:
			break;
		}
	}

	void DrawFixedText(const uint32_t frameTime)
	{
		const uint16_t colorProgress = GetProgress<TextColorPeriodMicros>(frameTime);

		SmallFont.Color.r = ScaleProgress(TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		SmallFont.Color.g = ScaleProgress(TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		SmallFont.Color.b = ScaleProgress(TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		GraphicsText::TextTopLeft(Frame, 0, 0, SmallFont, F("1234567890"));
		GraphicsText::TextTopLeft(Frame, 0, SmallTextDimension + 1, SmallFont, F("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
		GraphicsText::TextTopLeft(Frame, 0, (SmallTextDimension + 1) * 2, SmallFont, F("abcdefghijklmnopqrstuvwxyz"));
	}

	void DrawRoamingText(const uint32_t frameTime)
	{
		const uint8_t minHeight = 7;
		const uint8_t maxHeight = RemainderDimension - 1;

		const uint16_t sizeProgress = GetProgress<10000000>(frameTime);
		const uint8_t textHeight = minHeight + (((uint32_t)sizeProgress * (maxHeight - minHeight)) / UINT16_MAX);
		DynamicFont.SetStyle(UINT8_MAX, UINT8_MAX, 0, textHeight);

		const uint8_t y = Height - 1 - (RemainderDimension / 2) - (textHeight / 2);
		const uint16_t numberProgress = GetProgress<NumberPeriodMicros>(frameTime);
		const uint32_t number = ((uint32_t)numberProgress * NumberRange) / UINT16_MAX;

		if (IsDirectDraw)
		{
			Color.r = 0;
			Color.g = 0;
			Color.b = 0;
			Frame->RectangleFill(0, Height - 1 - (RemainderDimension / 2) - (maxHeight / 2), Width - 1, Height - 1, Color);
		}

		GraphicsText::NumberTopLeft(Frame, 0, y, DynamicFont, (uint32_t)number);
	}
};
#endif