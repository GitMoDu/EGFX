// Color8FrameBuffer.h

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
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter8
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class Color8FrameBuffer
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
		virtual void FillRaw(const color_t rawColor)
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
		virtual void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
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
	};
}
#endif