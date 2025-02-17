// CloneDemo.h

#ifndef _CLONE_DEMO_h
#define _CLONE_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Sprites.h"

using namespace DemoSprites;

/// <summary>
/// Up to 10 clones can be drawn from a single Sprite.
/// </summary>
/// <typeparam name="Layout"></typeparam>
template<typename Layout>
class CloneDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		PrepareDraw,
		DrawRainDrops,
		DrawElementsCount
	};

	static constexpr pixel_t RangeX = Layout::Width() - RainDropSprite::Width - 1;
	static constexpr pixel_t RangeY = Layout::Height() - RainDropSprite::Height - 1;

	using RainDropLayout = LayoutElement<Layout::X(), Layout::Y(), RangeX, RangeY>;


	static constexpr uint8_t RainDropsPerFrame = 10;

	static constexpr uint32_t RainVariationDuration = 10000000;
	static constexpr uint32_t ColorPeriod = 110000;

private:
	ColorShader<RainDropSprite> RainDrop{};
	rgb_color_t Color{};
	uint8_t droplets = 0;

public:
	CloneDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareDraw:
			PrepareDraw(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::DrawRainDrops:
			DrawRainDrops(frame);
			break;
		default:
			break;
		}
	}

private:
	void PrepareDraw(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progress = ProgressScaler::GetProgress<RainVariationDuration>(frameTime);
		const uint16_t colorProgress = ProgressScaler::GetProgress<ColorPeriod>(frameTime);

		droplets = 1 + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)RainDropsPerFrame);

		if (!frame->IsMonochrome())
		{
			Color = Rgb::Color(ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 4) / 3)), (uint8_t)(UINT8_MAX / 5))
				, ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)(UINT8_MAX / 2))
				, ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 1) / 3)), (uint8_t)UINT8_MAX));
		}
		else
		{

			Color = Rgb::Color(0, 0, UINT8_MAX);
		}

		RainDrop.SetColor(Color);
	}

	void DrawRainDrops(IFrameBuffer* frame)
	{
		switch (droplets)
		{
		case 1:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 2:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 3:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 4:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 5:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 6:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 7:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 8:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 9:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		case 10:
			SpriteRenderer::Draw(frame, &RainDrop,
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()),
				RainDropLayout::X() + random(RainDropLayout::Width()), RainDropLayout::Y() + random(RainDropLayout::Height()));
			break;
		default:
			break;
		}
	}
};

#endif