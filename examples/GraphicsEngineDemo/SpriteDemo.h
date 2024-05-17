// SpriteDemo.h

#ifndef _SPRITE_DEMO_h
#define _SPRITE_DEMO_h

#include <ArduinoGraphicsDrawer.h>

#include "Sprites.h"

using namespace DemoSprites;
using namespace SpriteShader;
using namespace SpriteShaderEffect;

class SpriteDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		AnimatedHeart,
		VerticalArrow,
		HorizontalArrow,
		DynamicRing,
		DrawElementsCount
	};

	static constexpr uint8_t MaxRadius = 13;
	static constexpr uint8_t RingWidth = 2;
	static constexpr uint8_t MinRadius = 3;

	static constexpr uint32_t AnimateDuration = 1100000;
	static constexpr uint32_t RingResizeDuration = 1712300;
	static constexpr uint32_t ScrollHorizontalDuration = 7067123;
	static constexpr uint32_t ScrollVerticalDuration = 3674000;

private:
	ExplosionSprite AnimatedExplosion{};
	ArrowSprite Arrow{};
	TransparentGridEffect<SingleColorShader<RingSprite>> Ring{};

public:
	SpriteDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		//Heart.SetColor(UINT8_MAX, 0, 0);
		//Heart.SetColor2(UINT8_MAX/8, 0, 0);

		Ring.SetColor(UINT8_MAX, 0, UINT8_MAX);
		Ring.SetTransparency(1);
	}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::AnimatedHeart:
			DrawHeart(drawState);
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
	void DrawHeart(DrawState* drawState)
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
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask);
			break;
		case 5:
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 7:
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask2);
			break;
		case 8:
		case 9:
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask3);
			break;
		case 10:
		case 11:
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask4);
			break;
		case 12:
		case 13:
		case 14:
			AnimatedExplosion.SetMask(SpriteSource::Explosion::Mask5);
			break;
		default:
			return;
			break;
		}

		SpriteRenderer::Draw(Frame, &AnimatedExplosion,
			(Frame->GetWidth() - AnimatedExplosion.Width - 1) / 2, (Frame->GetHeight() - AnimatedExplosion.Height) / 2 - 1);
	}

	void DrawVerticalArrow(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<ScrollVerticalDuration>();
		const uint8_t usableY = Frame->GetHeight() - Arrow.Height - 1;
		const uint8_t y = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), usableY);

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::Draw(Frame, &Arrow, 0, y);
		}
		else
		{
			SpriteRenderer::TransformDraw<InvertTransform::InvertY<Arrow.Width, Arrow.Height>>(
				Frame, &Arrow,
				0, y,
				0);
		}
	}

	void DrawHorizontalArrow(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<ScrollHorizontalDuration>();
		const uint8_t usableX = Frame->GetWidth() - Arrow.Height - 1;
		const uint8_t x = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), usableX);

		if (progress >= INT16_MAX)
		{
			SpriteRenderer::TransformDraw<InvertTransform::Flip<Arrow.Width, Arrow.Height>>(
				Frame, &Arrow,
				x, 0,
				0);
		}
		else
		{
			SpriteRenderer::TransformDraw<InvertTransform::FlipInvertX<Arrow.Width, Arrow.Height>>(
				Frame, &Arrow,
				x, 0,
				0);
		}
	}

	void DrawDynamicRing(DrawState* drawState)
	{
		const uint16_t progress = drawState->GetProgress<RingResizeDuration>();
		const uint8_t outerRadius = MinRadius + ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(MaxRadius - MinRadius));
		const uint8_t innerRadius = outerRadius - RingWidth;

		const uint16_t progressX = drawState->GetProgress<ScrollHorizontalDuration * 5>();
		const uint16_t progressY = drawState->GetProgress<ScrollVerticalDuration * 3>();

		const uint8_t x = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressX), Frame->GetWidth());
		const uint8_t y = ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progressY), Frame->GetHeight());

		Ring.SetRadius(innerRadius, outerRadius);

		SpriteRenderer::Draw(
			Frame, &Ring,
			x - outerRadius, y - outerRadius
		);
	}
};
#endif

