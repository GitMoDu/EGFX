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
	/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter32
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	class Color32FrameBuffer
		: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>
	{
	private:
		using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror>;

	public:
		using BaseClass::BufferSize;
		using typename BaseClass::color_t;

	protected:
		using BaseClass::Buffer;
		using BaseClass::Inverted;
		using BaseClass::DisplayMirror;

	public:
		Color32FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

		bool IsMonochrome() const final
		{
			return false;
		}

		uint8_t GetColorDepth() const final
		{
			return sizeof(color_t) * 8;
		}

	protected:
		virtual void PixelRaw(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const pixel_index_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

			if (Inverted)
			{
				Buffer[offset] = ~Rgb::B(rawColor);
				Buffer[offset + 1] = ~Rgb::G(rawColor);
				Buffer[offset + 2] = ~Rgb::R(rawColor);
			}
			else
			{
				Buffer[offset] = Rgb::B(rawColor);
				Buffer[offset + 1] = Rgb::G(rawColor);
				Buffer[offset + 2] = Rgb::R(rawColor);
			}
		}
	};
}
#endif