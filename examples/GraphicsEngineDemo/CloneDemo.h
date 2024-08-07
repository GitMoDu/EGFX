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

	static constexpr uint8_t rangeY = (uint8_t)(Layout::Height() - RainDropSprite::Height - 1);

	static constexpr uint8_t RainDropsPerFrame = 10;

	static constexpr uint32_t RainVariationDuration = 10000000;
	static constexpr uint32_t ColorPeriod = 110000;

private:
	ColorShader<RainDropSprite> RainDrop{};
	RgbColor Color{};
	uint8_t droplets = 0;

public:
	CloneDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareDraw:
			PrepareDraw(drawState);
			break;
		case (uint8_t)DrawElementsEnum::DrawRainDrops:
			DrawRainDrops();
			break;
		default:
			break;
		}
	}

private:
	void PrepareDraw(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<RainVariationDuration>();
		const uint16_t colorProgress = drawState->GetProgress<ColorPeriod>();

		droplets = 1 + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)RainDropsPerFrame);

		if (Frame->GetColorDepth() > 1)
		{
			Color.r = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 4) / 3)), (uint8_t)(UINT8_MAX / 5));
			Color.g = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 2) / 3)), (uint8_t)(UINT8_MAX / 2));
			Color.b = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(colorProgress + ((UINT16_MAX * 1) / 3)), (uint8_t)UINT8_MAX);
		}
		else
		{
			Color.r = 0;
			Color.g = 0;
			Color.b = UINT8_MAX;
		}

		RainDrop.SetColor(Color);
	}

	void DrawRainDrops()
	{
		switch (droplets)
		{
		case 1:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 2:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 3:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 4:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 5:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 6:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 7:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 8:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 9:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		case 10:
			SpriteRenderer::Draw(Frame, &RainDrop,
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY),
				Layout::X() + random(Layout::Width()), Layout::Y() + random(rangeY));
			break;
		default:
			break;
		}
	}
};

#endif