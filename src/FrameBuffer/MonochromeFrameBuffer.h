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
		/// Optimized version for 1 bit color.
		/// </summary>
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
		/// Optimized version for 1 bit color.
		/// </summary>
		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2) final
		{
			const pixel_t yStart = (y1 <= y2) ? y1 : y2;
			const pixel_t yEnd = (y1 <= y2) ? y2 : y1;
			const pixel_t startByte = yStart / 8;
			const pixel_t endByte = yEnd / 8;
			const pixel_index_t xOffset = x;

			if (startByte == endByte)
			{
				// Single byte case
				const uint8_t startBit = yStart % 8;
				const uint8_t endBit = yEnd % 8;
				const uint8_t mask = ((uint8_t(0xFF) << startBit) & (uint8_t(0xFF) >> (7 - endBit)));
				const pixel_index_t offset = (sizeof(color_t) * frameWidth * startByte) + xOffset;

				if (rawColor > 0)
					Buffer[offset] |= mask;
				else
					Buffer[offset] &= ~mask;

				return;
			}

			// First byte - partial
			const uint8_t startBit = yStart % 8;
			const pixel_index_t startOffset = (sizeof(color_t) * frameWidth * startByte) + xOffset;

			if (startBit > 0)
			{
				const uint8_t firstByteMask = uint8_t(0xFF) << startBit;
				if (rawColor > 0)
					Buffer[startOffset] |= firstByteMask;
				else
					Buffer[startOffset] &= ~firstByteMask;
			}
			else
			{
				Buffer[startOffset] = (rawColor > 0) ? 0xFF : 0x00;
			}

			// Middle bytes - full bytes
			for (pixel_t byteIndex = startByte + 1; byteIndex < endByte; byteIndex++)
			{
				const pixel_index_t midOffset = (sizeof(color_t) * frameWidth * byteIndex) + xOffset;
				Buffer[midOffset] = (rawColor > 0) ? 0xFF : 0x00;
			}

			// Last byte - partial
			const uint8_t endBit = yEnd % 8;
			const uint8_t lastByteMask = uint8_t(0xFF) >> (7 - endBit);
			const pixel_index_t endOffset = (sizeof(color_t) * frameWidth * endByte) + xOffset;

			if (rawColor > 0)
				Buffer[endOffset] |= lastByteMask;
			else
				Buffer[endOffset] &= ~lastByteMask;
		}

		/// <summary>
		/// Optimized version for 1 bit color.
		/// </summary>
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
	};
}
#endif