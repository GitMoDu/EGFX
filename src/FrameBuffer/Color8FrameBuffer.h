// Color8FrameBuffer.h

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
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, typename ColorConverter = ColorConverter8
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
		, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
	class Color8FrameBuffer
		: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror, displayRotation>
	{
	private:
		using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror, displayRotation>;

	public:
		using BaseClass::BufferSize;

	protected:
		using BaseClass::Buffer;
		using BaseClass::Inverted;
		using BaseClass::DisplayMirror;
		using typename BaseClass::color_t;

	public:
		Color8FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
			: BaseClass(buffer)
		{
		}

		const bool IsMonochrome() const final
		{
			return false;
		}

		const uint8_t GetColorDepth() const final
		{
			return sizeof(color_t) * 8;
		}

		virtual void Fill(const rgb_color_t color)
		{
			if (Inverted)
			{
				memset(Buffer, ColorConverter::GetRawColor(color), BufferSize);
			}
			else
			{
				memset(Buffer, ~ColorConverter::GetRawColor(color), BufferSize);
			}
		}

	protected:
		virtual void WritePixel(const color_t rawColor, const pixel_t x, const pixel_t y) final
		{
			const coordinate_t offset = (sizeof(color_t) * y * frameWidth) + x;

			if (Inverted)
			{
				Buffer[offset] = ~rawColor;
			}
			else
			{
				Buffer[offset] = rawColor;
			}
		}

		/// <summary>
		/// Optimized version for color_t = uint8_t.
		/// </summary>
		virtual void LineHorizontal(const uint8_t rawColor, const pixel_t x, const pixel_t y, const pixel_t width) final
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			if (x >= frameWidth
				|| y >= frameHeight
				|| width > frameWidth - x)
			{
				Serial.println(F("LH x,y "));
				Serial.print(x);
				Serial.print(',');
				Serial.print(y);
				Serial.print('\t');
				Serial.println(width);
				return;
			}
#endif
			coordinate_t offset = 0;

			//TODO: Handle rotation. This switches Width and Height around.
			switch (DisplayMirror)
			{
			case DisplayMirrorEnum::MirrorX:
				offset = (sizeof(color_t) * y * frameWidth) + (frameWidth - x) - width;
				break;
			case DisplayMirrorEnum::MirrorXY:
				offset = (sizeof(color_t) * (frameHeight - 1 - y) * frameWidth) + (frameWidth - x) - width;
				break;
			case DisplayMirrorEnum::MirrorY:
			case DisplayMirrorEnum::NoMirror:
			default:
				offset = (sizeof(color_t) * y * frameWidth) + x;
				break;
			}

			if (Inverted)
			{
				memset(&Buffer[offset], ~rawColor, width);
			}
			else
			{
				memset(&Buffer[offset], rawColor, width);
			}
		}
	};
}
#endif