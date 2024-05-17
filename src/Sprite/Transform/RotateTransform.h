// RotateTransform.h

#ifndef _ROTATE_TRANSFORM_h
#define _ROTATE_TRANSFORM_h

#include "SkewTransform.h"
#include "InvertTransform.h"

namespace RotateTransform
{
	template<const uint8_t Width,
		const uint8_t Height>
	class Rotate
	{
	public:
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			if (parameter > 360
				|| parameter < -360)
			{
				return Transform(x, y, parameter % 360);
			}
			else if (parameter > 180)
			{
				return Transform(x, y, parameter - 180)
					&& InvertTransform::Flip<Width, Height>::Transform(x, y, 0);
			}
			else if (parameter < -180)
			{
				return Transform(x, y, parameter + 180)
					&& InvertTransform::Flip<Width, Height>::Transform(x, y, 0);
			}
			else if (parameter > 45)
			{
				return Transform(x, y, -90 + parameter)
					&& InvertTransform::FlipInvertY<Width, Height>::Transform(x, y, 0);
			}
			else if (parameter < -45)
			{
				return Transform(x, y, +90 + parameter)
					&& InvertTransform::FlipInvertX<Width, Height>::Transform(x, y, 0);
			}
			else if (parameter == 0)
			{
				return true;
			}
			else
			{
				const int8_t h = ((int16_t)parameter * Width * 9) / 720;
				const int8_t v = ((int16_t)parameter * Height * 10) / 720;

				return SkewTransform::Horizontal<Width, Height>::Transform(x, y, -h)
					&& SkewTransform::Vertical<Width, Height>::Transform(x, y, v)
					&& SkewTransform::Horizontal<Width, Height>::Transform(x, y, -h)
					&& x < Width
					&& y < Height;
			}
		}
	};
}

#endif