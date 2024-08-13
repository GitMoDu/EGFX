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
		TinyLine1,
		TinyLine2,
		MicroLine,
		DrawElementsCount
	};

	struct TextLayout : Layout
	{
		static constexpr uint8_t VerticalKerning = 1;

		using BigLine1 = LayoutElement<Layout::X(), Layout::Y(), Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using BigLine2 = LayoutElement<Layout::X(), BigLine1::Y() + BigLine1::Height() + VerticalKerning, Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;
		using BigLine3 = LayoutElement<Layout::X(), BigLine2::Y() + BigLine2::Height() + VerticalKerning, Layout::Width(), SpriteFont5x5Renderer::FontHeight()>;

		using SmallLine1 = LayoutElement<Layout::X(), BigLine3::Y() + BigLine3::Height() + VerticalKerning, Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
		using SmallLine2 = LayoutElement<Layout::X(), SmallLine1::Y() + SmallLine1::Height() + VerticalKerning, Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;
		using SmallLine3 = LayoutElement<Layout::X(), SmallLine2::Y() + SmallLine2::Height() + VerticalKerning, Layout::Width(), SpriteFont3x5Renderer::FontHeight()>;

		using TinyLine1 = LayoutElement<Layout::X(), SmallLine3::Y() + SmallLine3::Height() + VerticalKerning, Layout::Width(), SpriteFont2x5Renderer::FontHeight()>;
		using TinyLine2 = LayoutElement<Layout::X(), TinyLine1::Y() + TinyLine1::Height() + VerticalKerning, Layout::Width(), SpriteFont2x5Renderer::FontHeight()>;

		using MicroLine = LayoutElement<Layout::X(), TinyLine2::Y() + TinyLine2::Height() + VerticalKerning, Layout::Width(), SpriteFont1x5Renderer::FontHeight()>;
	};

private:
	static constexpr uint32_t TextColorPeriodMicros = 3000000;

private:
	RgbColor Color{};

	VerticalGradientShader<SpriteFont5x5Renderer> BigDrawer{};
	ColorShader<SpriteFont3x5Renderer> SmallDrawer{};
	SpriteFont2x5Renderer TinyDrawer{};
	SpriteFont1x5Renderer MicroDrawer{};

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
		case (uint8_t)DrawElementsEnum::SmallLine1:
			DrawSmallLine1(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallLine2:
			DrawSmallLine2(drawState);
			break;
		case (uint8_t)DrawElementsEnum::SmallLine3:
			DrawSmallLine3(drawState);
			break;
		case (uint8_t)DrawElementsEnum::TinyLine1:
			if (Frame->GetColorDepth() >= 8)
			{
				DrawTinyLine1();
			}
			break;
		case (uint8_t)DrawElementsEnum::TinyLine2:
			if (Frame->GetColorDepth() >= 8)
			{
				DrawTinyLine2();
			}
			break;
		case (uint8_t)DrawElementsEnum::MicroLine:
			if (Frame->GetColorDepth() >= 8)
			{
				DrawMicroLine();
			}
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
			F("ABCDEFGHIJKLM")
		);
	}

	void DrawBigLine2(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (INT16_MAX / 3));
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigLine2::X(), TextLayout::BigLine2::Y(),
			F("NOPQRSTUVWXYZ")
		);
	}

	void DrawBigLine3(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (2 * (INT16_MAX / 3)));
		BigDrawer.SetColor2(Color);

		BigDrawer.TextTopLeft(Frame,
			TextLayout::BigLine3::X(), TextLayout::BigLine3::Y(),
			F("0123456789!?.,+-=")
		);
	}

	void DrawSmallLine1(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>());
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine1::X(), TextLayout::SmallLine1::Y(),
			F("ABCDEFGHIJKLM")
		);
	}

	void DrawSmallLine2(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (INT16_MAX / 3));
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine2::X(), TextLayout::SmallLine2::Y(),
			F("NOPQRSTUVWXYZ")
		);
	}

	void DrawSmallLine3(DrawState* drawState)
	{
		SetColorByProgress(drawState->GetProgress<TextColorPeriodMicros>() + (2 * (INT16_MAX / 3)));
		SmallDrawer.SetColor(Color);

		SmallDrawer.TextTopLeft(Frame,
			TextLayout::SmallLine3::X(), TextLayout::SmallLine3::Y(),
			F("0123456789!?.,+-=")
		);
	}

	void DrawTinyLine1()
	{
		if (TextLayout::TinyLine1::Height() > 0
			&& TextLayout::TinyLine1::Y() < Layout::Height())
		{
			TinyDrawer.TextTopLeft(Frame,
				TextLayout::TinyLine1::X(), TextLayout::TinyLine1::Y(),
				F("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
			);
		}
	}

	void DrawTinyLine2()
	{
		if (TextLayout::TinyLine1::Height() > 0
			&& TextLayout::TinyLine1::Y() < Layout::Height())
		{
			TinyDrawer.TextTopLeft(Frame,
				TextLayout::TinyLine2::X(), TextLayout::TinyLine2::Y(),
				F("0123456789!?.,+-=")
			);
		}
	}

	void DrawMicroLine()
	{
		if (TextLayout::MicroLine::Height() > 0
			&& TextLayout::MicroLine::Y() < Layout::Height())
		{
			MicroDrawer.TextTopLeft(Frame,
				TextLayout::MicroLine::X(), TextLayout::MicroLine::Y(),
				F("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?.,+-=")
			);
		}
	}

	void SetColorByProgress(const uint16_t colorProgress)
	{
		Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
		Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
		Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);
	}
};
#endif