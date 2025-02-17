// SpriteEffectBenchmark.h

#ifndef _SPRITE_EFFECT_BENCHMARK_h
#define _SPRITE_EFFECT_BENCHMARK_h

#include <ArduinoGraphicsDrawer.h>

#include "Assets.h"

class SpriteShaderBenchmark : public IFrameDraw
{
private:
	SpriteShaderEffect::BrightnessEffect<
		SpriteShaderEffect::TransparentGridEffect<
		SpriteShader::GridShader<Assets::GridMaskSprite>>> GridSprite{};

public:
	SpriteShaderBenchmark()
		: IFrameDraw()
	{
		GridSprite.SetColor1(Rgb::Color(0xFF, 0x00, 0xFF));
		GridSprite.SetColor1(Rgb::Color(0x00, 0xFF, 0x00));
		GridSprite.SetTransparency(1);
		GridSprite.SetBrightness(INT8_MAX / 2);
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		SpriteRenderer::Draw(frame, &GridSprite, 0, 0);

		return true;
	}
};

class SpriteTransformBenchmark : public IFrameDraw
{
private:
	SpriteShader::ColorShader<Assets::GridMaskSprite> GridSprite{};
	SpriteTransform::RotateTransform<Assets::GridMaskSprite::Width, Assets::GridMaskSprite::Height,
		SpriteTransform::DownScaleXYTransform<Assets::GridMaskSprite::Width, Assets::GridMaskSprite::Height,
		SpriteTransform::FlipInvertYTransform<Assets::GridMaskSprite::Width>>> Transform{};

public:
	SpriteTransformBenchmark()
		: IFrameDraw()
	{
		GridSprite.SetColor(Rgb::Color(0xFF, 0x00, 0xFF));
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		SpriteRenderer::TransformDraw(frame, &GridSprite, &Transform, 0, 0);

		return true;
	}
};

#endif

