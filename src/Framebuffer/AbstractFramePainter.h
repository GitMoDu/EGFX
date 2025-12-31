#ifndef _EGFX_ABSTRACT_FRAME_PAINTER_h
#define _EGFX_ABSTRACT_FRAME_PAINTER_h

#include "../Model/ColorConverter.h"
#include "../Model/IFrameBuffer.h"
#include <IntegerSignal.h>

namespace Egfx
{
	/// <summary>
	/// Abstract base class for rendering graphics primitives to a frame buffer with configurable color conversion, dimensions, and rotation.
	/// </summary>
	/// <typeparam name="ColorConverter">The color converter type that defines color representation, bit depth, and conversion from RGB.</typeparam>
	/// <typeparam name="frameWidth">The physical width of the frame buffer in pixels.</typeparam>
	/// <typeparam name="frameHeight">The physical height of the frame buffer in pixels.</typeparam>
	/// <typeparam name="rotated">If true, the logical frame dimensions are rotated 90 degrees relative to physical dimensions.</typeparam>
	template<typename ColorConverter, pixel_t frameWidth, pixel_t frameHeight, bool rotated>
	class AbstractFramePainter : public IFrameBuffer
	{
	private:
		enum class OutcodeEnum : uint8_t
		{
			OUT_LEFT = 1,
			OUT_RIGHT = 2,
			OUT_BOTTOM = 4,
			OUT_TOP = 8
		};

	public:
		// Physical screen dimensions
		static constexpr pixel_t PhysicalWidth = frameWidth;
		static constexpr pixel_t PhysicalHeight = frameHeight;
		static constexpr size_t BufferSize = ColorConverter::BufferSize(frameWidth, frameHeight);

		// Logical dimensions (taking rotation into account)
		static constexpr pixel_t FrameWidth = rotated ? frameHeight : frameWidth;
		static constexpr pixel_t FrameHeight = rotated ? frameWidth : frameHeight;

	public:
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

	protected:
		bool ClipRectangle(pixel_point_t& topLeft, pixel_point_t& bottomRight) const
		{
			if ((topLeft.x < 0 && bottomRight.x < 0)
				|| (topLeft.y < 0 && bottomRight.y < 0)
				|| (topLeft.x >= FrameWidth && bottomRight.x >= FrameWidth)
				|| (topLeft.y >= FrameHeight && bottomRight.y >= FrameHeight)
				|| (topLeft.x > bottomRight.x || topLeft.y > bottomRight.y))
			{
				return false; // Invalid rectangle
			}

			topLeft.x = LimitValue<pixel_t>(topLeft.x, 0, FrameWidth - 1);
			topLeft.y = LimitValue<pixel_t>(topLeft.y, 0, FrameHeight - 1);
			bottomRight.x = LimitValue<pixel_t>(bottomRight.x, 0, FrameWidth - 1);
			bottomRight.y = LimitValue<pixel_t>(bottomRight.y, 0, FrameHeight - 1);

			return true;
		}

		bool ClipLine(pixel_point_t& p0, pixel_point_t& p1) const
		{
			pixel_t x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;
			uint8_t outcode0 = ComputeOutCode(x0, y0);
			uint8_t outcode1 = ComputeOutCode(x1, y1);
			pixel_t x = 0;
			pixel_t y = 0;
			while (true)
			{
				if (!(outcode0 | outcode1))
				{
					p0 = { x0 , y0 };
					p1 = { x1 , y1 };
					return true;

				}
				else if (outcode0 & outcode1)
				{
					return false;
				}
				else
				{
					uint8_t outcodeOut = outcode0 ? outcode0 : outcode1;
					x = 0;
					y = 0;

					if (outcodeOut & (uint8_t)OutcodeEnum::OUT_TOP)
					{
						if (y1 == y0) return false;
						x = x0 + int32_t(x1 - x0) * (0 - y0) / (y1 - y0);
						y = 0;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_BOTTOM)
					{
						if (y1 == y0) return false;
						x = x0 + int32_t(x1 - x0) * (FrameHeight - 1 - y0) / (y1 - y0);
						y = FrameHeight - 1;
					}
					else if (outcodeOut & (uint8_t)OutcodeEnum::OUT_RIGHT)
					{
						if (x1 == x0) return false;
						y = y0 + int32_t(y1 - y0) * (FrameWidth - 1 - x0) / (x1 - x0);
						x = FrameWidth - 1;
					}
					else
					{
						if (x1 == x0) return false;
						y = y0 + int32_t(y1 - y0) * (0 - x0) / (x1 - x0);
						x = 0;
					}

					if (outcodeOut == outcode0)
					{
						x0 = x; y0 = y;
						outcode0 = ComputeOutCode(x0, y0);
					}
					else
					{
						x1 = x; y1 = y;
						outcode1 = ComputeOutCode(x1, y1);
					}
				}
			}
		}

		uint8_t ComputeOutCode(const pixel_t x, const pixel_t y) const
		{
			uint8_t code = 0;
			if (x < 0) code |= (uint8_t)OutcodeEnum::OUT_LEFT;
			else if (x >= FrameWidth) code |= (uint8_t)OutcodeEnum::OUT_RIGHT;
			if (y < 0) code |= (uint8_t)OutcodeEnum::OUT_TOP;
			else if (y >= FrameHeight) code |= (uint8_t)OutcodeEnum::OUT_BOTTOM;

			return code;
		}
	};
}
#endif