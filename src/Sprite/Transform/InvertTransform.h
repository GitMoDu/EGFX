// InvertTransform.h

#ifndef _INVERT_TRANSFORM_h
#define _INVERT_TRANSFORM_h

#include <stdint.h>

namespace InvertTransform
{
	template<const uint8_t Width,
		const uint8_t Height>
	struct InvertX
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			x = Width - 1 - x;
			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct InvertY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			y = Height - 1 - y;
			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct InvertXY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			x = Width - 1 - x;
			y = Height - 1 - y;
			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct FlipXY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t xx = x;
			x = y;
			y = xx;

			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct Flip
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t xx = x;
			x = y;
			y = xx;

			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct FlipInvertX
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t xx = x;
			x = Height - 1 - y;
			y = xx;

			return true;
		}
	};

	template<const uint8_t Width,
		const uint8_t Height>
	struct FlipInvertY
	{
		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			const uint8_t xx = x;
			x = y;
			y = Width - 1 - xx;

			return true;
		}
	};
};

#endif