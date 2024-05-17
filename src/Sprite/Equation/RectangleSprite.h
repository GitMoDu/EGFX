// RectangleSprite.h

#ifndef _RECTANGLE_SPRITE_h
#define _RECTANGLE_SPRITE_h

#include "../../Model/ISprite.h"

template<const uint8_t width,
	const uint8_t height>
class RectangleSprite : public AbstractSprite<width, height>
{
public:
	RectangleSprite() : AbstractSprite<width, height>()
	{}

	const bool Get(RgbColor& color, const uint8_t x, const uint8_t y) final
	{
		return GetColor(color, x, y);
	}

protected:
	virtual const bool GetColor(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		color.r = UINT8_MAX;
		color.g = UINT8_MAX;
		color.b = UINT8_MAX;

		return true;
	}
};

#endif