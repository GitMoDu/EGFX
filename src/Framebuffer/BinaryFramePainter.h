#ifndef _EGFX_BINARY_FRAME_PAINTER_h
#define _EGFX_BINARY_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{
	/// <summary>
	/// A lookup table for bit masks corresponding to each bit position in a byte.
	/// </summary>
	static constexpr uint8_t BitMaskBits[8] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7 };

	/// <summary>
	/// A template class for painting binary (1-bit per pixel) framebuffer, providing low-level pixel drawing operations.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	/// <typeparam name="colorThreshold">The threshold value used by the binary color converter to determine pixel on/off state.</typeparam>
	/// <typeparam name="rotated">Whether the frame is rotated (width and height swapped).</typeparam>
	template<uint16_t frameWidth,
		uint16_t frameHeight,
		uint8_t colorThreshold,
		bool rotated>
	class BinaryFramePainter : public AbstractFramePainter<BinaryColorConverter1<colorThreshold>, frameWidth, frameHeight, rotated>
	{
	private:
		using Base = AbstractFramePainter<BinaryColorConverter1<colorThreshold>, frameWidth, frameHeight, rotated>;

		static constexpr int16_t MaximumDimension()
		{
			return MaxValue<int16_t>(frameWidth, frameHeight);
		}

		// Optimized axis iterator type based on max frame dimension.
		using axis_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
			uint_fast8_t,
			uint_fast16_t,
			(MaximumDimension() <= UINT8_MAX)
		>::type;

		using offset_t = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type <
			uint_fast16_t,
			size_t,
			(Base::BufferSize <= UINT16_MAX)
		>::type;


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
			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];

			if (rawColor > 0)
			{
				Buffer[offset] |= mask;
			}
			else
			{
				Buffer[offset] &= ~mask;
			}
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			PixelRawBlendAlpha(rawColor, x, y, UINT8_MAX / 2); // Use 50% alpha for blending
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];
			const bool currentPixel = Buffer[offset] & mask;
			const bool newPixel = rawColor > 0;

			// If pixels are already the same, no change needed
			if (currentPixel == newPixel)
				return;

			// Use dithering to decide whether to draw the pixel based on alpha
			if (Dither(alpha))
			{
				if (newPixel)
				{
					Buffer[offset] |= mask;
				}
				else
				{
					Buffer[offset] &= ~mask;
				}
			}
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			if (rawColor == 0)
				return;

			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];
			const bool currentPixel = Buffer[offset] & mask;

			// In add mode, if current pixel is already white, it stays white
			if (currentPixel)
				return;

			Buffer[offset] |= mask;
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			if (rawColor == 0)
				return;

			const bool currentPixel = GetPixelRaw(x, y);

			// In subtract mode, if current pixel is already black, it stays black
			if (currentPixel)
				return;

			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];
			Buffer[offset] &= ~mask;
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];
			const bool currentPixel = Buffer[offset] & mask;
			const bool newPixel = rawColor > 0;

			// In multiply mode, result is white only if both are white
			if (currentPixel && !newPixel)
			{
				Buffer[offset] |= mask;
			}
			else
			{
				Buffer[offset] &= ~mask;
			}
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];
			const bool currentPixel = Buffer[offset] & mask;
			const bool newPixel = rawColor > 0;

			// In screen mode, result is black only if both are black
			if (!currentPixel && newPixel)
			{
				Buffer[offset] |= mask;
			}
			else
			{
				Buffer[offset] &= ~mask;
			}
		}

		void PixelRawBlendXor(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			if (rawColor > 0)
			{
				const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
				const uint8_t mask = BitMaskBits[y & 7];
				Buffer[offset] ^= mask;
			}
		}

		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			const axis_t yStart = (y1 <= y2) ? y1 : y2;
			const axis_t yEnd = (y1 <= y2) ? y2 : y1;
			const axis_t startByte = yStart >> 3;
			const axis_t endByte = yEnd >> 3;
			const offset_t rowStride = static_cast<offset_t>(sizeof(color_t)) * frameWidth;
			const offset_t baseOffset = (rowStride * startByte) + x;

			const uint8_t startBit = yStart & 7;
			const uint8_t endBit = yEnd & 7;

			if (startByte == endByte)
			{
				const uint8_t mask = ((uint8_t(0xFF) << startBit) & (uint8_t(0xFF) >> (7 - endBit)));
				uint8_t& target = Buffer[baseOffset];
				if (rawColor > 0) target |= mask;
				else target &= ~mask;
				return;
			}

			// First byte - partial
			uint8_t* ptr = &Buffer[baseOffset];
			if (startBit > 0)
			{
				const uint8_t firstByteMask = uint8_t(0xFF) << startBit;
				if (rawColor > 0) *ptr |= firstByteMask;
				else *ptr &= ~firstByteMask;
				ptr += rowStride;
			}
			else
			{
				*ptr = (rawColor > 0) ? 0xFF : 0x00;
				ptr += rowStride;
			}

			// Middle bytes - full bytes
			const uint8_t fillByte = rawColor > 0 ? 0xFF : 0x00;
			for (axis_t byteIndex = startByte + 1; byteIndex < endByte; byteIndex++, ptr += rowStride)
			{
				*ptr = fillByte;
			}

			// Last byte - partial
			const uint8_t lastByteMask = uint8_t(0xFF) >> (7 - endBit);
			if (rawColor > 0) *ptr |= lastByteMask;
			else *ptr &= ~lastByteMask;
		}

		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const axis_t xStart = (x1 <= x2) ? x1 : x2;
			const axis_t xEnd = (x1 <= x2) ? x2 : x1;
			const axis_t width = static_cast<axis_t>(xEnd - xStart) + 1;

			const offset_t rowOffset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + xStart;
			const uint8_t mask = BitMaskBits[y & 7];
			uint8_t* ptr = &Buffer[rowOffset];

			if (rawColor > 0)
			{
				for (axis_t i = 0; i < width; i++) *ptr++ |= mask;
			}
			else
			{
				for (axis_t i = 0; i < width; i++) *ptr++ &= ~mask;
			}
		}

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			const axis_t xStart = (x1 <= x2) ? x1 : x2;
			const axis_t xEnd = (x1 <= x2) ? x2 : x1;
			const axis_t yStart = (y1 <= y2) ? y1 : y2;
			const axis_t yEnd = (y1 <= y2) ? y2 : y1;
			const axis_t startPage = yStart >> 3;
			const axis_t endPage = yEnd >> 3;
			const offset_t rowStride = static_cast<offset_t>(sizeof(color_t)) * frameWidth;
			const axis_t width = static_cast<axis_t>(xEnd - xStart) + 1;
			const uint8_t fillByte = rawColor > 0 ? UINT8_MAX : 0;

			for (axis_t page = startPage; ; page++)
			{
				const uint8_t firstBit = (page == startPage) ? (yStart & 7) : 0;
				const uint8_t lastBit = (page == endPage) ? (yEnd & 7) : 7;
				const offset_t pageOffset = static_cast<offset_t>(page) * rowStride + xStart;

				if (firstBit == 0 && lastBit == 7)
				{
					memset(&Buffer[pageOffset], fillByte, width);
				}
				else
				{
					const uint8_t mask = (uint8_t(0xFF) << firstBit) & (uint8_t(0xFF) >> (7 - lastBit));
					for (axis_t x = 0; x < width; x++)
					{
						uint8_t& target = Buffer[pageOffset + x];
						if (rawColor > 0)
							target |= mask;
						else
							target &= static_cast<uint8_t>(~mask);
					}
				}

				if (page == endPage)
					break;
			}
		}

		void FillRaw(const color_t rawColor)
		{
			memset(Buffer, rawColor, BufferSize);
		}

		template<bool inverted, uint8_t Sections>
		void ClearRaw(const uint8_t section)
		{
			static constexpr offset_t SectionSize = BufferSize / Sections;

			const offset_t sectionOffset = SectionSize * section;
			if (inverted)
			{
				memset(&Buffer[sectionOffset], UINT8_MAX, SectionSize);
			}
			else
			{
				memset(&Buffer[sectionOffset], 0, SectionSize);
			}
		}

	private:
		bool GetPixelRaw(const pixel_t x, const pixel_t y) const
		{
			const offset_t offset = (static_cast<offset_t>(sizeof(color_t)) * frameWidth * (y >> 3)) + x;
			const uint8_t mask = BitMaskBits[y & 7];

			return (Buffer[offset] & mask) != 0;
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