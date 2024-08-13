// BitmaskDemo.h

#ifndef _BITMASK_DEMO_h
#define _BITMASK_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Sprites.h"

using namespace DemoSprites;

/// <summary>
/// Bitmask with translation and color shading animation.
/// Draws the bitmask line by line on each draw call.
/// </summary>
/// <typeparam name="Layout"></typeparam>
template<typename Layout>
class BitmaskDemo : public ElementDrawer
{
private:
	static constexpr uint32_t TranslationXDuration = 11000000;
	static constexpr uint32_t TranslationYDuration = 13000000;
	static constexpr uint32_t ColorDuration = 13000000;

	struct BitmapLayout : public Layout
	{
		static constexpr uint8_t BitmapDrawSteps()
		{
			return DogeBitSprite::Height / 2;
		}

		static constexpr uint8_t UsableX()
		{
			return Layout::Width() - DogeBitSprite::Width - 1;
		}

		static constexpr uint8_t UsableY()
		{
			return Layout::Height() - DogeBitSprite::Height - 1;
		}

		static constexpr uint8_t SectionHeight()
		{
			return DogeBitSprite::Height / BitmapDrawSteps();
		}
	};

	enum class DrawElementsEnum : uint8_t
	{
		PrepareBitmap = 0,
		DrawBitmapN = 1,
		DrawElementsCount = (uint8_t)DrawBitmapN + BitmapLayout::BitmapDrawSteps()
	};

	ColorShader<DogeBitSprite> DogeBit{};

	RgbColor Color{};

	uint8_t x = 0;
	uint8_t y = 0;

public:
	BitmaskDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareBitmap:
			PrepareBitmask(drawState);
			break;
		default:
			if (drawState->ElementIndex >= (uint8_t)DrawElementsEnum::DrawBitmapN
				&& drawState->ElementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN < BitmapLayout::BitmapDrawSteps())
			{
				DrawBitmaskSection(drawState->ElementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN);
			}
			break;
		}
	}

private:
	void PrepareBitmask(DrawState* drawState)
	{
		const uint16_t progressX = ProgressScaler::TriangleResponse(drawState->GetProgress<TranslationXDuration>());
		const uint16_t progressY = ProgressScaler::TriangleResponse(drawState->GetProgress<TranslationYDuration>());

		x = Layout::X() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressX), BitmapLayout::UsableX());
		y = Layout::Y() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressY), BitmapLayout::UsableY());

		const uint16_t colorProgress = drawState->GetProgress<ColorDuration>();

		if (Frame->GetColorDepth() > 1)
		{
			Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX);
			Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX);
			Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX);
		}
		else
		{
			Color.r = UINT8_MAX;
			Color.g = UINT8_MAX;
			Color.b = UINT8_MAX;
		}

		DogeBit.SetColor(Color);
	}

	void DrawBitmaskSection(uint8_t index)
	{
		SpriteRenderer::DrawPartial(
			Frame, &DogeBit,
			x,
			y,
			0,
			index * BitmapLayout::SectionHeight(),
			DogeBitSprite::Width,
			BitmapLayout::SectionHeight());
	}
};
#endif