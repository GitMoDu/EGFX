#ifndef _EGFX_COLOR16_FRAME_PAINTER_h
#define _EGFX_COLOR16_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{
	/// <summary>
	/// A template class for painting 16-bit grayscale framebuffer, providing low-level pixel drawing operations.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	/// <typeparam name="rotated">Whether the frame is rotated (width and height swapped).</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight, bool rotated>
	class Color16FramePainter : public AbstractFramePainter<ColorConverter16, frameWidth, frameHeight, rotated>
	{
	private:
		using Base = AbstractFramePainter<ColorConverter16, frameWidth, frameHeight, rotated>;

	protected:
		using Base::Buffer;

	public:
		using Base::BufferSize;
		using typename Base::color_t;

	public:
		Color16FramePainter(uint8_t* buffer = nullptr) : Base(buffer) {}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			Buffer[offset] = uint8_t(rawColor >> 8);
			Buffer[offset + 1] = (uint8_t)rawColor;
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				(((uint16_t)Rgb::R5(existingColor) + Rgb::R5(rawColor)) >> 1),
				(((uint16_t)Rgb::G6(existingColor) + Rgb::G6(rawColor)) >> 1),
				(((uint16_t)Rgb::B5(existingColor) + Rgb::B5(rawColor)) >> 1));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];

			// Mix in RGB8 color space.
			const uint8_t r = ((((uint16_t)Rgb::R(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::R(rawColor) * alpha)) >> 8);
			const uint8_t g = ((((uint16_t)Rgb::G(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::G(rawColor) * alpha)) >> 8);
			const uint8_t b = ((((uint16_t)Rgb::B(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::B(rawColor) * alpha)) >> 8);

			// Convert back to RGB565.
			const color_t blendedColor = ((uint16_t)(r >> 3) << 11)
				| ((uint16_t)(g >> 2) << 5)
				| ((b >> 3));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				MinValue((uint16_t)(Rgb::R5(existingColor) + Rgb::R5(rawColor)), (uint16_t)31),
				MinValue((uint16_t)(Rgb::G6(existingColor) + Rgb::G6(rawColor)), (uint16_t)63),
				MinValue((uint16_t)(Rgb::B5(existingColor) + Rgb::B5(rawColor)), (uint16_t)31));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				MaxValue<int16_t>(0, int16_t(Rgb::R5(existingColor)) - Rgb::R5(rawColor)),
				MaxValue<int16_t>(0, int16_t(Rgb::G6(existingColor)) - Rgb::G6(rawColor)),
				MaxValue<int16_t>(0, int16_t(Rgb::B5(existingColor)) - Rgb::B5(rawColor)));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				((uint16_t(Rgb::R5(existingColor)) * uint16_t(Rgb::R5(rawColor))) >> 5),
				((uint16_t(Rgb::G6(existingColor)) * uint16_t(Rgb::G6(rawColor))) >> 6),
				((uint16_t(Rgb::B5(existingColor)) * uint16_t(Rgb::B5(rawColor))) >> 5));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				MinValue((uint8_t)(31 - ((31 - Rgb::R5(existingColor)) * (31 - Rgb::R5(rawColor)) >> 5)), (uint8_t)31),
				MinValue((uint8_t)(63 - ((63 - Rgb::G6(existingColor)) * (63 - Rgb::G6(rawColor)) >> 6)), (uint8_t)63),
				MinValue((uint8_t)(31 - ((31 - Rgb::B5(existingColor)) * (31 - Rgb::B5(rawColor)) >> 5)), (uint8_t)31));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			const uint8_t highColor = rawColor >> 8;
			const int8_t sign = (y2 >= y1) ? 1 : -1;
			const pixel_index_t lineSize = sign * pixel_index_t(sizeof(color_t) * frameWidth);
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x);
			const pixel_index_t offsetEnd = offset + (lineSize * ((pixel_t(sign) * (y2 - y1)) + 1));

			for (; offset != offsetEnd; offset += lineSize)
			{
				Buffer[offset] = highColor;
				Buffer[offset + 1] = (uint8_t)rawColor;
			}
		}

		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const uint8_t highColor = rawColor >> 8;
			const int8_t sign = (x2 >= x1) ? 1 : -1;
			const int8_t columnOffset = sizeof(color_t) * sign;
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + (sizeof(color_t) * x1);
			const pixel_index_t offsetEnd = offset + (sign * pixel_index_t(sizeof(color_t) * ((pixel_t(sign) * (x2 - x1)) + 1)));

			for (; offset != offsetEnd; )
			{
				Buffer[offset] = highColor;
				Buffer[offset + 1] = (uint8_t)rawColor;
				offset += columnOffset;
			}
		}

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			const uint8_t highColor = rawColor >> 8;
			const pixel_index_t rowSpan = sizeof(color_t) * frameWidth;

			for (pixel_t y = y1; y <= y2; y++)
			{
				pixel_index_t offset = (rowSpan * y) + (sizeof(color_t) * x1);
				for (pixel_t x = x1; x <= x2; x++)
				{
					Buffer[offset++] = highColor;
					Buffer[offset++] = (uint8_t)rawColor;
				}
			}
		}

		void FillRaw(const color_t rawColor)
		{
			const uint8_t highColor = rawColor >> 8;
			for (pixel_index_t offset = 0; offset < pixel_index_t(BufferSize); offset += sizeof(color_t))
			{
				Buffer[offset] = highColor;
				Buffer[offset + 1] = (uint8_t)rawColor;
			}
		}

#if defined(EGFX_PLATFORM_32BIT)
		template<bool inverted, uint8_t Sections>
		void ClearRaw(const uint8_t section)
		{
			// Compute section bounds.
			static constexpr size_t sectionSize = BufferSize / Sections;
			uint8_t* dst = &Buffer[sectionSize * section];
			size_t remaining = sectionSize;

			// Fill values: only 0x00 or 0xFF (bytes) and 0x00000000 or 0xFFFFFFFF (words).
			static constexpr uint8_t byteFill = inverted ? UINT8_MAX : 0;
			static constexpr uint32_t wordFill = inverted ? 0xFFFFFFFFu : 0x00000000u;

			// Head: align to 4-byte boundary using byte writes.
			while (((reinterpret_cast<uintptr_t>(dst) & 0x3u) != 0) && remaining)
			{
				*dst++ = byteFill;
				--remaining;
			}

			// Bulk: 32-bit stores (mildly unrolled), no extra buffers.
			uint32_t* wdst = reinterpret_cast<uint32_t*>(dst);
			size_t wordCount = remaining >> 2; // divide by 4

			while (wordCount >= 8)
			{
				wdst[0] = wordFill; wdst[1] = wordFill; wdst[2] = wordFill; wdst[3] = wordFill;
				wdst[4] = wordFill; wdst[5] = wordFill; wdst[6] = wordFill; wdst[7] = wordFill;
				wdst += 8;
				wordCount -= 8;
			}
			while (wordCount)
			{
				*wdst++ = wordFill;
				--wordCount;
			}

			// Tail: any remaining bytes (remaining % 4).
			dst = reinterpret_cast<uint8_t*>(wdst);
			remaining &= 0x3u; // modulo 4
			while (remaining)
			{
				*dst++ = byteFill;
				--remaining;
			}
		}
#else
		template<bool inverted, uint8_t Sections>
		void ClearRaw(const uint8_t section)
		{
			static constexpr size_t sectionSize = BufferSize / Sections;
			const size_t sectionOffset = sectionSize * section;
			memset(&Buffer[sectionOffset], inverted ? UINT8_MAX : 0, sectionSize);
		}
#endif
	};
}
#endif