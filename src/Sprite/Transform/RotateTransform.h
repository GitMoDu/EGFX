// RotateTransform.h

#ifndef _ROTATE_TRANSFORM_h
#define _ROTATE_TRANSFORM_h

#include "SkewTransform.h"
#include "InvertTransform.h"

namespace RotateTransform
{
	static constexpr uint8_t SinTable[] PROGMEM
	{
		18, 22, 27, 31, 35, 40, 44, 49, 53, 57, 62, 66, 70, 75, 79, 83, 87 ,91 ,96, 100, 104, 108, 112, 116, 120, 124, 128, 131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 167, 171, 174, 177, 180, 183, 186, 190, 192
	};

	static constexpr uint8_t TanTable[] PROGMEM
	{
		18, 22, 27, 31, 36, 41, 45, 50, 54, 59, 64, 68, 73, 78, 83, 88, 93, 98, 103, 108, 114, 119, 124
	};

	/// <summary>
	/// Pixel-retaining rotate done with 3 consecutive skews.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	class Rotate
	{
	private:
		static constexpr uint8_t GetDimension()
		{
			return ((Width >= Height) * (Width)) | ((Width < Height) * (Height));
		}

	public:
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			switch (parameter)
			{
			case 0:
			case 360:
			case -360:
				return true;

			case 90:
			case -90:
				return InvertTransform::FlipInvertX<GetDimension(), GetDimension()>::Transform(x, y);

			case 180:
			case -180:
				return InvertTransform::Invert<GetDimension(), GetDimension()>::Transform(x, y);
			default:
				if (parameter > 360
					|| parameter < -360)
				{
					return Transform(x, y, parameter % 360);
				}
				else if (parameter > 180)
				{
					return Transform(x, y, parameter - 180)
						&& InvertTransform::Invert<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else if (parameter < -180)
				{
					return Transform(x, y, parameter + 180)
						&& InvertTransform::Invert<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else if (parameter > 90)
				{
					return Transform(x, y, (parameter - 90))
						&& InvertTransform::FlipInvertX<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else if (parameter < -90)
				{
					return Transform(x, y, (parameter + 90))
						&& InvertTransform::FlipInvertY<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else if (parameter > 45)
				{
					return Transform(x, y, parameter - 90)
						&& InvertTransform::FlipInvertX<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else if (parameter < -45)
				{
					return Transform(x, y, parameter + 90)
						&& InvertTransform::FlipInvertY<GetDimension(), GetDimension()>::Transform(x, y);
				}
				else  if (parameter > 0)
				{
#if defined(ARDUINO_ARCH_AVR)
					const int8_t h = (((int16_t)pgm_read_byte(&TanTable[parameter / 2]) * Height) / UINT8_MAX);
					const int8_t v = ((int16_t)pgm_read_byte(&SinTable[parameter]) * Width) / UINT8_MAX;
#else
					const int8_t h = (((int16_t)TanTable[parameter / 2] * Height) / UINT8_MAX);
					const int8_t v = ((int16_t)SinTable[parameter] * Width) / UINT8_MAX;
#endif
					return SkewTransform::Horizontal<GetDimension(), GetDimension()>::Transform(x, y, h)
						&& SkewTransform::Vertical<GetDimension(), GetDimension()>::Transform(x, y, -v)
						&& SkewTransform::Horizontal<GetDimension(), GetDimension()>::Transform(x, y, h);
				}
				else
				{
#if defined(ARDUINO_ARCH_AVR)
					const int8_t h = ((int16_t)pgm_read_byte(&TanTable[-(parameter / 2)]) * Height) / UINT8_MAX;
					const int8_t v = (((int16_t)pgm_read_byte(&SinTable[-parameter]) * Width) / UINT8_MAX);
#else
					const int8_t h = ((int16_t)TanTable[-(parameter / 2)] * Height) / UINT8_MAX;
					const int8_t v = (((int16_t)SinTable[-parameter] * Width) / UINT8_MAX);
#endif

					return SkewTransform::Horizontal<GetDimension(), GetDimension()>::Transform(x, y, -h)
						&& SkewTransform::Vertical<GetDimension(), GetDimension()>::Transform(x, y, v)
						&& SkewTransform::Horizontal<GetDimension(), GetDimension()>::Transform(x, y, -h);
				}
				break;
			}
		}
	};
}
#endif