// RgbColor.h

#ifndef _RGB_COLOR_h
#define _RGB_COLOR_h

#include <stdint.h>

struct RgbColor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	RgbColor()
		: r(0)
		, g(0)
		, b(0)
	{}

	RgbColor(const uint8_t _r, const uint8_t _g, const uint8_t _b)
		: r(_r)
		, g(_g)
		, b(_b)
	{}
};
#endif