#ifndef _EGFX_COLOR32_FRAME_PAINTER_h
#define _EGFX_COLOR32_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{
	/// <summary>
	/// A template class for painting 32-bit color frames, providing various pixel and blending operations on a frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight>
	class Color32FramePainter : public AbstractFramePainter<ColorConverter32, frameWidth, frameHeight>
	{
	private:
		using Base = AbstractFramePainter<ColorConverter32, frameWidth, frameHeight>;

	protected:
		using Base::Buffer;

	public:
		using Base::BufferSize;
		using typename Base::color_t;

	public:
		Color32FramePainter(uint8_t* buffer = nullptr) : Base(buffer) {}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			Buffer[offset] = Rgb::B(rawColor);
			Buffer[offset + 1] = Rgb::G(rawColor);
			Buffer[offset + 2] = Rgb::R(rawColor);
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Average each channel
			Buffer[offset + 2] = ((uint16_t(Buffer[offset + 2]) + uint16_t(Rgb::R(rawColor))) >> 1);
			Buffer[offset + 1] = ((uint16_t(Buffer[offset + 1]) + uint16_t(Rgb::G(rawColor))) >> 1);
			Buffer[offset] = ((uint16_t(Buffer[offset + 0]) + uint16_t(Rgb::B(rawColor))) >> 1);
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const uint8_t alphaInv = 255 - alpha;
			Buffer[offset + 2] = ((uint16_t(Buffer[offset + 2]) * alphaInv) + (uint16_t(Rgb::R(rawColor) * alpha))) >> 8;
			Buffer[offset + 1] = ((uint16_t(Buffer[offset + 1]) * alphaInv) + (uint16_t(Rgb::G(rawColor) * alpha))) >> 8;
			Buffer[offset] = ((uint16_t(Buffer[offset + 0]) * alphaInv) + (uint16_t(Rgb::B(rawColor) * alpha))) >> 8;
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Add each channel, clamp to 255
			Buffer[offset + 2] = uint8_t(MinValue<uint16_t>(255, uint16_t(Buffer[offset + 2]) + uint16_t(Rgb::R(rawColor))));
			Buffer[offset + 1] = uint8_t(MinValue<uint16_t>(255, uint16_t(Buffer[offset + 1]) + uint16_t(Rgb::G(rawColor))));
			Buffer[offset] = uint8_t(MinValue<uint16_t>(255, uint16_t(Buffer[offset + 0]) + uint16_t(Rgb::B(rawColor))));
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			// Subtract each channel, clamp to 0
			Buffer[offset + 2] = uint8_t(MaxValue<int16_t>(0, int16_t(Buffer[offset + 2]) - int16_t(Rgb::R(rawColor))));
			Buffer[offset + 1] = uint8_t(MaxValue<int16_t>(0, int16_t(Buffer[offset + 1]) - int16_t(Rgb::G(rawColor))));
			Buffer[offset] = uint8_t(MaxValue<int16_t>(0, int16_t(Buffer[offset + 0]) - int16_t(Rgb::B(rawColor))));
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Multiply each channel, normalize to 8 bits
			Buffer[offset + 2] = uint8_t((uint16_t(Buffer[offset + 2]) * uint16_t(Rgb::R(rawColor))) >> 8);
			Buffer[offset + 1] = uint8_t((uint16_t(Buffer[offset + 1]) * uint16_t(Rgb::G(rawColor))) >> 8);
			Buffer[offset] = uint8_t((uint16_t(Buffer[offset + 0]) * uint16_t(Rgb::B(rawColor))) >> 8);
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Screen blend: result = 255 - ((255 - src) * (255 - dst) / 255)
			Buffer[offset + 2] = 255 - ((uint16_t(255 - Buffer[offset + 2]) * uint16_t(255 - Rgb::R(rawColor))) >> 8);
			Buffer[offset + 1] = 255 - ((uint16_t(255 - Buffer[offset + 1]) * uint16_t(255 - Rgb::G(rawColor))) >> 8);
			Buffer[offset] = 255 - ((uint16_t(255 - Buffer[offset + 0]) * uint16_t(255 - Rgb::B(rawColor))) >> 8);
		}

		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2)
		{
			const int8_t sign = (y2 >= y1) ? 1 : -1;
			const pixel_t endY = (y2 + sign);
			for (pixel_t y = y1; y != endY; y += sign)
			{
				PixelRaw(rawColor, x, y);
			}
		}

		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2)
		{
			const int8_t sign = (x2 >= x1) ? 1 : -1;
			const pixel_t endX = (x2 + sign);
			for (pixel_t x = x1; x != endX; x += sign)
			{
				PixelRaw(rawColor, x, y);
			}
		}

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2)
		{
			for (pixel_t y = y1; y <= y2; y++)
			{
				LineHorizontalRaw(rawColor, x1, y, x2);
			}
		}

		void FillRaw(const color_t rawColor)
		{
			for (pixel_t y = 0; y < frameHeight; y++)
			{
				for (pixel_t x = 0; x < frameWidth; x++)
				{
					PixelRaw(rawColor, x, y);
				}
			}
		}

		template<bool inverted, uint8_t Sections>
		void ClearRaw(const uint8_t section)
		{
			static constexpr size_t sectionSize = BufferSize / Sections;
			const size_t sectionOffset = sectionSize * section;
			if (inverted)
			{
				memset(&Buffer[sectionOffset], UINT8_MAX, sectionSize);
			}
			else
			{
				memset(&Buffer[sectionOffset], 0, sectionSize);
			}
		}
	};
}
#endif