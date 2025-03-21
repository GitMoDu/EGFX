// Color16FrameBuffer.h

#ifndef _EGFX_COLOR_16_FRAME_BUFFER_h
#define _EGFX_COLOR_16_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 16 bit color frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter16
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class Color16FrameBuffer
		: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>
	{
	private:
		using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>;

	public:
		using BaseClass::BufferSize;

	protected:
		using BaseClass::Buffer;
		using BaseClass::Inverted;
		using BaseClass::DisplayMirror;
		using typename BaseClass::color_t;

	public:
		Color16FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

		const bool IsMonochrome() const final
		{
			return false;
		}

		const uint8_t GetColorDepth() const final
		{
			return sizeof(color_t) * 8;
		}

	protected:
		virtual void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			if (Inverted)
			{
				Buffer[offset] = ~((uint8_t)(rawColor >> 8));
				Buffer[offset + 1] = ~((uint8_t)rawColor);
			}
			else
			{
				Buffer[offset] = ((uint8_t)(rawColor >> 8));
				Buffer[offset + 1] = ((uint8_t)rawColor);
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint16_t.
		/// </summary>
		virtual void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			constexpr pixel_index_t lineSize = sizeof(color_t) * frameWidth;
			const uint8_t high = Inverted ? ~((uint8_t)(rawColor >> 8)) : ((uint8_t)(rawColor >> 8));
			const uint8_t low = Inverted ? ~((uint8_t)rawColor) : ((uint8_t)rawColor);
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x);

			for (pixel_t i = y1; i <= y2; i++, offset += lineSize)
			{
				Buffer[offset] = high;
				Buffer[offset + 1] = low;
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint16_t.
		/// </summary>
		virtual void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const uint8_t high = Inverted ? ~((uint8_t)(rawColor >> 8)) : ((uint8_t)(rawColor >> 8));
			const uint8_t low = Inverted ? ~((uint8_t)rawColor) : ((uint8_t)rawColor);
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + (sizeof(color_t) * x1);

			for (pixel_t i = x1; i <= x2; i++, offset++)
			{
				Buffer[offset++] = high;
				Buffer[offset] = low;
			}
		}

		virtual void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			const pixel_t lineStart = (sizeof(color_t) * x1);
			const uint8_t high = Inverted ? ~((uint8_t)(rawColor >> 8)) : ((uint8_t)(rawColor >> 8));
			const uint8_t low = Inverted ? ~((uint8_t)rawColor) : ((uint8_t)rawColor);

			for (pixel_t y = y1; y <= y2; y++)
			{
				pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + lineStart;

				for (pixel_t i = x1; i <= x2; i++, offset++)
				{
					Buffer[offset++] = high;
					Buffer[offset] = low;
				}
			}
		}
	};
}
#endif