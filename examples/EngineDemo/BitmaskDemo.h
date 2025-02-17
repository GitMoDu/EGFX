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
		PrepareBitmap,
		DrawBitmapN,
		DrawElementsCount = (uint8_t)DrawBitmapN + BitmapLayout::BitmapDrawSteps()
	};

	ColorShader<DogeBitSprite> DogeBit{};

	rgb_color_t Color{};

	uint8_t x = 0;
	uint8_t y = 0;

public:
	BitmaskDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareBitmap:
			PrepareBitmask(frame, frameTime);
			break;
		default:
			if (elementIndex >= (uint8_t)DrawElementsEnum::DrawBitmapN
				&& elementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN < BitmapLayout::BitmapDrawSteps())
			{
				DrawBitmaskSection(frame, elementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN);
			}
			break;
		}
	}

private:
	void PrepareBitmask(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progressX = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationXDuration>(frameTime));
		const uint16_t progressY = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationYDuration>(frameTime));

		x = Layout::X() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressX), BitmapLayout::UsableX());
		y = Layout::Y() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressY), BitmapLayout::UsableY());

		if (frame->GetColorDepth() > 1)
		{
			const uint16_t colorProgress = ProgressScaler::GetProgress<ColorDuration>(frameTime);

			Color = Rgb::Color(ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress), (uint8_t)UINT8_MAX)
				, ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + (UINT16_MAX / 3)), (uint8_t)UINT8_MAX)
				, ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)UINT8_MAX));
		}
		else
		{
			Color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);
		}

		DogeBit.SetColor(Color);
	}

	void DrawBitmaskSection(IFrameBuffer* frame, const uint8_t index)
	{
		SpriteRenderer::DrawPartial(
			frame, &DogeBit,
			x,
			y,
			0,
			index * BitmapLayout::SectionHeight(),
			DogeBitSprite::Width,
			BitmapLayout::SectionHeight());
	}
};
#endif