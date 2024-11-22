// CircleSprite.h

#ifndef _CIRCLE_SPRITE_h
#define _CIRCLE_SPRITE_h

#include "../../Model/ISprite.h"

class CircleSprite : public ISprite
{
private:
	uint16_t RadiusPow = 0;
	uint8_t Radius = 0;

public:
	CircleSprite() : ISprite()
	{}

	const uint8_t GetWidth() const final
	{
		return GetRadius() * 2;
	}

	const uint8_t GetHeight() const final
	{
		return GetWidth();
	}

	const uint8_t GetRadius() const
	{
		return Radius;
	}

	void SetRadius(const uint8_t radius)
	{
		Radius = radius;
		if (radius > 1)
		{
			RadiusPow = ((uint16_t)radius * radius) - 1;
		}
		else
		{
			RadiusPow = ((uint16_t)radius * radius);
		}
	}

	const bool IsInsideCircle(const uint8_t x, const uint8_t y)
	{
		const int16_t dx = -(int16_t)Radius + x + 1;
		const int16_t dy = -(int16_t)Radius + y + 1;
		const uint16_t distancePow = (uint16_t)((dx * dx) + (dy * dy));

		return distancePow <= RadiusPow;
	}

	virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		if (IsInsideCircle(x, y))
		{
			color.r = UINT8_MAX;
			color.g = UINT8_MAX;
			color.b = UINT8_MAX;

			return true;
		}
		else
		{
			return false;
		}
	}
};
#endif