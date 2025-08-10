#ifndef _EGFX_BINARY_FRAME_PAINTER_h
#define _EGFX_BINARY_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{

	/// <summary>
	/// A template class for painting binary (1-bit per pixel) framebuffer, providing low-level pixel drawing operations.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	/// <typeparam name="colorThreshold">The threshold value used by the binary color converter to determine pixel on/off state.</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight, const uint8_t colorThreshold>
	class BinaryFramePainter : public AbstractFramePainter<BinaryColorConverter1<colorThreshold>, frameWidth, frameHeight>
	{
	private:
		using Base = AbstractFramePainter<BinaryColorConverter1<colorThreshold>, frameWidth, frameHeight>;

	protected:
		using Base::Buffer;

	public:
		using Base::BufferSize;
		using typename Base::color_t;

	private:
		uint16_t AlphaRng = 42;

	public:
		BinaryFramePainter(uint8_t* buffer = nullptr) : Base(buffer) {}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
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

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const bool currentPixel = GetPixelRaw(x, y);
			const bool newPixel = rawColor > 0;

			// If pixels are already the same, no change needed
			if (currentPixel == newPixel)
				return;

			// Use 50/50 dithering to decide whether to draw the pixel
			if (Dither(INT8_MAX))
			{
				PixelRaw(rawColor, x, y);
			}
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const bool currentPixel = GetPixelRaw(x, y);
			const bool newPixel = rawColor > 0;

			// If pixels are already the same, no change needed
			if (currentPixel == newPixel)
				return;

			// Use dithering to decide whether to draw the pixel based on alpha
			if (Dither(alpha))
			{
				PixelRaw(rawColor, x, y);
			}
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const bool currentPixel = GetPixelRaw(x, y);

			// In add mode, if current pixel is already white, it stays white
			if (currentPixel)
				return;

			// If new pixel is white, chance to turn current pixel white
			if (rawColor > 0 && Dither(INT8_MAX))
			{
				PixelRaw(1, x, y);
			}
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const bool currentPixel = GetPixelRaw(x, y);

			// In subtract mode, if current pixel is already black, it stays black
			if (!currentPixel)
				return;

			// If new pixel is white, chance to turn current pixel black
			if (rawColor > 0 && Dither(INT8_MAX))
			{
				PixelRaw(0, x, y);
			}
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const bool currentPixel = GetPixelRaw(x, y);
			const bool newPixel = rawColor > 0;

			// In multiply mode, result is white only if both are white
			if (currentPixel && !newPixel)
			{
				PixelRaw(0, x, y); // Change from white to black
			}
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const bool currentPixel = GetPixelRaw(x, y);
			const bool newPixel = rawColor > 0;

			// In screen mode, result is black only if both are black
			if (!currentPixel && newPixel)
			{
				PixelRaw(1, x, y); // Change from black to white
			}
		}

		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
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

		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
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

		void FillRaw(const color_t rawColor)
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

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			for (pixel_t y = y1; y < y2; y++)
			{
				LineHorizontalRaw(rawColor, x1, y, x2);
			}
		}

	private:
		bool GetPixelRaw(const pixel_t x, const pixel_t y) const
		{
			const pixel_t yByte = y / 8;
			const uint8_t yBit = y % 8;
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * yByte) + x;

			return (Buffer[offset] & (uint8_t(1) << yBit)) != 0;
		}

		bool Dither(const uint8_t alpha)
		{
			AlphaRng ^= AlphaRng << 7;
			AlphaRng ^= AlphaRng >> 9;
			AlphaRng ^= AlphaRng << 8;

			return alpha >= uint8_t(AlphaRng);
		}
	};
}
#endif