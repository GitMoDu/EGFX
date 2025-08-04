#ifndef _EGFX_COLOR_8_FRAME_BUFFER_h
#define _EGFX_COLOR_8_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 8 bit color/grayscale frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter8
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class Color8FrameBuffer final
		: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>
	{
	private:
		using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>;

	public:
		using BaseClass::BufferSize;
		using typename BaseClass::color_t;

	protected:
		using BaseClass::Buffer;
		using BaseClass::Inverted;
		using BaseClass::DisplayMirror;

	public:
		Color8FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;

			if (Inverted)
			{
				Buffer[offset] = ~rawColor;
			}
			else
			{
				Buffer[offset] = rawColor;
			}
		}

		/// <summary>
		/// Optimized version color_t = uint8_t.
		/// </summary>
		/// <param name="rawColor"></param>
		void FillRaw(const color_t rawColor) final
		{
			if (Inverted)
			{
				memset(Buffer, ~rawColor, BufferSize);
			}
			else
			{
				memset(Buffer, rawColor, BufferSize);
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2) final
		{
			constexpr pixel_index_t lineSize = sizeof(color_t) * frameWidth;
			const uint8_t value = Inverted ? ~rawColor : rawColor;
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x);

			for (pixel_t i = y1; i <= y2; i++, offset += lineSize)
			{
				Buffer[offset] = value;
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2) final
		{
			const pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + (sizeof(color_t) * x1);
			if (Inverted)
			{
				memset(&Buffer[offset], ~rawColor, x2 - x1 + 1);
			}
			else
			{
				memset(&Buffer[offset], rawColor, x2 - x1 + 1);
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			const pixel_t lineStart = (sizeof(color_t) * x1);
			const uint8_t value = Inverted ? ~rawColor : rawColor;
			const uint16_t width = x2 - x1 + 1;

			for (pixel_t y = y1; y <= y2; y++)
			{
				const pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + lineStart;
				memset(&Buffer[offset], value, width);
			}
		}
	};
}
#endif