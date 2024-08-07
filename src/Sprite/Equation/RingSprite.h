// RingSprite.h

#ifndef _RING_SPRITE_h
#define _RING_SPRITE_h

#include "../../Model/ISprite.h"

class RingSprite : public ISprite
{
private:
	uint16_t InnerRadiusPow = 0;
	uint16_t OuterRadiusPow = 0;
	uint8_t OuterRadius = 0;

public:
	RingSprite() : ISprite()
	{}

	const uint8_t GetWidth() const final
	{
		return OuterRadius * 2;
	}

	const uint8_t GetHeight() const final
	{
		return GetWidth();
	}

	void SetRadius(const uint8_t innerRadius, const uint8_t outerRadius)
	{
		if (innerRadius < outerRadius)
		{
			OuterRadius = outerRadius;

			if (outerRadius > 1)
			{
				OuterRadiusPow = ((uint16_t)outerRadius * outerRadius) - 1;
			}
			else
			{
				OuterRadiusPow = (uint16_t)outerRadius * outerRadius;
			}

			InnerRadiusPow = ((uint16_t)innerRadius * innerRadius) - 1;
		}
		else
		{
			OuterRadius = 0;
			InnerRadiusPow = 0;
			OuterRadiusPow = 0;
		}
	}

	const bool IsInsideRing(const uint8_t x, const uint8_t y)
	{
		const int16_t dx = -(int16_t)OuterRadius + x;
		const int16_t dy = -(int16_t)OuterRadius + y;
		const uint16_t distancePow = (uint16_t)((dx * dx) + (dy * dy));

		return distancePow <= OuterRadiusPow
			&& distancePow >= InnerRadiusPow;
	}

	virtual const bool Get(RgbColor& color, const uint8_t x, const uint8_t y)
	{
		if (IsInsideRing(x, y))
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