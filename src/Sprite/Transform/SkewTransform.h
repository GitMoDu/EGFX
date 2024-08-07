// SkewTransform.h

#ifndef _SKEW_TRANSFORM_h
#define _SKEW_TRANSFORM_h

#include <stdint.h>

namespace SkewTransform
{
	template<const uint8_t Width,
		const uint8_t Height,
		const uint8_t ReferenceY = (Height / 2)>
	struct Horizontal
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			if (y < Height)
			{
				x += ((((int16_t)ReferenceY - 1) - y) * parameter) / (Height - 1);
			}
			else
			{
				return false;
			}

			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height,
		const uint8_t ReferenceX = (Width / 2)>
	struct Vertical
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			if (x < Width)
			{
				y += ((((int16_t)ReferenceX - 1) - x) * parameter) / (Width - 1);
			}
			else
			{
				return false;
			}

			return true;
		}
	};
}

#endif