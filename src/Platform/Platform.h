// Platform.h

#ifndef _EGFX_PLATFORM_h
#define _EGFX_PLATFORM_h

#include <stdint.h>

/// <summary>
/// Enabled: use internal 8-8-8 color for rendering.
/// Disabled: use internal 5-6-5 color for rendering.
/// Defaults to HDR on 64 bit platforms.
/// </summary>
#if !defined(EGFX_PLATFORM_HDR) && defined(__SIZEOF_INT__) && (__SIZEOF_INT__ > 4)
#define(EGFX_PLATFORM_HDR)
#endif

/// <summary>
/// EGFX Platform Big: suports 65535x65535 screens.
/// EGFX Platform Small: suports 255x255 screens.
/// Defaults to Big on 32 bit platforms.
/// </summary>
#if !defined(EGFX_PLATFORM_BIG) && defined(__SIZEOF_INT__) && (__SIZEOF_INT__ > 2)
#define EGFX_PLATFORM_BIG
#endif

namespace Egfx
{
#if defined(EGFX_PLATFORM_BIG)
	/// <summary>
	/// Big variant of platform defines, supports screens up to 65535x65535.
	/// </summary>
	typedef uint16_t pixel_t;

	typedef int16_t pixel_signed_t;
	typedef uint32_t coordinate_t;
	typedef int32_t coordinate_signed_t;

	/// <summary>
	/// Suports 65535x65535 screens.
	/// </summary>
	static constexpr pixel_t MAX_PIXEL_SIZE = UINT16_MAX;
#else
	/// <summary>
	/// Small variant of platform defines, supports screens up to 255x255.
	/// </summary>
	typedef uint8_t pixel_t;

	typedef int8_t pixel_signed_t;
	typedef uint16_t coordinate_t;
	typedef int16_t coordinate_signed_t;

	/// <summary>
	/// Suports 255x255 screens.
	/// </summary>
	static constexpr pixel_t MAX_PIXEL_SIZE = UINT8_MAX;
#endif

#if defined(EGFX_PLATFORM_HDR)
	/// <summary>
	/// Native EGFX HDR rgb color.
	/// 8-8-8 24 bit color in a uin32_t wrapper.
	/// </summary>
	typedef uint32_t rgb_color_t;
#else
	/// <summary>
	/// Native EGFX rgb color.
	/// 5-6-5 16 bit color in a uin16_t wrapper.
	/// </summary>
	typedef uint16_t rgb_color_t;
#endif
}

#endif