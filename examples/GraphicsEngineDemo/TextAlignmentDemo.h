// TextAlignmentDemo.h

#ifndef _TEXT_ALIGNMENT_DEMO_h
#define _TEXT_ALIGNMENT_DEMO_h

#include <ArduinoGraphicsCore.h>

class TextAlignmentDemo : public FrameElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DynamicText,
		DrawElementsCount
	};

	static constexpr uint32_t TextScalePeriodMicros = 4000000;
	static constexpr uint32_t TextPositionPeriodMicros = TextScalePeriodMicros * 2;

	RgbColor Color{};
	FontStyle DynamicFont;

public:
	TextAlignmentDemo(IFramePrimitives* frame)
		: FrameElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::DynamicText:
			DrawRoamingText(frameTime);
			break;
		default:
			break;
		}
	}

	void DrawRoamingText(const uint32_t frameTime)
	{
		if (IsDirectDraw)
		{
			Frame->ClearFrame();
		}

		const uint8_t y = 1;
		const uint8_t minHeight = 7;
		const uint8_t maxHeight = ((GetShortestDimension() - 1) / 3) - 1;
		const uint16_t sizeProgress = GetProgress<TextScalePeriodMicros>(frameTime);
		const uint8_t textHeight = minHeight + ScaleProgress(TriangleResponse(sizeProgress), (uint8_t)(maxHeight - minHeight));
		const uint32_t number = frameTime / 1000;

		DynamicFont.SetStyle(UINT8_MAX, UINT8_MAX, 0, textHeight);

		const uint8_t sectionCount = 4;
		const uint16_t moveProgress = GetProgress<TextPositionPeriodMicros>(frameTime);
		const uint8_t section = (moveProgress / (UINT16_MAX / sectionCount));

		switch (section)
		{
		case 0:
			GraphicsText::NumberTopLeft(Frame, 0, y, DynamicFont, (uint32_t)(number));
			break;
		case 1:
			GraphicsText::NumberTopRight(Frame, Width - 1, y, DynamicFont, (const uint32_t)(number));
			break;
		case 2:
			GraphicsText::NumberBottomRight(Frame, Width - 1, Height - 1, DynamicFont, (const uint32_t)(number));
			break;
		case 3:
			GraphicsText::NumberBottomLeft(Frame, 0, Height - 1, DynamicFont, (const uint32_t)(number));
		default:
			break;
		}
	}
};
#endif