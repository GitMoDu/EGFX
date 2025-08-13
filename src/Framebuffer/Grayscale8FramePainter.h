#ifndef _EGFX_GRAYSCALE8_FRAME_PAINTER_h
#define _EGFX_GRAYSCALE8_FRAME_PAINTER_h

#include "AbstractFramePainter.h"

namespace Egfx
{
	/// <summary>
	/// A template class for painting 8-bit grayscale framebuffer, providing low-level pixel drawing operations.
	/// </summary>
	/// <typeparam name="frameWidth">The width of the frame in pixels.</typeparam>
	/// <typeparam name="frameHeight">The height of the frame in pixels.</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight>
	class Grayscale8FramePainter : public AbstractFramePainter<GrayScaleConverter8, frameWidth, frameHeight>
	{
	private:
		using Base = AbstractFramePainter<GrayScaleConverter8, frameWidth, frameHeight>;

	protected:
		using Base::Buffer;

	public:
		using Base::BufferSize;
		using typename Base::color_t;

	public:
		Grayscale8FramePainter(uint8_t* buffer = nullptr) : Base(buffer) {}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = rawColor;
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = (uint16_t(Buffer[offset]) + rawColor) >> 1;
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = (((uint16_t)Buffer[offset] * (255 - alpha)) + ((uint16_t)rawColor * alpha)) >> 8;;
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = MinValue<uint16_t>(uint16_t(Buffer[offset]) + rawColor, 255);
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = MaxValue<int16_t>(int16_t(Buffer[offset]) - rawColor, 0);
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = MinValue<uint16_t>((uint16_t(Buffer[offset]) * rawColor) >> 8, 255);
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y)
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = MinValue<int16_t>(int16_t(Buffer[offset]) + rawColor - ((Buffer[offset] * rawColor) >> 8), 255);
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

		void FillRaw(const color_t rawColor)
		{
			memset(Buffer, rawColor, BufferSize);
		}

		template<bool inverted, uint8_t Sections>
		void ClearRaw(const uint8_t section)
		{
			static constexpr size_t sectionSize = BufferSize / Sections;
			const size_t sectionOffset = sectionSize * section;
			if (inverted)
				memset(&Buffer[sectionOffset], UINT8_MAX, sectionSize);
			else
				memset(&Buffer[sectionOffset], 0, sectionSize);
		}
	};
}
#endif