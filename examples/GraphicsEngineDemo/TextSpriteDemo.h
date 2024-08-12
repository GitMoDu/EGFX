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
		BigLine1,
		BigLine2,
		BigLine3,
		BigNumbers,
		SmallLine1,
		SmallLine2,
		SmallLine3,
		SmallNumbers,
		DrawElementsCount
	};

	struct TextLayout : Layout
	{
		using BigLine1 = LayoutElement<Layout::X(), Layout::Y(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using BigLine2 = LayoutElement<Layout::X(), BigLine1::Y() + BigLine1::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using BigLine3 = LayoutElement<Layout::X(), BigLine2::Y() + BigLine2::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using BigNumbers = LayoutElement<Layout::X(), BigLine3::Y() + BigLine2::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;

		using SmallLine1 = LayoutElement<Layout::X(), BigNumbers::Y() + BigNumbers::Height() + SpriteFont5x5Renderer::FontKerning(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using SmallLine2 = LayoutElement<Layout::X(), SmallLine1::Y() + SmallLine1::Height() + SpriteFont3x5Renderer::FontKerning(), Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
		using SmallLine3 = LayoutElement<Layout::X(), SmallLine2::Y() + SmallLine2::Height() + SpriteFont3x5Renderer::FontKerning(), Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
		using SmallNumbers = LayoutElement<Layout::X(), SmallLine3::Y() + SmallLine2::Height() + SpriteFont3x5Renderer::FontKerning(), Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
	};

private:
	static constexpr uint32_t TextColorPeriodMicros = 3000000;

private:
	RgbColor Color{};

	VerticalGradientShader<SpriteFont5x5Renderer> BigDrawer{};
	ColorShader<SpriteFont3x5Renderer> SmallDrawer{};

public:
	TextSpriteDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		BigDrawer.SetColor1(RgbColor{ UINT8_MAX / 4 ,UINT8_MAX / 4 ,UINT8_MAX / 4 });
	}

	virtual void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::BigLine1:
			DrawBigLine1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::BigLine2:
			DrawBigLine2(drawState);
			break;
		case (uint8_t)DrawElementsEnum::BigLine3:
			DrawBigLine3(drawState);
			break;
		case (uint8_t)DrawElementsEnum::BigNumbers:
			DrawBigNumbers(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallLine1:
			DrawSmallLine1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallLine2:
			DrawSmallLine2(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallLine3:
			DrawSmallLine3(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallNumbers:
			DrawSmallNumbers(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawBigLine1(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>());
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigLine1::X(), TextLayout::BigLine1::Y(),
			F("ABCDEFGHIJ")
		);
	}

	void DrawBigLine2(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (INT16_MAX / 3));
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigLine2::X(), TextLayout::BigLine2::Y(),
			F("KLMNOPQRST")
		);
	}

	void DrawBigLine3(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (2 * (INT16_MAX / 3)));
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigLine3::X(), TextLayout::BigLine3::Y(),
			F("UVWXYZ!?.,")
		);
	}

	void DrawBigNumbers(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + INT16_MAX);
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigNumbers::X(), TextLayout::BigNumbers::Y(),
			F("0123456789.,+-=")
		);
	}

	void DrawSmallLine1(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>());
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine1::X(), TextLayout::SmallLine1::Y(),
			F("ABCDEFGHIJ")
		);
	}

	void DrawSmallLine2(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (INT16_MAX / 3));
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine2::X(), TextLayout::SmallLine2::Y(),
			F("KLMNOPQRST")
		);
	}

	void DrawSmallLine3(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (2 * (INT16_MAX / 3)));
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine3::X(), TextLayout::SmallLine3::Y(),
			F("UVWXYZ!?.,")
		);
	}

	void DrawSmallNumbers(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + INT16_MAX);
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallNumbers::X(), TextLayout::SmallNumbers::Y(),
			F("0123456789.,+-=")
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