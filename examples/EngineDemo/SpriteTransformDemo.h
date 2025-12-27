#ifndef _SPRITE_TRANSFORM_DEMO_h
#define _SPRITE_TRANSFORM_DEMO_h

#include <EgfxDrawer.h>
#include "Assets.h"

using namespace Egfx;

template<typename Layout, bool BinaryDisplay>
class SpriteTransformDemo final : public IFrameDraw
{
private:
	enum class DrawCallEnum : uint8_t
	{
		Heart,
		Rectangle,
		Doge,
		EnumCount
	};

	static constexpr bool UseHeartBig = (Layout::Width() > 128) || (Layout::Height() > 128);

	using HeartSpriteType = typename Assets::HeartSelect<UseHeartBig>::type;

	struct LayerLayout
	{
		struct Heart
		{
			static constexpr pixel_t Height() { return HeartSpriteType::Height; }
			static constexpr pixel_t Width() { return HeartSpriteType::Width; }

			static constexpr pixel_t X() { return Layout::X() + 2; }
			static constexpr pixel_t Y() { return Layout::Y() + (Layout::Height() / 2) - (Height() / 2); }
		};

		struct Rectangle
		{
			static constexpr pixel_t Width() { return MinValue(Layout::Width(), Layout::Height()) / 3; }
			static constexpr pixel_t Height() { return Width(); }
			static constexpr pixel_t X() { return Layout::X() + Layout::Width() - Width() - (Width() / 3); }
			static constexpr pixel_t Y() { return Layout::Y() + (Layout::Height() / 2) - (Height() / 2); }
		};

		struct Doge
		{
			static constexpr pixel_t Height() { return Assets::DogeBitSprite::Height; }
			static constexpr pixel_t Width() { return Assets::DogeBitSprite::Width; }
			static constexpr pixel_t RangeX() { return Layout::Width() - Width() - 0 - Rectangle::Width() - 0 - Heart::Width() - 0; }
			static constexpr pixel_t RangeY() { return Layout::Height() - Height(); }

			static constexpr pixel_t X() { return Layout::X() + Heart::Width() + 1; }
			static constexpr pixel_t Y() { return Layout::Y(); }
		};
	};

	SpriteShader::ColorShader<HeartSpriteType> Heart{};
	SpriteTransform::DownScaleXYTransform<HeartSpriteType::Width, HeartSpriteType::Height> HeartDownScaler{};

	SpriteShader::GridShader<RectangleFixedSprite<LayerLayout::Rectangle::Width(),
		LayerLayout::Rectangle::Height()>> Rectangle{};
	SpriteTransform::SkewHorizontalTransform<LayerLayout::Rectangle::Height(),
		SpriteTransform::SkewVerticalTransform<LayerLayout::Rectangle::Width()>> RectangleSkewer{};

	SpriteShader::HorizontalGradientShader<Assets::DogeBitSprite> GradientDoge{};
	SpriteShader::ColorShader<Assets::DogeBitSprite> Doge{};
	SpriteTransform::RotateTransform<Assets::DogeBitSprite::Width,
		Assets::DogeBitSprite::Height> DogeRotator{};

	uint8_t CallIndex = 0;

public:
	SpriteTransformDemo() : IFrameDraw()
	{
		if (BinaryDisplay)
			Heart.SetColor(RGB_COLOR_WHITE);
		else
			Heart.SetColor(RGB_COLOR_RED);

		if (BinaryDisplay)
		{
			Rectangle.SetColor1(RGB_COLOR_WHITE);
			Rectangle.SetColor2(RGB_COLOR_BLACK);
		}

		if (BinaryDisplay)
		{
			Doge.SetColor(Rgb::Color(RGB_COLOR_WHITE));
		}
		else
		{
			// Gradient colors for Doge sprite.
			GradientDoge.SetColor1(Rgb::Color(0xFF, 0x5A, 0x23));
			GradientDoge.SetColor2(Rgb::Color(0xFF, 0xd3, 0x91));
		}

		RectangleSkewer.SetReferenceX(0); // Skew vertically around left edge.
		RectangleSkewer.SetReferenceY((LayerLayout::Rectangle::Height() - 1) / 2); // Skew horizontally around vertical center.
	}

	~SpriteTransformDemo() = default;

	// Always enabled.
	virtual bool IsEnabled() const final { return true; }
	virtual void SetEnabled(const bool /*enabled*/) final {}

#if defined(SERIAL_LOG)
	void PrintDescription() const
	{
		Serial.println(F("Sprite Transform\n\tdraw and transform sprites with scaling, skewing, and rotation."));
	}
#endif

	virtual bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t /*frameCounter*/) final
	{
		switch (DrawCallEnum(CallIndex))
		{
		case DrawCallEnum::Heart:
		{
			const uint16_t progress = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<1500000>(frameTime));
			const uint8_t downScale = ProgressScaler::ScaleProgress(progress, (uint8_t)(LayerLayout::Heart::Height()));

			HeartDownScaler.SetDownScaleXY(downScale);

			SpriteRenderer::TransformDraw(
				frame, &Heart, &HeartDownScaler,
				LayerLayout::Heart::X() + (downScale / 2), LayerLayout::Heart::Y() + (downScale / 2));
		}
		break;
		case DrawCallEnum::Rectangle:
		{
			if (!BinaryDisplay)
			{
				Rectangle.SetColor1(Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 233), UINT8_MAX, UINT8_MAX));
				Rectangle.SetColor2(Rgb::ColorFromHSV(static_cast<angle_t>(frameTime / 175), UINT8_MAX, UINT8_MAX));
			}
			const uint16_t sectionProgress = ProgressScaler::GetProgress<1400000 * 2>(frameTime);
			const uint16_t progress = ProgressScaler::GetProgress<1400000>(frameTime) + (INT16_MAX / 2);

			if (ProgressScaler::ScaleProgress(sectionProgress, (uint8_t)2) == 0)
			{
				const int8_t skew = -(int16_t)LayerLayout::Rectangle::Width() + (int16_t)ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(LayerLayout::Rectangle::Width() * 2));
				RectangleSkewer.SetSkewX(skew);
				RectangleSkewer.SetSkewY(0);

				SpriteRenderer::TransformDraw(frame, &Rectangle, &RectangleSkewer,
					LayerLayout::Rectangle::X(), LayerLayout::Rectangle::Y());
			}
			else
			{
				const int8_t skew = -(int16_t)LayerLayout::Rectangle::Height() + (int16_t)ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(LayerLayout::Rectangle::Height() * 2));
				RectangleSkewer.SetSkewX(0);
				RectangleSkewer.SetSkewY(skew);

				SpriteRenderer::TransformDraw(frame, &Rectangle, &RectangleSkewer,
					LayerLayout::Rectangle::X(), LayerLayout::Rectangle::Y());
			}
		}
		break;
		case DrawCallEnum::Doge:
		{
			const uint16_t progressX = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<7000000>(frameTime));
			const uint16_t progressY = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<13000000>(frameTime));

			const pixel_t x = LayerLayout::Doge::X() + ProgressScaler::ScaleProgress(progressX, (uint16_t)LayerLayout::Doge::RangeX());
			const pixel_t y = LayerLayout::Doge::Y() + ProgressScaler::ScaleProgress(progressY, (uint16_t)LayerLayout::Doge::RangeY());

			DogeRotator.SetRotation(frameTime / 100);
			if (BinaryDisplay)
				SpriteRenderer::TransformDraw(frame, &Doge, &DogeRotator, x, y);
			else
				SpriteRenderer::TransformDraw(frame, &GradientDoge, &DogeRotator, x, y);
		}
		break;
		default:
			break;
		}

		CallIndex++; // Advance call index and determine if cycle is complete.
		if (CallIndex >= uint8_t(DrawCallEnum::EnumCount))
		{
			CallIndex = 0;
			return true;
		}

		return false;
	}
};
#endif

