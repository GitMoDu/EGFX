#ifndef _BITMAP_DEMO_h
#define _BITMAP_DEMO_h

#include <ArduinoGraphicsDrawer.h>
#include "Assets.h"

using namespace Assets;
using namespace SpriteShader;
using namespace SpriteShaderEffect;
using namespace SpriteTransform;

using namespace IntegerSignal::FixedPoint;

/// <summary>
/// Bitmap with translation, rotation and brightness animation.
/// Draws the bitmap line by line on each draw call.
/// </summary>
template<typename Layout, bool Monochrome>
class BitmapDemo : public IFrameDraw
{
private:
	static constexpr uint32_t TranslationXDuration = 7000000;
	static constexpr uint32_t TranslationYDuration = 13000000;
	static constexpr uint32_t BrightnessPeriod = 3876543;
	static constexpr uint16_t RotationFrequency = 50;

	struct BitmapLayout
	{
		static constexpr pixel_t UsableX()
		{
			return Layout::Width() - DogeSprite::Width;
		}

		static constexpr pixel_t UsableY()
		{
			return Layout::Height() - DogeSprite::Height;
		}

		static constexpr uint8_t BitmapDrawSteps()
		{
			return DogeSprite::Height / 2;
		}

		static constexpr pixel_t SectionHeight()
		{
			return DogeSprite::Height / BitmapDrawSteps();
		}

		static constexpr pixel_t X() { return Layout::X(); }
		static constexpr pixel_t Y() { return Layout::Y(); }
		static constexpr pixel_t Width() { return DogeSprite::Width; }
		static constexpr pixel_t Height() { return DogeSprite::Height; }

		static constexpr pixel_t RangeX() { return Layout::Width() - Width(); }
		static constexpr pixel_t RangeY() { return Layout::Height() - Height(); }
	};

private:
	BrightnessEffect<TransparentColorEffect<DogeSprite>> Doge{};
	RotateTransform<DogeSprite::Width, DogeSprite::Height> DogeRotator{};

	pixel_t x = 0;
	pixel_t y = 0;

	uint8_t CallIndex = 0;

public:
	BitmapDemo() : IFrameDraw()
	{
		Doge.SetTransparentColor(0);
	}

	~BitmapDemo() = default;

	// Always enabled.
	virtual bool IsEnabled() const final { return true; }
	virtual void SetEnabled(const bool /*enabled*/) final {}

#if defined(SERIAL_LOG)
	void PrintDescription() const
	{
		Serial.println(F("Bitmap\n\twith translation, rotation and brightness animation."));
	}
#endif

	virtual bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		// Update bitmap effects on first call.
		if (CallIndex == 0)
		{
			const uint16_t progressX = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationXDuration>(frameTime));
			const uint16_t progressY = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<TranslationYDuration>(frameTime));

			x = BitmapLayout::X() + ProgressScaler::ScaleProgress(progressX, (uint16_t)BitmapLayout::RangeX());
			y = BitmapLayout::Y() + ProgressScaler::ScaleProgress(progressY, (uint16_t)BitmapLayout::RangeY());

			if (Monochrome)
			{
				Doge.SetBrightness(0);
			}
			else
			{
				const fraction16_t brightnessFraction = Fraction16::GetScalar((uint32_t)(frameTime % (BrightnessPeriod + 0)), BrightnessPeriod);
				const angle_t brightnessAngle = Fraction(brightnessFraction, ANGLE_RANGE);
				Doge.SetBrightness(Sine16(brightnessAngle));
			}

			DogeRotator.SetRotation(frameTime / RotationFrequency);
		}

		// Draw a section of the bitmap.
		SpriteRenderer::TransformDrawPartial(frame, &Doge, &DogeRotator,
			x, y, 0, CallIndex * BitmapLayout::SectionHeight(),
			DogeSprite::Width, BitmapLayout::SectionHeight());

		CallIndex++; // Advance call index and determine if cycle is complete.
		if (CallIndex >= BitmapLayout::BitmapDrawSteps())
		{
			CallIndex = 0;
			return true;
		}

		return false;
	}
};
#endif