// CircleSprite.h

#ifndef _CIRCLE_SPRITE_h
#define _CIRCLE_SPRITE_h

#include "../../Model/ISprite.h"

template<const pixel_t Radius>
class CircleFixedSprite : public AbstractSprite<Radius * 2, Radius * 2>
{
private:
	static constexpr coordinate_t GetRadiusPower(const pixel_t radius)
	{
		return ((radius > 1) * (((coordinate_t)radius * radius) - 1)) | ((radius <= 1) * (((coordinate_t)radius * radius)));
	}

private:
	static constexpr coordinate_t RadiusPow = GetRadiusPower(Radius);

public:
	CircleFixedSprite() : AbstractSprite<Radius * 2, Radius * 2>()
	{
	}

	const bool IsInsideCircle(const pixel_t x, const pixel_t y)
	{
		const coordinate_signed_t dx = -(coordinate_signed_t)Radius + x + 1;
		const coordinate_signed_t dy = -(coordinate_signed_t)Radius + y + 1;
		const coordinate_t distancePow = (coordinate_t)((dx * dx) + (dy * dy));

		return distancePow <= RadiusPow;
	}

	virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
	{
		if (IsInsideCircle(x, y))
		{
			color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			return true;
		}
		else
		{
			return false;
		}
	}
};

class CircleSprite : public ISprite
{
private:
	coordinate_t RadiusPow = 0;
	pixel_t Radius = 0;

public:
	CircleSprite() : ISprite()
	{
	}

	const pixel_t GetWidth() const final
	{
		return GetRadius() * 2;
	}

	const pixel_t GetHeight() const final
	{
		return GetWidth();
	}

	const pixel_t GetRadius() const
	{
		return Radius;
	}

	void SetRadius(const pixel_t radius)
	{
		Radius = radius;
		if (radius > 1)
		{
			RadiusPow = ((coordinate_t)radius * radius) - 1;
		}
		else
		{
			RadiusPow = ((coordinate_t)radius * radius);
		}
	}

	const bool IsInsideCircle(const pixel_t x, const pixel_t y)
	{
		const coordinate_signed_t dx = -(coordinate_signed_t)Radius + x + 1;
		const coordinate_signed_t dy = -(coordinate_signed_t)Radius + y + 1;
		const coordinate_t distancePow = (coordinate_t)((dx * dx) + (dy * dy));

		return distancePow <= RadiusPow;
	}

	virtual const bool Get(rgb_color_t& color, const pixel_t x, const pixel_t y)
	{
		if (IsInsideCircle(x, y))
		{
			color = Rgb::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			return true;
		}
		else
		{
			return false;
		}
	}
};
#endif