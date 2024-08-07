// InvertTransform.h

#ifndef _INVERT_TRANSFORM_h
#define _INVERT_TRANSFORM_h

#include <stdint.h>

namespace InvertTransform
{
	/// <summary>
	/// Invert X Axis only.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct InvertX
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			x = Width - 1 - x;
			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};

	/// <summary>
	/// Invert Y axis only.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct InvertY
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			y = Height - 1 - y;
			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};

	/// <summary>
	/// Equivalent to a +-180 degree rotation.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct Invert
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			x = Width - 1 - x;
			y = Height - 1 - y;
			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};

	/// <summary>
	/// Flip X axis with Y axis.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct Flip
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			const uint8_t xx = x;
			x = y;
			y = xx;

			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};

	/// <summary>
	/// Flip axis and invert X axis.
	/// Equivalent to a +90 degree rotation.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct FlipInvertX
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			const uint8_t xx = x;
			x = Height - 1 - y;
			y = xx;

			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};

	/// <summary>
	/// Flip axis and invert Y axis.
	/// Equivalent to a 270 degree (-90 degree) rotation.
	/// </summary>
	/// <typeparam name="Width"></typeparam>
	/// <typeparam name="Height"></typeparam>
	template<const uint8_t Width,
		const uint8_t Height>
	struct FlipInvertY
	{
		static const bool Transform(uint8_t& x, uint8_t& y)
		{
			const uint8_t xx = x;
			x = y;
			y = Width - 1 - xx;

			return true;
		}

		static const bool Transform(uint8_t& x, uint8_t& y, const int16_t parameter)
		{
			return Transform(x, y);
		}
	};
};

#endif