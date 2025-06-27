#ifndef _EGFX_PLATFORM_h
#define _EGFX_PLATFORM_h

#include <stdint.h>

#include <IntegerSignal.h>
#include <IntegerTrigonometry.h>


/// <summary>
/// Enabled: use internal 8-8-8 color for rendering.
/// Disabled: use internal 5-6-5 color for rendering.
/// Defaults to HDR on 64 bit platforms.
/// </summary>
#if !defined(EGFX_PLATFORM_HDR) && !defined(ARDUINO)
#define EGFX_PLATFORM_HDR
#endif

/// <summary>
/// EGFX Platform Big: suports 8000x8000 screens.
/// EGFX Platform Small: suports 128x128 screens.
/// Defaults to Big on 32 bit platforms.
/// </summary>
#if !defined(EGFX_PLATFORM_BIG) && (defined(__SIZEOF_INT__) && (__SIZEOF_INT__ > 2)) || (!defined(ARDUINO))
#define EGFX_PLATFORM_BIG
#endif

namespace Egfx
{
	using namespace IntegerSignal;
	using namespace IntegerSignal::Trigonometry;

#if defined(EGFX_PLATFORM_BIG)
	typedef int16_t pixel_t;
	typedef int32_t pixel_index_t;

	/// <summary>
	/// Supports 8000x8000 screens.
	/// </summary>
	static constexpr pixel_t MAX_PIXEL_SIZE = 8000;

	/// <summary>
	/// Line/Triangle sampling factor.
	/// </summary>
	static constexpr uint8_t BRESENHAM_SCALE = 16;
#else
	typedef int16_t pixel_t;
	typedef int16_t pixel_index_t;

	/// <summary>
	/// Supports 128x128 screens.
	/// </summary>
	static constexpr pixel_index_t MAX_PIXEL_SIZE = 128;

	/// <summary>
	/// Line/Triangle sampling factor.
	/// </summary>
	static constexpr uint8_t BRESENHAM_SCALE = 8;
#endif

#if defined(EGFX_PLATFORM_HDR)
	/// <summary>
	/// Native EGFX HDR rgb color.
	/// 8-8-8 24 bit color in a uint32_t wrapper.
	/// </summary>
	typedef uint32_t rgb_color_t;
#else
	/// <summary>
	/// Native EGFX rgb color.
	/// 5-6-5 16 bit color in a uint16_t wrapper.
	/// </summary>
	typedef uint16_t rgb_color_t;
#endif
}

#endif