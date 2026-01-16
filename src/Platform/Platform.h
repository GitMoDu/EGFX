#ifndef _EGFX_PLATFORM_h
#define _EGFX_PLATFORM_h

#include <stdint.h>

#include <IntegerSignal.h>
#include <IntegerTrigonometry.h>

#if defined(ARDUINO)
#include <Arduino.h>
#endif

/// <summary>
/// EGFX Platform 32 bit: platform with int size > 2 bytes.
/// </summary>
#if defined(ARDUINO_ARCH_NRF52) || (defined(__SIZEOF_INT__) && (__SIZEOF_INT__ > 2))
#define EGFX_PLATFORM_32BIT
#endif

/// <summary>
/// Enabled: use internal 8-8-8 color for rendering.
/// Disabled: use internal 5-6-5 color for rendering.
/// Defaults to HDR on 32 bit platforms.
/// </summary>
#if !defined(EGFX_PLATFORM_HDR) && (!defined(ARDUINO) || defined(EGFX_PLATFORM_32BIT) ) 
#define EGFX_PLATFORM_HDR
#endif

namespace Egfx
{
	using namespace IntegerSignal;
	using namespace IntegerSignal::Trigonometry;

	/// <summary>
	/// Pixel coordinate type.
	/// </summary>
	using pixel_t = int16_t;

#if defined(EGFX_PLATFORM_32BIT)
	using pixel_index_t = int32_t;
#else
	using pixel_index_t = int16_t;
#endif

#if defined(EGFX_PLATFORM_HDR)
	/// <summary>
	/// Native EGFX HDR rgb color.
	/// 8-8-8 24 bit color in a uint32_t wrapper.
	/// </summary>
	using rgb_color_t = uint32_t;
#else
	/// <summary>
	/// Native EGFX rgb color.
	/// 5-6-5 16 bit color in a uint16_t wrapper.
	/// </summary>
	using rgb_color_t = uint16_t;
#endif

	/// <summary>
	/// Point defined by two pixel coordinates.
	/// </summary>
	struct pixel_point_t
	{
		pixel_t x;
		pixel_t y;
	};

	/// <summary>
	/// Line segment defined by start and end points.
	/// </summary>
	struct pixel_line_t
	{
		pixel_point_t start;
		pixel_point_t end;
	};

	/// <summary>
	/// Triangle defined by three points.
	/// </summary>
	struct pixel_triangle_t
	{
		pixel_point_t a;
		pixel_point_t b;
		pixel_point_t c;
	};

	/// <summary>
	/// Rectangle defined by top-left and bottom-right points.
	/// </summary>
	struct pixel_rectangle_t
	{
		pixel_point_t topLeft;
		pixel_point_t bottomRight;
	};
}

#endif