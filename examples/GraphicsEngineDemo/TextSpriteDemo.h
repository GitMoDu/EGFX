// TextSpriteDemo.h

#ifndef _TEXTSPRITEDEMO_h
#define _TEXTSPRITEDEMO_h

#include <ArduinoGraphicsDrawer.h>

/// <summary>
/// Sprite Font Renderer with animated color and shading.
/// </summary>
/// <typeparam name="Layout"></typeparam>
template<typename Layout>
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

	struct TextLayout : Layout
	{
		using Line1 = LayoutElement<Layout::X(), Layout::Y(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using Line2 = LayoutElement<Layout::X(), Line1::Y() + Line1::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using Line3 = LayoutElement<Layout::X(), Line2::Y() + Line2::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using Numbers = LayoutElement<Layout::X(), Line3::Y() + Line2::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
	};

private:
	static constexpr uint32_t TextColorPeriodMicros = 3000000;

private:
	RgbColor Color{};

	VerticalGradientShader<SpriteFont5x5Renderer> TextDrawer{};
	ColorShader<SpriteFont3x5Renderer> NumberDrawer{};

public:
	TextSpriteDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		TextDrawer.SetColor1(RgbColor{ UINT8_MAX / 4 ,UINT8_MAX / 4 ,UINT8_MAX / 4 });
	}

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
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>());
		TextDrawer.SetColor2(Color);

		TextDrawer.TextTopLeft(Frame,
			TextLayout::Line1::X(), TextLayout::Line1::Y(),
			"ABCDEFGHIJ"
		);
	}

	void DrawLine2(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (INT16_MAX / 3));
		TextDrawer.SetColor2(Color);

		TextDrawer.TextTopLeft(Frame,
			TextLayout::Line2::X(), TextLayout::Line2::Y(),
			"KLMNOPQRST"
		);
	}

	void DrawLine3(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (2 * (INT16_MAX / 3)));
		TextDrawer.SetColor2(Color);

		TextDrawer.TextTopLeft(Frame,
			TextLayout::Line3::X(), TextLayout::Line3::Y(),
			"UVWXYZ!?.,"
		);
	}

	void DrawNumbers(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + INT16_MAX);
		NumberDrawer.SetColor(Color);

		NumberDrawer.TextTopLeft(Frame,
			TextLayout::Numbers::X(), TextLayout::Numbers::Y(),
			"0123456789.,+-="
		);
	}

	void SetColorByProgress(const uint16_t colorProgress)
	{
		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);
	}
};
#endif