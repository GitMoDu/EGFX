#ifndef _EGFX_COLOR_32_FRAME_BUFFER_h
#define _EGFX_COLOR_32_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 32 bit color frame buffer.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter32.</typeparam>
	/// <typeparam name="displayOptions">Display configuration options (mirror, rotation, inverted colors, AA).</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter32
		, typename displayOptions = DisplayOptions::Default>
	class Color32FrameBuffer final
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
		Color32FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

	protected:
		virtual void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			Buffer[offset] = Rgb::B(rawColor);
			Buffer[offset + 1] = Rgb::G(rawColor);
			Buffer[offset + 2] = Rgb::R(rawColor);
		}

		void PixelRawBlend(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Average each channel
			Buffer[offset + 2] = ((uint16_t(Buffer[offset + 2]) + uint16_t(Rgb::R(rawColor))) >> 1);
			Buffer[offset + 1] = ((uint16_t(Buffer[offset + 1]) + uint16_t(Rgb::G(rawColor))) >> 1);
			Buffer[offset] = ((uint16_t(Buffer[offset + 0]) + uint16_t(Rgb::B(rawColor))) >> 1);
		}

		void PixelRawBlendAlpha(const color_t rawColor, const pixel_t x, const pixel_t y, const uint8_t alpha) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			const uint8_t alphaInv = 255 - alpha;
			Buffer[offset + 2] = ((uint16_t(Buffer[offset + 2]) * alphaInv) + (uint16_t(Rgb::R(rawColor) * alpha))) >> 8;
			Buffer[offset + 1] = ((uint16_t(Buffer[offset + 1]) * alphaInv) + (uint16_t(Rgb::G(rawColor) * alpha))) >> 8;
			Buffer[offset] = ((uint16_t(Buffer[offset + 0]) * alphaInv) + (uint16_t(Rgb::B(rawColor) * alpha))) >> 8;
		}

		void PixelRawBlendAdd(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Add each channel, clamp to 255
			Buffer[offset + 2] = MinValue<uint16_t>(255, uint16_t(Buffer[offset + 2]) + uint16_t(Rgb::R(rawColor)));
			Buffer[offset + 1] = MinValue<uint16_t>(255, uint16_t(Buffer[offset + 1]) + uint16_t(Rgb::G(rawColor)));
			Buffer[offset] = MinValue<uint16_t>(255, uint16_t(Buffer[offset + 0]) + uint16_t(Rgb::B(rawColor)));
		}

		void PixelRawBlendSubtract(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);
			// Subtract each channel, clamp to 0
			Buffer[offset + 2] = MaxValue<int16_t>(0, int16_t(Buffer[offset + 2]) - int16_t(Rgb::R(rawColor)));
			Buffer[offset + 1] = MaxValue<int16_t>(0, int16_t(Buffer[offset + 1]) - int16_t(Rgb::G(rawColor)));
			Buffer[offset] = MaxValue<int16_t>(0, int16_t(Buffer[offset + 0]) - int16_t(Rgb::B(rawColor)));
		}

		void PixelRawBlendMultiply(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Multiply each channel, normalize to 8 bits
			Buffer[offset + 2] = ((uint16_t(Buffer[offset + 2]) * uint16_t(Rgb::R(rawColor))) >> 8);
			Buffer[offset + 1] = ((uint16_t(Buffer[offset + 1]) * uint16_t(Rgb::G(rawColor))) >> 8);
			Buffer[offset] = ((uint16_t(Buffer[offset + 0]) * uint16_t(Rgb::B(rawColor))) >> 8);
		}

		void PixelRawBlendScreen(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			// Screen blend: result = 255 - ((255 - src) * (255 - dst) / 255)
			Buffer[offset + 2] = 255 - ((uint16_t(255 - Buffer[offset + 2]) * uint16_t(255 - Rgb::R(rawColor))) >> 8);
			Buffer[offset + 1] = 255 - ((uint16_t(255 - Buffer[offset + 1]) * uint16_t(255 - Rgb::G(rawColor))) >> 8);
			Buffer[offset] = 255 - ((uint16_t(255 - Buffer[offset + 0]) * uint16_t(255 - Rgb::B(rawColor))) >> 8);
		}
	};
}
#endif