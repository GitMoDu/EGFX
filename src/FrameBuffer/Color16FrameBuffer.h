#ifndef _EGFX_COLOR_16_FRAME_BUFFER_h
#define _EGFX_COLOR_16_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 16 bit color frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter16.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<pixel_t frameWidth, pixel_t frameHeight
		, uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter16
		, typename displayOptions = DisplayOptions::Default>
	class Color16FrameBuffer final
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
		Color16FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

	protected:
		void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			Buffer[offset] = uint8_t(rawColor >> 8);
			Buffer[offset + 1] = (uint8_t)rawColor;
		}

		/// <summary>
		/// Optimized version for color_t = uint16_t.
		/// </summary>
		void LineVerticalRaw(const color_t rawColor, const pixel_t x, const pixel_t y1, const pixel_t y2) final
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

		/// <summary>
		/// Optimized version for color_t = uint16_t.
		/// </summary>
		void LineHorizontalRaw(const color_t rawColor, const pixel_t x1, const pixel_t y, const pixel_t x2) final
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

		void RectangleFillRaw(const color_t rawColor, const pixel_t x1, const pixel_t y1, const pixel_t x2, const pixel_t y2) final
		{
			const uint8_t highColor = rawColor >> 8;
			const pixel_index_t offsetStart = (sizeof(color_t) * frameWidth * y1) + (sizeof(color_t) * x1);

			for (pixel_t y = y1; y <= y2; y++)
			{
				pixel_index_t offset = offsetStart + (sizeof(color_t) * frameWidth * (y - y1));
				for (pixel_t x = x1; x <= x2; x++)
				{
					Buffer[offset++] = highColor;
					Buffer[offset++] = (uint8_t)rawColor;
				}
			}
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y) final
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

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const color_t existingColor = (color_t)Buffer[offset] << 8 | Buffer[offset + 1];
			const color_t blendedColor = Rgb::Color565From565(
				((((uint16_t)Rgb::R5(existingColor) * (255 - alpha)) + (Rgb::R5(rawColor) * alpha)) >> 8),
				((((uint16_t)Rgb::G6(existingColor) * (255 - alpha)) + (Rgb::G6(rawColor) * alpha)) >> 8),
				((((uint16_t)Rgb::B5(existingColor) * (255 - alpha)) + (Rgb::B5(rawColor) * alpha)) >> 8));

			Buffer[offset] = uint8_t(blendedColor >> 8);
			Buffer[offset + 1] = uint8_t(blendedColor);
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y) final
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

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y) final
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

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y) final
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

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y) final
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
	};
}
#endif