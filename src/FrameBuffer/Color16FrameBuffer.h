// Color16FrameBuffer.h

#ifndef _COLOR_16_FRAME_BUFFER_h
#define _COLOR_16_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 16 bit color frame buffer.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, const uint8_t clearDivisorPower = 0
	, typename ColorConverter = ColorConverter16
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class Color16FrameBuffer
	: public AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror, displayRotation>
{
private:
	using BaseClass = AbstractFrameBuffer<ColorConverter, clearDivisorPower, frameWidth, frameHeight, displayMirror, displayRotation>;

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

	const uint32_t GetColorDepth() const final
	{
		return sizeof(color_t) * 8;
	}

protected:
	virtual void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) final
	{
		const size_t offset = ((sizeof(color_t) * frameWidth) * y) + (sizeof(color_t) * x);

		if (Inverted)
		{
			Buffer[offset] = ~((uint8_t)(rawColor >> 8));
			Buffer[offset + 1] = ~((uint8_t)rawColor);
		}
		else
		{
			Buffer[offset] = ((uint8_t)(rawColor >> 8));
			Buffer[offset + 1] = ((uint8_t)rawColor);
		}
	}
};
#endif