// BinaryFrameBuffer.h

#ifndef _BINARY_FRAME_BUFFER_h
#define _BINARY_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

/// <summary>
/// 1 bit monochrome frame buffer, with fast color converter.
/// </summary>
/// <typeparam name="frameWidth">Frame buffer width [0;254].</typeparam>
/// <typeparam name="frameHeight">Frame buffer height [0;254].</typeparam>
/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
/// <typeparam name="displayAxis">Display mirror option.</typeparam>
/// <typeparam name="displayRotation">Display rotation option.</typeparam>
template<const uint8_t frameWidth, const uint8_t frameHeight
	, const uint8_t clearDivisorPower = 0
	, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror
	, DisplayRotationEnum displayRotation = DisplayRotationEnum::NoRotation>
using BinaryFrameBuffer = MonochromeFrameBuffer<frameWidth, frameHeight, clearDivisorPower, BinaryColorConverter1, displayMirror, displayRotation>;

#endif