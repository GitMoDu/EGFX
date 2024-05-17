// GraphicsBuffer.h

#ifndef _GRAPHICS_BUFFER_h
#define _GRAPHICS_BUFFER_h

#include <stdint.h>

namespace GraphicsBuffer
{
	template<typename color_t>
	static constexpr size_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return (size_t)width * height * sizeof(color_t);
	}

	template<const uint8_t colorDivisor>
	static constexpr size_t GetLowColorBufferSize(const uint8_t width, const uint8_t height)
	{
		return (size_t)width * (height / colorDivisor);
	}

	static constexpr size_t GetMonochromeBufferSize(const uint8_t width, const uint8_t height)
	{
		return GetLowColorBufferSize<8>(width, height);
	}
}

#endif