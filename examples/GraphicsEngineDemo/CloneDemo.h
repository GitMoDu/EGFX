// CloneDemo.h

#ifndef _CLONE_DEMO_h
#define _CLONE_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Sprites.h"

using namespace DemoSprites;

class CloneDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		RainDrops,
		DrawElementsCount
	};

	/// <summary>
	/// Up to 10 clones can be drawn from a single Sprite.
	/// </summary>
	static constexpr uint8_t RainDropsPerFrame = 10;

	static constexpr uint32_t RainVariationDuration = 10000000;

private:
	RainDropSprite RainDrop{};

public:
	CloneDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::RainDrops:
			DrawRainDrops(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawRainDrops(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<RainVariationDuration>();
		const uint8_t droplets = 1 + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)RainDropsPerFrame);
		const uint8_t rangeY = (uint8_t)(Frame->GetHeight() - RainDrop.Height - 1);

		switch (droplets)
		{
		case 1:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 2:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 3:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 4:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 5:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 6:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 7:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 8:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 9:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		case 10:
			SpriteRenderer::Draw(Frame, &RainDrop,
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY),
				random(Frame->GetWidth()), random(rangeY));
			break;
		default:
			break;
		}
	}
};

#endif