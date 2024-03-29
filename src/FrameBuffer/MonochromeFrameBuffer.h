// MonochromeFrameBuffer.h

#ifndef _MONOCHROME_FRAME_BUFFER_h
#define _MONOCHROME_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 1 bit monochrome frame buffer.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, typename ColorConverter = MonochromeColorConverter1<>
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class MonochromeFrameBuffer final
	: public AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>
{
private:
	using BaseClass = AbstractFrameBuffer<ColorConverter, frameWidth, frameHeight, displayMirror, displayRotation>;

public:
	using BaseClass::BufferSize;

protected:
	using BaseClass::Buffer;
	using BaseClass::Inverted;
	using BaseClass::DisplayMirror;
	using typename BaseClass::color_t;

public:
	MonochromeFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: BaseClass(buffer)
	{}

	virtual void Fill(const RgbColor& color) final
	{
		const uint8_t rawColor = ColorConverter::GetRawColor(color);
		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			memset(Buffer, UINT8_MAX, BufferSize);
		}
		else
		{
			memset(Buffer, 0, BufferSize);
		}
	}

protected:
	virtual void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) final
	{
		const uint8_t yByte = y / 8;
		const uint8_t yBit = y % 8;

		const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x;

		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			Buffer[offset] |= 1 << yBit;
		}
		else
		{
			Buffer[offset] &= ~(1 << yBit);
		}
	}

	/// <summary>
	/// Optimized version.
	/// </summary>
	/// <param name="rawColor"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	virtual void LineHorizontal(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t width) final
	{
		uint8_t x1 = 0;
		uint8_t yByte = 0;
		uint8_t yBit = 0;

		//TODO: Handle rotation. This switches Width and Height around.
		switch (DisplayMirror)
		{
		case DisplayMirrorEnum::MirrorY:
			x1 = x;
			yByte = (frameHeight - 1 - y) / 8;
			yBit = (frameHeight - 1 - y) % 8;
			break;
		case DisplayMirrorEnum::MirrorXY:
			x1 = frameWidth - width - x;
			yByte = (frameHeight - 1 - y) / 8;
			yBit = (frameHeight - 1 - y) % 8;
			break;
		case DisplayMirrorEnum::MirrorX:
			x1 = frameWidth - width - x;
			yByte = y / 8;
			yBit = y % 8;
			break;
		case DisplayMirrorEnum::NoMirror:
		default:
			x1 = x;
			yByte = y / 8;
			yBit = y % 8;
			break;
		}

		if (((bool)(rawColor > 0) ^ (bool)Inverted))
		{
			for (uint_fast8_t i = 0; i < width; i++)
			{
				const size_t offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1 + i;
				Buffer[offset] |= 1 << yBit;
			}
		}
		else
		{
			for (uint_fast8_t i = 0; i < width; i++)
			{
				const size_t  offset = (sizeof(color_t) * ((size_t)yByte * frameWidth)) + x1 + i;
				Buffer[offset] &= ~(1 << yBit);
			}
		}
	}
};
#endif