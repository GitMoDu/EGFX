#ifndef _EGFX_GRAPHICS_BUFFER_h
#define _EGFX_GRAPHICS_BUFFER_h

#include "../Platform/Platform.h"

namespace Egfx
{
	template<typename color_t>
	static constexpr size_t GetFrameBufferSize(const pixel_t width, const pixel_t height)
	{
		return sizeof(color_t) * width * height;
	}

	template<const uint8_t colorDivisor>
	static constexpr size_t GetFrameBufferLowColorSize(const pixel_t width, const pixel_t height)
	{
		return (size_t)width * (height / colorDivisor);
	}

	static constexpr size_t GetFrameBufferMonochromeSize(const pixel_t width, const pixel_t height)
	{
		return GetFrameBufferLowColorSize<8>(width, height);
	}
}

#endif