// RgbColor.h

#ifndef _RGB_COLOR_h
#define _RGB_COLOR_h

#include <stdint.h>

struct RgbColor
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	RgbColor()
		: r(0)
		, g(0)
		, b(0)
	{}

	RgbColor(const uint32_t rgb32)
		: r((rgb32 >> 16)& UINT8_MAX)
		, g((rgb32 >> 8)& UINT8_MAX)
		, b((rgb32 >> 0)& UINT8_MAX)
	{
	}

	RgbColor(const uint8_t _r, const uint8_t _g, const uint8_t _b)
		: r(_r)
		, g(_g)
		, b(_b)
	{}

	void SetFrom(const RgbColor& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
	}

	void FromRGB32(const uint32_t rgb32)
	{
		r = (rgb32 >> 16) & UINT8_MAX;
		g = (rgb32 >> 8) & UINT8_MAX;
		b = (rgb32 >> 0) & UINT8_MAX;
	}
};
#endif