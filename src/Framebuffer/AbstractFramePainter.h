#ifndef _EGFX_ABSTRACT_FRAME_PAINTER_h
#define _EGFX_ABSTRACT_FRAME_PAINTER_h

#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"
#include <IntegerSignal.h>

namespace Egfx
{
	template<typename ColorConverter, pixel_t frameWidth, pixel_t frameHeight>
	class AbstractFramePainter : public IFrameBuffer
	{
	public:
		static constexpr size_t BufferSize = ColorConverter::BufferSize(frameWidth, frameHeight);
		static constexpr pixel_t FrameWidth = frameWidth;
		static constexpr pixel_t FrameHeight = frameHeight;
		static constexpr uint8_t ColorDepth = ColorConverter::ColorDepth;
		static constexpr bool Monochrome = ColorConverter::Monochrome;

		using color_t = typename ColorConverter::color_t;

	protected:
		uint8_t* Buffer;

	public:
		AbstractFramePainter(uint8_t* buffer = nullptr)
			: IFrameBuffer()
			, Buffer(buffer)
		{
		}

		static constexpr color_t GetRawColor(const rgb_color_t color)
		{
			return ColorConverter::GetRawColor(color);
		}

	protected:
		/// <summary>
		/// Fixed-point scale used by line and triangle rendering.
		/// </summary>
		static constexpr uint8_t BRESENHAM_SCALE = 8;

		// Fixed-point rounding helper: add half-unit for the current Bresenham scale, then arithmetic right-shift.
		static constexpr int16_t FP_ROUND_HALF = SignedLeftShift<int16_t>(1, BRESENHAM_SCALE - 1);

		/// <summary>
		/// Rounds a 32-bit fixed-point value to the nearest integer using a signed right shift.
		/// </summary>
		static constexpr pixel_t FixedRoundToInt(const int32_t fx)
		{
			return SignedRightShift(fx + FP_ROUND_HALF, BRESENHAM_SCALE);
		}

		/// <summary>
		/// Converts an integer pixel coordinate to fixed-point representation.
		/// </summary>
		static constexpr int32_t IntToFixed(const pixel_t x)
		{
			return SignedLeftShift<int32_t>(x, BRESENHAM_SCALE);
		}
	};
}
#endif