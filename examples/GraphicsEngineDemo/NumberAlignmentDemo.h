// NumberAlignmentDemo.h

#ifndef _NUMBER_ALIGNMENT_DEMO_h
#define _NUMBER_ALIGNMENT_DEMO_h

#include <ArduinoGraphicsDrawer.h>

template<typename Layout>
class NumberAlignmentDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		DynamicText,
		DrawElementsCount
	};

	static constexpr uint32_t TextScalePeriodMicros = 3000000;
	static constexpr uint32_t TextColorPeriodMicros = 10000000;
	static constexpr uint32_t NumberFactor = 333000;
	static constexpr uint8_t CornerCount = 4;

	RgbColor Color{};
	TemplateFontStyle<FontStyle::WIDTH_RATIO_DEFAULT> DynamicFont;

public:
	NumberAlignmentDemo(IFrameBuffer* frame)
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
		static constexpr uint8_t minHeight = FontStyle::FONT_SIZE_MIN;
		static constexpr uint8_t maxHeight = (Layout::Height() / 2) - 2;

		const uint8_t y = 0;
		const uint16_t sizeProgress = drawState->GetProgress<TextScalePeriodMicros>();
		const uint8_t textHeight = minHeight + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(sizeProgress), (uint8_t)(maxHeight - minHeight + 1));
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();


		const uint16_t sectionProgress = drawState->GetProgress<TextScalePeriodMicros * CornerCount>();
		const uint8_t section = ProgressScaler::ScaleProgress(sectionProgress, (uint8_t)(CornerCount));

		const uint32_t number = drawState->FrameTime / NumberFactor;

		DynamicFont.SetSize(textHeight);
		DynamicFont.Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		DynamicFont.Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		DynamicFont.Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		switch (section)
		{
		case 0:
			NumberRenderer::NumberTopLeft(Frame, DynamicFont, Layout::X(), Layout::Y() + y, number);
			break;
		case 1:
			NumberRenderer::NumberTopRight(Frame, DynamicFont, Layout::X() + Layout::Width() - 1, Layout::Y() + y, number);
			break;
		case 2:
			NumberRenderer::NumberBottomRight(Frame, DynamicFont, Layout::X() + Layout::Width() - 1, Layout::Y() + Layout::Height() - 1, number);
			break;
		case 3:
			NumberRenderer::NumberBottomLeft(Frame, DynamicFont, Layout::X(), Layout::Y() + Layout::Height() - 1, number);
			break;
		default:
			break;
		}
	}
};
#endif