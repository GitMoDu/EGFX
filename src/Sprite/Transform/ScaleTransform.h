// ScaleTransform.h

#ifndef _SCALE_TRANSFORM_h
#define _SCALE_TRANSFORM_h

#include <stdint.h>

namespace ScaleTransform
{
	template<const uint8_t Width,
		const uint8_t Height>
	struct DownScaleX
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t skips = (parameter * x) / Width;

			if (skips == 0
				|| (x % Width) != skips)
			{
				x = x - skips;
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct DownScaleY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t skips = (parameter * y) / Height;

			if (skips == 0
				|| (y % Height) != skips)
			{
				y = y - skips;
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct DownScaleXY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t skipsX = (parameter * x) / Width;
			const uint8_t skipsY = (parameter * y) / Height;

			if ((skipsX == 0 || (x % Width) != skipsX) && (skipsY == 0 || (y % Height) != skipsY))
			{
				x = x - skipsX;
				y = y - skipsY;

				return true;
			}
			else
			{
				return false;
			}
		}
	};
};

#endif