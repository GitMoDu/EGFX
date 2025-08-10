#ifndef _EGFX_COLOR8_FRAME_PAINTER_h
#define _EGFX_COLOR8_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{
	/// <summary>
	/// A template class for painting 8-bit grayscale framebuffer, providing low-level pixel drawing operations.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight>
	class Color8FramePainter : public AbstractFramePainter<ColorConverter8, frameWidth, frameHeight>
	{
	private:
		using Base = AbstractFramePainter<ColorConverter8, frameWidth, frameHeight>;

	protected:
		using Base::Buffer;

	public:
		using Base::BufferSize;
		using typename Base::color_t;

	public:
		Color8FramePainter(uint8_t* buffer = nullptr) : Base(buffer) {}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = rawColor;
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(
				MinValue<uint8_t>((Rgb::R3(existingColor) + Rgb::R3(rawColor)) >> 1, uint8_t(7)),
				MinValue<uint8_t>((Rgb::G3(existingColor) + Rgb::G3(rawColor)) >> 1, uint8_t(7)),
				MinValue<uint8_t>((Rgb::B2(existingColor) + Rgb::B2(rawColor)) >> 1, uint8_t(3)));
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];

			// Mix in RGB8 color space.
			const uint8_t r = (((uint16_t)Rgb::R(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::R(rawColor) * alpha)) >> 8;
			const uint8_t g = (((uint16_t)Rgb::G(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::G(rawColor) * alpha)) >> 8;
			const uint8_t b = (((uint16_t)Rgb::B(existingColor) * (255 - alpha)) + ((uint16_t)Rgb::B(rawColor) * alpha)) >> 8;

			// Convert back to RGB332.
			Buffer[offset] = ((r & 0xE0)) | ((g & 0xE0) >> 3) | ((b & 0xC0) >> 6);
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<uint8_t>(Rgb::R3(existingColor) + Rgb::R3(rawColor), 7),
				MinValue<uint8_t>(uint8_t(Rgb::G3(existingColor)) + Rgb::G3(rawColor), 7),
				MinValue<uint8_t>(uint8_t(Rgb::B2(existingColor)) + Rgb::B2(rawColor), 3));
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MaxValue<int16_t>(int16_t(Rgb::R3(existingColor)) - Rgb::R3(rawColor), 0),
				MaxValue<int16_t>(int16_t(Rgb::G3(existingColor)) - Rgb::G3(rawColor), 0),
				MaxValue<int16_t>(int16_t(Rgb::B2(existingColor)) - Rgb::B2(rawColor), 0));
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<uint8_t>(uint8_t(Rgb::R3(existingColor)) * Rgb::R3(rawColor) / 7, 7),
				MinValue<uint8_t>(uint8_t(Rgb::G3(existingColor)) * Rgb::G3(rawColor) / 7, 7),
				MinValue<uint8_t>(uint8_t(Rgb::B2(existingColor)) * Rgb::B2(rawColor) / 3, 3));
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<int16_t>(int16_t(Rgb::R3(existingColor)) + Rgb::R3(rawColor) - ((Rgb::R3(existingColor) * Rgb::R3(rawColor)) / 7), 7),
				MinValue<int16_t>(int16_t(Rgb::G3(existingColor)) + Rgb::G3(rawColor) - ((Rgb::G3(existingColor) * Rgb::G3(rawColor)) / 7), 7),
				MinValue<int16_t>(int16_t(Rgb::B2(existingColor)) + Rgb::B2(rawColor) - ((Rgb::B2(existingColor) * Rgb::B2(rawColor)) / 3), 3));
		}

		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			const int8_t sign = (y2 >= y1) ? 1 : -1;
			const pixel_index_t lineSize = sign * pixel_index_t(sizeof(color_t) * frameWidth);
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x);
			const pixel_index_t offsetEnd = offset + (lineSize * ((pixel_t(sign) * (y2 - y1)) + 1));

			for (; offset != offsetEnd; offset += lineSize)
			{
				Buffer[offset] = rawColor;
			}
		}

		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const pixel_t xStart = (x2 >= x1) ? x1 : x2;
			const pixel_t xEnd = (x2 >= x1) ? x2 : x1;
			const pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + (sizeof(color_t) * xStart);

			memset(&Buffer[offset], rawColor, xEnd - xStart + 1);
		}

		void FillRaw(const color_t rawColor)
		{
			memset(Buffer, rawColor, BufferSize);
		}

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			const pixel_t width = x2 - x1 + 1;
			const int8_t sign = (y2 >= y1) ? 1 : -1;
			const pixel_index_t lineSize = sign * pixel_index_t(sizeof(color_t) * frameWidth);
			pixel_index_t offset = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x1);
			const pixel_index_t offsetEnd = offset + (lineSize * ((pixel_t(sign) * (y2 - y1)) + 1));

			for (; offset != offsetEnd; offset += lineSize)
			{
				memset(&Buffer[offset], rawColor, width);
			}
		}
	};
}
#endif