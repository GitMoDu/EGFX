// BitmapDemo.h

#ifndef _BITMAP_DEMO_h
#define _BITMAP_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Sprites.h"

using namespace DemoSprites;
using namespace SpriteShader;
using namespace SpriteShaderEffect;
using namespace SpriteTransform;

/// <summary>
/// Bitmap with translation, rotation and brightness animation.
/// Draws the bitmap line by line on each draw call.
/// </summary>
/// <typeparam name="Layout"></typeparam>
template<typename Layout>
class BitmapDemo : public ElementDrawer
{
private:
	static constexpr uint32_t TranslationXDuration = 7000000;
	static constexpr uint32_t TranslationYDuration = 13000000;
	static constexpr uint32_t BrightnessPeriod = 9876543;

	struct BitmapLayout : public Layout
	{
		static constexpr uint8_t UsableX()
		{
			return Layout::Width() - 1 - DogeSprite::Width;
		}

		static constexpr uint8_t UsableY()
		{
			return Layout::Height() - 1 - DogeSprite::Height;
		}
	};

	enum class DrawElementsEnum : uint8_t
	{
		PrepareBitmap = 0,
		DrawBitmapN = 1,
		DrawElementsCount = (uint8_t)DrawBitmapN + DogeSprite::Height
	};

private:
	BrightnessEffect<TransparentColorEffect<DogeSprite>> Doge{};

	RotateTransform<DogeSprite::Width, DogeSprite::Height> DogeRotator{};

	uint8_t x = 0;
	uint8_t y = 0;

public:
	BitmapDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Doge.SetTransparentColor(0, 0, 0);
	}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareBitmap:
			PrepareBitmap(drawState);
			break;
		default:
			if (drawState->ElementIndex >= (uint8_t)DrawElementsEnum::DrawBitmapN
				&& drawState->ElementIndex < (uint8_t)DrawElementsEnum::DrawElementsCount)
			{
				DrawBitmapLine(drawState->ElementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN);
			}
			break;
		}
	}

private:
	void PrepareBitmap(DrawState* drawState)
	{
		const uint16_t progressX = ProgressScaler::TriangleResponse(drawState->GetProgress<TranslationXDuration>());
		const uint16_t progressY = ProgressScaler::TriangleResponse(drawState->GetProgress<TranslationYDuration>());
		const uint16_t progressBrightness = ProgressScaler::TriangleResponse(drawState->GetProgress<BrightnessPeriod>());
		const int8_t brightness = ProgressScaler::ScaleProgress(progressBrightness, (uint8_t)UINT8_MAX) + INT8_MIN;

		x = Layout::X() + ProgressScaler::ScaleProgress(progressX, BitmapLayout::UsableX());
		y = Layout::Y() + ProgressScaler::ScaleProgress(progressY, BitmapLayout::UsableY());

		DogeRotator.SetRotation(drawState->FrameCounter % 360);
		Doge.SetBrightness(brightness);
	}

	void DrawBitmapLine(uint8_t index)
	{
		SpriteRenderer::TransformDrawPartial(
			Frame, &Doge, &DogeRotator,
			x, y,
			0, index,
			DogeSprite::Width, 1);
	}
};
#endif