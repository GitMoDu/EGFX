// SpriteBenchmark.h

#ifndef _SPRITEBENCHMARK_h
#define _SPRITEBENCHMARK_h

#include <ArduinoGraphicsDrawer.h>

#include "Assets.h"


class BitMaskBenchmark : public IFrameDraw
{
private:
	Assets::GridMaskSprite GridSprite{};

public:
	BitMaskBenchmark()
		: IFrameDraw()
	{
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		SpriteRenderer::Draw(frame, &GridSprite, 0, 0);

		return true;
	}
};

class BitMapBenchmark : public IFrameDraw
{
private:
	Assets::GridColorSprite GridSprite{};

private:
	rgb_color_t Color = Rgb::Color(UINT32_MAX);

public:
	BitMapBenchmark()
		: IFrameDraw()
	{
	}

	virtual const bool DrawCall(IFrameBuffer* frame, const uint32_t frameTime, const uint16_t frameCounter) final
	{
		SpriteRenderer::Draw(frame, &GridSprite, 0, 0);

		return true;
	}
};

#endif

