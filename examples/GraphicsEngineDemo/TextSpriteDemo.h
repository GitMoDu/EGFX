// TextSpriteDemo.h

#ifndef _TEXTSPRITEDEMO_h
#define _TEXTSPRITEDEMO_h

#include <ArduinoGraphicsDrawer.h>

class TextSpriteDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		Line1,
		Line2,
		Line3,
		Numbers,
		DrawElementsCount
	};

	static constexpr uint32_t TextColorPeriodMicros = 3000000;

	RgbColor Color{};

	SpriteFont5x5Renderer FontRenderer{};


public:
	TextSpriteDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	/// <summary>
	/// ElementIndex can be used to separate draw calls, avoiding hogging the co-operative scheduler.
	/// </summary>
	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::Line1:
			DrawLine1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::Line2:
			DrawLine2(drawState);
			break;
		case (uint8_t)DrawElementsEnum::Line3:
			DrawLine3(drawState);
			break;
		case (uint8_t)DrawElementsEnum::Numbers:
			DrawNumbers(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawLine1(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>();

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		FontRenderer.TextTopLeft(Frame, Color,
			0, 0,
			"ABCDEFGHIJ"
		);
	}

	void DrawLine2(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>() + INT16_MAX / 3;

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		FontRenderer.TextTopLeft(Frame, Color,
			0, FontRenderer.FontHeight + 1,
			"KLMNOPQRST"
		);
	}

	void DrawLine3(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>() + INT16_MAX / 3;

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		FontRenderer.TextTopLeft(Frame, Color,
			0, (FontRenderer.FontHeight + 1) * 2,
			"UVWXYZ!?"
		);
	}

	void DrawNumbers(DrawState* drawState)
	{
		const uint16_t colorProgress = drawState->GetProgress<TextColorPeriodMicros>() + (uint8_t)(((int32_t)INT16_MAX * 2) / 3);

		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);

		FontRenderer.TextTopLeft(Frame, Color,
			0, (FontRenderer.FontHeight + 1) * 4,
			"0123456789"
		);
	}
};

#endif

