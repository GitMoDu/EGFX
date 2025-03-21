// SpriteDemo.h

#ifndef _SPRITE_DEMO_h
#define _SPRITE_DEMO_h

#include <ArduinoGraphicsDrawer.h>

#include "Sprites.h"

using namespace DemoSprites;
using namespace SpriteShader;
using namespace SpriteShaderEffect;
using namespace SpriteTransform;

template<typename Layout>
class SpriteDemo : public ElementDrawer
{
private:
	static constexpr uint32_t AnimateDuration = 1100000;
	static constexpr uint32_t RingResizeDuration = 1712300;
	static constexpr uint32_t ScrollHorizontalDuration = 7067123;
	static constexpr uint32_t ScrollVerticalDuration = 3674000;

	enum class DrawElementsEnum : uint8_t
	{
		AnimatedExplosion,
		VerticalArrow,
		HorizontalArrow,
		DynamicRing,
		DrawElementsCount
	};

	struct ArrowLayout : public LayoutElement<Layout::X(), Layout::Y(), Layout::Width(), Layout::Height()>
	{
		static constexpr pixel_t UsableY()
		{
			return Layout::Height() - ArrowSprite::Height - 1;
		}

		static constexpr pixel_t UsableX()
		{
			return Layout::Width() - ArrowSprite::Width - 1;
		}
	};

	using ExplosionLayout = LayoutElement<(Layout::Width() - ExplosionSprite::Width - 1) / 2,
		(Layout::Height() - ExplosionSprite::Height) / 2 - 1,
		ExplosionSprite::Width, ExplosionSprite::Height>;


	struct RingLayout
	{
		static constexpr pixel_t RingWidth() { return 5; }
		static constexpr pixel_t MinRadius() { return RingWidth() + 1; }
		static constexpr pixel_t MaxRadius() { return (MinRadius() * 2) + 1; }

		static constexpr pixel_t RadiusRange()
		{
			return MaxRadius() - MinRadius();
		}

		static constexpr pixel_t X()
		{
			return Layout::X() + MaxRadius();
		}

		static constexpr pixel_t Y()
		{
			return Layout::Y() + MaxRadius();
		}

		static constexpr pixel_t UsableY()
		{
			return Layout::Height() - (MaxRadius() * 2);
		}

		static constexpr pixel_t UsableX()
		{
			return Layout::Width() - (MaxRadius() * 2);
		}
	};

private:
	ColorShader<ExplosionSprite> AnimatedExplosion{};
	ColorShader<ArrowSprite> Arrow{};
	ColorShader<TransparentGridEffect<RingSprite>> Ring{};

	FlipTransform<> ArrowInvert{};
	FlipInvertXTransform<ArrowSprite::Height> ArrowFlipInvertX{};
	InvertYTransform<ArrowSprite::Height> ArrowInvertY{};

public:
	SpriteDemo()
		: ElementDrawer((uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Ring.SetColor(Rgb::Color(UINT8_MAX, 0, UINT8_MAX));
		Ring.SetTransparency(1);

		Arrow.SetColor(Rgb::Color(0, UINT8_MAX, 0));
	}

	virtual void DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter, const uint8_t elementIndex) final
	{
		switch (elementIndex)
		{
		case (uint8_t)DrawElementsEnum::AnimatedExplosion:
			DrawAnimatedExplosion(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::VerticalArrow:
			DrawVerticalArrow(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::HorizontalArrow:
			DrawHorizontalArrow(frame, frameTime);
			break;
		case (uint8_t)DrawElementsEnum::DynamicRing:
			DrawDynamicRing(frame, frameTime);
			break;
		default:
			break;
		}
	}

private:
	void DrawAnimatedExplosion(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progress = ProgressScaler::GetProgress<AnimateDuration>(frameTime);
		const uint8_t animationFrame = ProgressScaler::ScaleProgress(progress, (uint8_t)20);

		switch (animationFrame)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			AnimatedExplosion.SetColor(Rgb::Color(0, UINT8_MAX, UINT8_MAX));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask);
			break;
		case 5:
			AnimatedExplosion.SetColor(Rgb::Color(UINT8_MAX, INT8_MAX, INT8_MAX));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 7:
			AnimatedExplosion.SetColor(Rgb::Color(UINT8_MAX, 0, 0));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 8:
		case 9:
			AnimatedExplosion.SetColor(Rgb::Color(UINT8_MAX, UINT8_MAX, 0));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask3);
			break;
		case 10:
		case 11:
			AnimatedExplosion.SetColor(Rgb::Color(UINT8_MAX, INT8_MAX, 0));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask4);
			break;
		case 12:
		case 13:
		case 14:
			AnimatedExplosion.SetColor(Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX));
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask5);
			break;
		default:
			return;
			break;
		}

		SpriteRenderer::Draw(frame, &AnimatedExplosion, ExplosionLayout::X(), ExplosionLayout::Y());
	}

	void DrawVerticalArrow(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progress = ProgressScaler::GetProgress<ScrollVerticalDuration>(frameTime);
		const pixel_t y = ArrowLayout::Y() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint16_t)ArrowLayout::UsableY());

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::Draw(frame, &Arrow, ArrowLayout::X(), y);
		}
		else
		{
			SpriteRenderer::TransformDraw(frame, &Arrow, &ArrowInvertY, ArrowLayout::X(), y);
		}
	}

	void DrawHorizontalArrow(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progress = ProgressScaler::GetProgress<ScrollHorizontalDuration>(frameTime);
		const pixel_t x = ArrowLayout::X() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint16_t)ArrowLayout::UsableX());

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::TransformDraw(
				frame, &Arrow, &ArrowInvert,
				x, ArrowLayout::Y());
		}
		else
		{
			SpriteRenderer::TransformDraw(
				frame, &Arrow, &ArrowFlipInvertX,
				x, ArrowLayout::Y());
		}
	}

	void DrawDynamicRing(IFrameBuffer* frame, const uint32_t frameTime)
	{
		const uint16_t progress = ProgressScaler::GetProgress<RingResizeDuration>(frameTime);
		const pixel_t outerRadius = RingLayout::MinRadius() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint16_t)RingLayout::RadiusRange());
		const pixel_t innerRadius = outerRadius - RingLayout::RingWidth();

		const uint16_t progressX = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<ScrollHorizontalDuration * 5>(frameTime));
		const uint16_t progressY = ProgressScaler::TriangleResponse(ProgressScaler::GetProgress<ScrollVerticalDuration * 3>(frameTime));

		const pixel_t x = RingLayout::X() + ProgressScaler::ScaleProgress(progressX, (uint16_t)RingLayout::UsableX()) - outerRadius;
		const pixel_t y = RingLayout::Y() + ProgressScaler::ScaleProgress(progressY, (uint16_t)RingLayout::UsableY()) - outerRadius;

		Ring.SetRadius(innerRadius, outerRadius);

		SpriteRenderer::Draw(frame, &Ring, x, y);
	}
};
#endif

