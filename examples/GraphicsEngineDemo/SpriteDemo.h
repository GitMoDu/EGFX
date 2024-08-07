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
		static constexpr uint8_t UsableY()
		{
			return Layout::Height() - ArrowSprite::Height - 1;
		}

		static constexpr uint8_t UsableX()
		{
			return Layout::Width() - ArrowSprite::Width - 1;
		}
	};

	using ExplosionLayout = LayoutElement<(Layout::Width() - ExplosionSprite::Width - 1) / 2,
		(Layout::Height() - ExplosionSprite::Height) / 2 - 1,
		ExplosionSprite::Width, ExplosionSprite::Height>;


	struct RingLayout
	{
		static constexpr uint8_t RingWidth() { return 5; }
		static constexpr uint8_t MinRadius() { return RingWidth() + 1; }
		static constexpr uint8_t MaxRadius() { return (MinRadius() * 2) + 1; }

		static constexpr uint8_t RadiusRange()
		{
			return MaxRadius() - MinRadius();
		}
	};

private:
	ColorShader<ExplosionSprite> AnimatedExplosion{};
	ArrowSprite Arrow{};
	ColorShader<TransparentGridEffect<RingSprite>> Ring{};

	FlipTransform<> ArrowInvert{};
	FlipInvertXTransform<ArrowSprite::Height> ArrowFlipInvertX{};
	InvertYTransform<ArrowSprite::Height> ArrowInvertY{};

public:
	SpriteDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Ring.SetColor(UINT8_MAX, 0, UINT8_MAX);
		Ring.SetTransparency(1);
	}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::AnimatedExplosion:
			DrawAnimatedExplosion(drawState);
			break;
		case (uint8_t)DrawElementsEnum::VerticalArrow:
			DrawVerticalArrow(drawState);
			break;
		case (uint8_t)DrawElementsEnum::HorizontalArrow:
			DrawHorizontalArrow(drawState);
			break;
		case (uint8_t)DrawElementsEnum::DynamicRing:
			DrawDynamicRing(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawAnimatedExplosion(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<AnimateDuration>();
		const uint8_t frame = ProgressScaler::ScaleProgress(progress, (uint8_t)20);

		switch (frame)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			AnimatedExplosion.SetColor(0, UINT8_MAX, UINT8_MAX);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask);
			break;
		case 5:
			AnimatedExplosion.SetColor(UINT8_MAX, INT8_MAX, INT8_MAX);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 7:
			AnimatedExplosion.SetColor(UINT8_MAX, 0, 0);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 8:
		case 9:
			AnimatedExplosion.SetColor(UINT8_MAX, UINT8_MAX, 0);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask3);
			break;
		case 10:
		case 11:
			AnimatedExplosion.SetColor(UINT8_MAX, INT8_MAX, 0);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask4);
			break;
		case 12:
		case 13:
		case 14:
			AnimatedExplosion.SetColor(UINT8_MAX, UINT8_MAX, UINT8_MAX);
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask5);
			break;
		default:
			return;
			break;
		}

		SpriteRenderer::Draw(Frame, &AnimatedExplosion, ExplosionLayout::X(), ExplosionLayout::Y());
	}

	void DrawVerticalArrow(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<ScrollVerticalDuration>();
		const uint8_t y = ArrowLayout::Y() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), ArrowLayout::UsableY());

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::Draw(Frame, &Arrow, ArrowLayout::X(), y);
		}
		else
		{
			SpriteRenderer::TransformDraw(Frame, &Arrow, &ArrowInvertY, ArrowLayout::X(), y);
		}
	}

	void DrawHorizontalArrow(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<ScrollHorizontalDuration>();
		const uint8_t x = ArrowLayout::X() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), ArrowLayout::UsableX());

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::TransformDraw(
				Frame, &Arrow, &ArrowInvert,
				x, ArrowLayout::Y());
		}
		else
		{
			SpriteRenderer::TransformDraw(
				Frame, &Arrow, &ArrowFlipInvertX,
				x, ArrowLayout::Y());
		}
	}

	void DrawDynamicRing(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<RingResizeDuration>();
		const uint8_t outerRadius = RingLayout::MinRadius() + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), RingLayout::RadiusRange());
		const uint8_t innerRadius = outerRadius - RingLayout::RingWidth();

		const uint16_t progressX = ProgressScaler::TriangleResponse(drawState->GetProgress<ScrollHorizontalDuration * 5>());
		const uint16_t progressY = ProgressScaler::TriangleResponse(drawState->GetProgress<ScrollVerticalDuration * 3>());

		const uint8_t x = Layout::X() + ProgressScaler::ScaleProgress(progressX, Layout::Width()) - outerRadius;
		const uint8_t y = Layout::Y() + ProgressScaler::ScaleProgress(progressY, Layout::Height()) - outerRadius;

		Ring.SetRadius(innerRadius, outerRadius);

		SpriteRenderer::Draw(Frame, &Ring, x, y);
	}
};
#endif

