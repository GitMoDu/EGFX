// BinaryFrameBuffer.h

#ifndef _BINARY_FRAME_BUFFER_h
#define _BINARY_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 1 bit monochrome frame buffer, with fast color converter.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
class BinaryFrameBuffer
	: public MonochromeFrameBuffer<frameWidth, frameHeight, BinaryColorConverter1, displayMirror, displayRotation>
{
private:
	using BaseClass = MonochromeFrameBuffer<frameWidth, frameHeight, BinaryColorConverter1, displayMirror, displayRotation>;

public:
	using BaseClass::BufferSize;

public:
	BinaryFrameBuffer(uint8_t buffer[BufferSize] = nullptr)
		: BaseClass(buffer)
	{}
};
#endif