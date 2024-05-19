// Color16FrameBuffer.h

#ifndef _COLOR_16_FRAME_BUFFER_h
#define _COLOR_16_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 16 bit color frame buffer.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, typename ColorConverter = ColorConverter16
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class Color16FrameBuffer
	: public AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>
{
private:
	using BaseClass = AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>;

public:
	using BaseClass::BufferSize;

protected:
	using BaseClass::Buffer;
	using BaseClass::Inverted;
	using typename BaseClass::color_t;

public:
	Color16FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: BaseClass(buffer)
	{}

protected:
	virtual void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) final
	{
		const size_t offset = (sizeof(color_t) * ((size_t)y * frameWidth)) + x;

		if (Inverted)
		{
			Buffer[offset] = ~((uint8_t)rawColor);
			Buffer[offset + 1] = ~((uint8_t)(rawColor >> 8));
		}
		else
		{
			Buffer[offset] = ((uint8_t)rawColor);
			Buffer[offset + 1] = ((uint8_t)(rawColor >> 8));
		}
	}
};
#endif