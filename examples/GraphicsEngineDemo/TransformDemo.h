// SpriteTransformDemo.h

#ifndef _SPRITE_TRANSFORM_DEMO_h
#define _SPRITE_TRANSFORM_DEMO_h

#include <ArduinoGraphicsDrawer.h>

#include "Sprites.h"

using namespace DemoSprites;
using namespace SpriteShader;
using namespace SpriteShaderEffect;

class TransformDemo : public ElementDrawer
{
private:
	enum class DrawElementsEnum : uint8_t
	{
		Skew,
		Scale,
		Rotate,
		DrawElementsCount
	};

	static constexpr uint32_t SkewDuration = 1400000;
	static constexpr uint32_t ScaleDuration = 1500000;
	static constexpr uint32_t RotationDuration = 4000000;

	GridShader<HeartSprite> Heart{};
	PyramidSprite Pyramid{};
	RectangleSprite<14, 7> Rectangle{};

	uint8_t Rotation = 0;
	int8_t Angle = 0;

public:
	TransformDemo(IFrameBuffer* frame)
		: ElementDrawer(frame, (uint8_t)DrawElementsEnum::DrawElementsCount)
	{
		Heart.SetColor(UINT8_MAX, 0, 0);
		Heart.SetColor2(UINT8_MAX / 8, 0, 0);
	}

	void DrawCall(DrawState* drawState) final
	{
		switch (drawState->ElementIndex)
		{
		case (uint8_t)DrawElementsEnum::Skew:
			DrawSkew(drawState);
			break;
		case (uint8_t)DrawElementsEnum::Scale:
			DrawScale(drawState);
			break;
		case (uint8_t)DrawElementsEnum::Rotate:
			DrawRotate(drawState);
			break;
		default:
			break;
		}
	}

private:
	void DrawSkew(DrawState* drawState)
	{
		const uint16_t sectionProgress = drawState->GetProgress<SkewDuration * 2>();
		const uint16_t progress = drawState->GetProgress<SkewDuration>() + (INT16_MAX / 2);

		if (ProgressScaler::ScaleProgress(sectionProgress, (uint8_t)2) == 0)
		{
			const int8_t skew = -(int16_t)Rectangle.Width + (int16_t)ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(Rectangle.Width * 2));

			SpriteRenderer::TransformDraw<SkewTransform::Horizontal<Rectangle.Width, Rectangle.Height>>(
				Frame, &Rectangle,
				(Rectangle.Width / 2), Rectangle.Height,
				skew);
		}
		else
		{
			const int8_t skew = -(int16_t)Rectangle.Height + (int16_t)ProgressScaler::ScaleProgress(ProgressScaler::TriangleResponse(progress), (uint8_t)(Rectangle.Height * 2));
			SpriteRenderer::TransformDraw<SkewTransform::Vertical<Rectangle.Width, Rectangle.Height, 0>>(
				Frame, &Rectangle,
				(Rectangle.Width / 2), Rectangle.Height,
				skew);
		}
	}

	void DrawScale(DrawState* drawState)
	{
		const uint16_t progress = ProgressScaler::TriangleResponse(drawState->GetProgress<ScaleDuration>());
		const int8_t scaleDown = ProgressScaler::ScaleProgress(progress, (uint8_t)(Heart.Width - 1));

		SpriteRenderer::TransformDraw<ScaleTransform::DownScaleXY<Heart.Width, Heart.Height>>(
			Frame,
			&Heart,
			(scaleDown / 2), (((uint16_t)Frame->GetHeight() * 2) / 3) - (Heart.Height / 2) + (scaleDown / 2),
			scaleDown);
	}

	void DrawRotate(DrawState* drawState)
	{
		const uint16_t progress = ProgressScaler::TriangleResponse(drawState->GetProgress<RotationDuration>());
		const int16_t angle = -(int16_t)180 + ProgressScaler::ScaleProgress(progress, (uint16_t)360);

		SpriteRenderer::TransformDraw<RotateTransform::Rotate<Pyramid.Width, Pyramid.Height>>(
			Frame, &Pyramid,
			(Frame->GetWidth() - Pyramid.Width) / 2,
			(Frame->GetHeight() - Pyramid.Height) / 2,
			angle);
	}
};
#endif

