#ifndef _EGFX_MONOCHROME_FRAME_BUFFER_h
#define _EGFX_MONOCHROME_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 1 bit monochrome frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter1.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = BinaryColorConverter1
		, typename displayOptions = DisplayOptions::Default>
	class MonochromeFrameBuffer final
		: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayOptions>
	{
	private:
		using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayOptions>;

	public:
		using BaseClass::BufferSize;
		using typename BaseClass::color_t;

	protected:
		using BaseClass::Buffer;

	public:
		MonochromeFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

		uint8_t GetColorDepth() const final
		{
			return 1;
		}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_t yByte = y / 8;
			const uint8_t yBit = y % 8;

			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x;

			if (rawColor > 0)
			{
				Buffer[offset] |= uint8_t(1) << yBit;
			}
			else
			{
				Buffer[offset] &= ~(uint8_t(1) << yBit);
			}
		}

		/// <summary>
		/// Optimized version 1 bit color.
		/// </summary>
		/// <param name="rawColor"></param>
		void FillRaw(const color_t rawColor) final
		{
			if (rawColor > 0)
			{
				memset(Buffer, UINT8_MAX, BufferSize);
			}
			else
			{
				memset(Buffer, 0, BufferSize);
			}
		}

		/// <summary>
		/// Optimized version 1 bit color.
		/// </summary>
		/// <param name="rawColor"></param>
		/// <param name="x1"></param>
		/// <param name="y"></param>
		/// <param name="x2"></param>
		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2) final
		{
			const uint8_t yByte = y / 8;
			const uint8_t yBit = y % 8;
			const int8_t sign = (x2 >= x1) ? 1 : -1;
			const pixel_t width = (pixel_t(sign) * (x2 - x1));
			pixel_index_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x1;

			if (rawColor > 0)
			{
				for (pixel_t i = 0; i <= width; i++, offset += sign)
				{
					Buffer[offset] |= 1 << yBit;
				}
			}
			else
			{
				for (pixel_t i = 0; i <= width; i++, offset += sign)
				{
					Buffer[offset] &= ~(1 << yBit);
				}
			}
		}

		/// <summary>
		/// Optimized version.
		/// TODO: fix address issues and rename to LineVerticalRaw.
		/// </summary>
		void LineVerticalBuggy(const color_t rawColor, const pixel_t x, const pixel_t y, const pixel_t height)
		{
			if (rawColor > 0)
			{
				if (height >= 8)
				{
					const uint8_t byte0 = y / 8;
					uint8_t byte1;
					if (y == (byte0 * 8))
					{
						byte1 = byte0;
					}
					else
					{
						byte1 = byte0 + 1;

						const uint8_t yBitStart = ((byte1 * 8) - y);
						for (uint_fast8_t i = 0; i < yBitStart; i++)
						{
							const uint8_t yByte = (y + i) / 8;
							const uint8_t yBit = (y + i) % 8;

							const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

							Buffer[offset] |= 1 << yBit;
						}
					}

					const uint8_t byteHeight = (height / 8) - byte1;
					for (uint_fast8_t i = 0; i < byteHeight; i++)
					{
						const uint8_t yByte = byte1 + i;
						const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

						Buffer[offset] = UINT8_MAX;
					}

					const uint8_t broadHeight = byteHeight * 8;
					if (height > broadHeight)
					{
						for (uint_fast8_t i = broadHeight; i < height; i++)
						{
							const uint8_t yByte = ((y + i) / 8);
							const uint8_t yBit = (y + i) % 8;

							const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

							Buffer[offset] |= 1 << yBit;
						}
					}
				}
				else
				{
					for (uint_fast8_t i = 0; i < height; i++)
					{
						const uint8_t yByte = (y + i) / 8;
						const uint8_t yBit = (y + i) % 8;

						const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

						Buffer[offset] |= 1 << yBit;
					}
				}
			}
			else
			{
				for (uint_fast8_t i = 0; i < height; i++)
				{
					const uint8_t yByte = (y + i) / 8;
					const uint8_t yBit = (y + i) % 8;

					const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

					Buffer[offset] &= ~(1 << yBit);
				}
			}
		}
	};
}
#endif