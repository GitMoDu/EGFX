// TextAlignmentDemo.h

#ifndef _TEXT_ALIGNMENT_DEMO_h
#define _TEXT_ALIGNMENT_DEMO_h

#include <ArduinoGraphicsDrawer.h>

class TextAlignmentDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DynamicText,
		DrawElementsCount
	};

	static constexpr uint32_t TextScalePeriodMicros = 4000000;
	static constexpr uint32_t TextPositionPeriodMicros = TextScalePeriodMicros * 2;
	static constexpr uint32_t TextColorPeriodMicros = 10000000;

	RgbColor Color{};
	FontStyle DynamicFont;

public:
	TextAlignmentDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::DynamicText:
			DrawRoamingText(drawState);
			break;
		default:
			break;
		}
	}

	void DrawRoamingText(DrawState* drawState)
	{
		const uint8_t y = 1;
		const uint8_t minHeight = TinyFont::FONT_SIZE;
		const uint8_t maxHeight = GetShortestDimension() / 3;
		const uint16_t sizeProgress = drawState->GetProgress<TextScalePeriodMicros>();
		const uint8_t textHeight = minHeight + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(sizeProgress), (uint8_t)(maxHeight - minHeight));
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		const uint8_t sectionCount = 4;
		const uint16_t moveProgress = drawState->GetProgress<TextPositionPeriodMicros>();
		const uint8_t section = (moveProgress / (UINT16_MAX / sectionCount));

		const uint16_t number = drawState->FrameCounter;

		DynamicFont.SetStyle(0, 0, 0, textHeight);
		DynamicFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		DynamicFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		DynamicFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		switch (section)
		{
		case 0:
			NumberRenderer::NumberTopLeft(Frame, DynamicFont, 0, y, number);
			break;
		case 1:
			NumberRenderer::NumberTopRight(Frame, DynamicFont, Frame->GetWidth() - 1, y, number);
			break;
		case 2:
			NumberRenderer::NumberBottomRight(Frame, DynamicFont, Frame->GetWidth() - 1, Frame->GetHeight() - 1, number);
			break;
		case 3:
			NumberRenderer::NumberBottomLeft(Frame, DynamicFont, 0, Frame->GetHeight() - 1, number);
		default:
			break;
		}
	}
};
#endif