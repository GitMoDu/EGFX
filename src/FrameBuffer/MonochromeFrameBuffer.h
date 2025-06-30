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
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = BinaryColorConverter1
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class MonochromeFrameBuffer final
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
		MonochromeFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

		bool IsMonochrome() const final
		{
			return true;
		}

		uint8_t GetColorDepth() const final
		{
			return 1;
		}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const uint8_t yByte = y / 8;
			const uint8_t yBit = y % 8;

			const size_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x;

			if (((bool)(rawColor > 0) ^ (bool)Inverted))
			{
				Buffer[offset] |= 1 << yBit;
			}
			else
			{
				Buffer[offset] &= ~(1 << yBit);
			}
		}

		/// <summary>
		/// Optimized version 1 bit color.
		/// </summary>
		/// <param name="rawColor"></param>
		virtual void FillRaw(const color_t rawColor)
		{
			if (((bool)(rawColor > 0) ^ (bool)Inverted))
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
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="width"></param>
		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const uint8_t yByte = y >> 3;
			const uint8_t yBit = y % 8;

			const size_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x1;

			if (((bool)(rawColor > 0) ^ (bool)Inverted))
			{
				for (uint_fast8_t i = 0; i <= x2 - x1; i++)
				{
					Buffer[offset + i] |= 1 << yBit;
				}
			}
			else
			{
				for (uint_fast8_t i = 0; i <= x2 - x1; i++)
				{
					Buffer[offset + i] &= ~(1 << yBit);
				}
			}
		}

		/// <summary>
		/// Optimized version.
		/// TODO: fix address issues and rename to LineVerticalRaw.
		/// </summary>
		void LineVerticalBuggy(const color_t rawColor, const pixel_t x, const pixel_t y, const pixel_t height)
		{
			if (((bool)(rawColor > 0) ^ (bool)Inverted))
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