// BinaryFrameBuffer.h

#ifndef _EGFX_BINARY_FRAME_BUFFER_h
#define _EGFX_BINARY_FRAME_BUFFER_h

#include "AbstractFrameBuffer.h"

namespace Egfx
{
	/// <summary>
	/// 1 bit monochrome frame buffer, with fast color converter.
	/// </summary>
	/// <typeparam name="frameWidth">Frame buffer width [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="frameHeight">Frame buffer height [0;Egfx::MAX_PIXEL_SIZE].</typeparam>
	/// <typeparam name="clearDivisorPower">Frame buffer clear will be divided into sections. The divisor is set by the power of 2, keeping it a multiple of 2.</typeparam>
	/// <typeparam name="displayAxis">Display mirror option.</typeparam>
	/// <typeparam name="displayRotation">Display rotation option.</typeparam>
	template<const pixel_t frameWidth, const pixel_t frameHeight
		, const uint8_t clearDivisorPower = 0
		, DisplayMirrorEnum displayMirror = DisplayMirrorEnum::NoMirror>
	using BinaryFrameBuffer = MonochromeFrameBuffer<frameWidth, frameHeight, clearDivisorPower, BinaryColorConverter1, displayMirror>;
}
#endif