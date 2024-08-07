// RgbColorUtil.h

#ifndef _RGB_COLOR_UTIL_h
#define _RGB_COLOR_UTIL_h

#include "RgbColor.h"

class RgbColorUtil
{
private:
	/// <summary>
	/// Fast square root for uint16_t.
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	static const uint8_t SqrtU16(const uint16_t x)
	{
		uint8_t res = 0;
		uint8_t add = 0x80;

		for (uint_fast8_t i = 0; i < 8; i++)
		{
			const uint8_t temp = res | add;
			const uint16_t g2 = (uint16_t)temp * temp;
			if (x >= g2)
			{
				res = temp;
			}
			add >>= 1;
		}

		return res;
	}

	static const uint8_t InterpolateSquared(const uint8_t a, const uint8_t b, const uint8_t progress)
	{
		const uint16_t aa = ((uint32_t)(UINT8_MAX - progress) * (((uint16_t)a * a))) >> 8;
		const uint16_t bb = ((uint32_t)progress * ((uint16_t)b * b)) >> 8;

		return SqrtU16(aa + bb);
	}

	static constexpr uint8_t InterpolateLinear(const uint8_t a, const uint8_t b, const uint8_t progress)
	{
		return (((uint16_t)b * progress) + ((uint16_t)a * (UINT8_MAX - progress))) >> 8;
	}

public:
	/// <summary>
	/// Interpolate between 2 colors in RGB space.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="color1"></param>
	/// <param name="color2"></param>
	/// <param name="progress"></param>
	static void InterpolateRgb(RgbColor& color, const RgbColor& color1, const RgbColor& color2, const uint8_t progress)
	{
		color.r = InterpolateSquared(color1.r, color2.r, progress);
		color.g = InterpolateSquared(color1.g, color2.g, progress);
		color.b = InterpolateSquared(color1.b, color2.b, progress);
	}

	/// <summary>
	/// Interpolate between 2 colors linearly.
	/// Not as accurate as InterpolateRgb, but much faster.
	/// </summary>
	/// <param name="color"></param>
	/// <param name="color1"></param>
	/// <param name="color2"></param>
	/// <param name="progress"></param>
	static void InterpolateRgbLinear(RgbColor& color, const RgbColor& color1, const RgbColor& color2, const uint8_t progress)
	{
		color.r = InterpolateLinear(color1.r, color2.r, progress);
		color.g = InterpolateLinear(color1.g, color2.g, progress);
		color.b = InterpolateLinear(color1.b, color2.b, progress);
	}
};
#endif