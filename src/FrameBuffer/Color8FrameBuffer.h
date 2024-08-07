// Color8FrameBuffer.h

#ifndef _COLOR_8_FRAME_BUFFER_h
#define _COLOR_8_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 8 bit color/grayscale frame buffer.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="ColorConverter">Must be an implementation of AbstractColorConverter8.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, typename ColorConverter = ColorConverter8
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class Color8FrameBuffer
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
	Color8FrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: BaseClass(buffer)
	{}

	const uint32_t GetColorDepth() const  final
	{
		return sizeof(color_t) * 8;
	}

	virtual void Fill(const RgbColor& color)
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
	virtual void WritePixel(const color_t rawColor, const uint8_t x, const uint8_t y) final
	{
		const size_t offset = (sizeof(color_t) * y * frameWidth) + x;

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
	virtual void LineHorizontal(const color_t rawColor, const uint8_t x, const uint8_t y, const uint8_t width) final
	{
		if (x >= frameWidth
			|| y >= frameHeight
			|| width > frameWidth - x)
		{
#if defined(GRAPHICS_ENGINE_DEBUG)
			Serial.println(F("LH x,y "));
			Serial.print(x);
			Serial.print(',');
			Serial.print(y);
			Serial.print('\t');
			Serial.println(width);
#endif
			return;
		}
		size_t offset = 0;

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
#endif