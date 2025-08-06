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
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter8
		, typename displayOptions = DisplayOptions::Default>
	class Color8FrameBuffer final
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
		Color8FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			Buffer[offset] = rawColor;
		}

		/// <summary>
		/// Optimized version color_t = uint8_t.
		/// </summary>
		/// <param name="rawColor"></param>
		void FillRaw(const color_t rawColor) final
		{
			memset(Buffer, rawColor, BufferSize);
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2) final
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

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2) final
		{
			const pixel_t xStart = (x2 >= x1) ? x1 : x2;
			const pixel_t xEnd = (x2 >= x1) ? x2 : x1;
			const pixel_index_t offset = (sizeof(color_t) * frameWidth * y) + (sizeof(color_t) * xStart);

			memset(&Buffer[offset], rawColor, xEnd - xStart + 1);
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
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

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(
				MinValue<uint8_t>((Rgb::R3(existingColor) + Rgb::R3(rawColor)) >> 1, uint8_t(7)),
				MinValue<uint8_t>((Rgb::G3(existingColor) + Rgb::G3(rawColor)) >> 1, uint8_t(7)),
				MinValue<uint8_t>((Rgb::B2(existingColor) + Rgb::B2(rawColor)) >> 1, uint8_t(3))
			);
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];

			// Extract channels
			const uint8_t r0 = Rgb::R3(existingColor);
			const uint8_t g0 = Rgb::G3(existingColor);
			const uint8_t b0 = Rgb::B2(existingColor);

			const uint8_t r1 = Rgb::R3(rawColor);
			const uint8_t g1 = Rgb::G3(rawColor);
			const uint8_t b1 = Rgb::B2(rawColor);

			// Blend channels
			Buffer[offset] = Rgb::Color332From332(
				MinValue<uint8_t>(((uint16_t(r0) * (255 - alpha)) + (uint16_t(r1) * alpha)) >> 8, 7),
				MinValue<uint8_t>(((uint16_t(g0) * (255 - alpha)) + (uint16_t(g1) * alpha)) >> 8, 7),
				MinValue<uint8_t>(((uint16_t(b0) * (255 - alpha)) + (uint16_t(b1) * alpha)) >> 8, 3));
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<uint8_t>(Rgb::R3(existingColor) + Rgb::R3(rawColor), 7),
				MinValue<uint8_t>(uint8_t(Rgb::G3(existingColor)) + Rgb::G3(rawColor), 7),
				MinValue<uint8_t>(uint8_t(Rgb::B2(existingColor)) + Rgb::B2(rawColor), 3));
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MaxValue<int16_t>(int16_t(Rgb::R3(existingColor)) - Rgb::R3(rawColor), 0),
				MaxValue<int16_t>(int16_t(Rgb::G3(existingColor)) - Rgb::G3(rawColor), 0),
				MaxValue<int16_t>(int16_t(Rgb::B2(existingColor)) - Rgb::B2(rawColor), 0));
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<uint8_t>(uint8_t(Rgb::R3(existingColor)) * Rgb::R3(rawColor) / 7, 7),
				MinValue<uint8_t>(uint8_t(Rgb::G3(existingColor)) * Rgb::G3(rawColor) / 7, 7),
				MinValue<uint8_t>(uint8_t(Rgb::B2(existingColor)) * Rgb::B2(rawColor) / 3, 3));
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = (pixel_index_t(sizeof(color_t)) * y * frameWidth) + x;
			const color_t existingColor = Buffer[offset];
			Buffer[offset] = Rgb::Color332From332(MinValue<int16_t>(int16_t(Rgb::R3(existingColor)) + Rgb::R3(rawColor) - ((Rgb::R3(existingColor) * Rgb::R3(rawColor)) / 7), 7),
				MinValue<int16_t>(int16_t(Rgb::G3(existingColor)) + Rgb::G3(rawColor) - ((Rgb::G3(existingColor) * Rgb::G3(rawColor)) / 7), 7),
				MinValue<int16_t>(int16_t(Rgb::B2(existingColor)) + Rgb::B2(rawColor) - ((Rgb::B2(existingColor) * Rgb::B2(rawColor)) / 3), 3));
		}
	};
}
#endif