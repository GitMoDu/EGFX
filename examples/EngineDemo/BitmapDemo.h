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

	static constexpr pixel_t UsableX()
	{
		return Layout::Width() - DogeSprite::Width;
	}

	static constexpr pixel_t UsableY()
	{
		return Layout::Height() - DogeSprite::Height;
	}

	using BitmapLayout = LayoutElement<Layout::X(), Layout::Y(), UsableX(), UsableY()>;

	enum class DrawElementsEnum : uint8_t
	{
		PrepareBitmap = 0,
		DrawBitmapN = 1,
		DrawElementsCount = (uint8_t)DrawBitmapN + DogeSprite::Height
	};

private:
	BrightnessEffect<TransparentColorEffect<DogeSprite>> Doge{};

	RotateTransform<DogeSprite::Width, DogeSprite::Height> DogeRotator{};

	pixel_t x = 0;
	pixel_t y = 0;

public:
	BitmapDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Doge.SetTransparentColor(0);
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::PrepareBitmap:
			PrepareBitmap(frameTime, frameCounter);
			break;
		default:
			if (elementIndex >= (uint8_t)DrawElementsEnum::DrawBitmapN
				&& elementIndex < (uint8_t)DrawElementsEnum::DrawElementsCount)
			{
				DrawBitmapLine(frame, elementIndex - (uint8_t)DrawElementsEnum::DrawBitmapN);
			}
			break;
		}
	}

private:
	void PrepareBitmap(const uint32_t frameTime, const uint16_t frameCounter)
	{
		const uint16_t progressX = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationXDuration>(frameTime));
		const uint16_t progressY = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationYDuration>(frameTime));
		const uint16_t progressBrightness = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<BrightnessPeriod>(frameTime));
		const int8_t brightness = ProgressScaler::ScaleProgress(progressBrightness, (uint8_t)UINT8_MAX) + INT8_MIN;

		x = BitmapLayout::X() + ProgressScaler::ScaleProgress(progressX, BitmapLayout::Width());
		y = BitmapLayout::Y() + ProgressScaler::ScaleProgress(progressY, BitmapLayout::Height());

		DogeRotator.SetRotation(frameCounter % 360);
		Doge.SetBrightness(brightness);
	}

	void DrawBitmapLine(IFrameBuffer* frame, const uint8_t index)
	{
		SpriteRenderer::TransformDrawPartial(
			frame, &Doge, &DogeRotator,
			x, y,
			0, index,
			DogeSprite::Width, 1);
	}
};
#endif