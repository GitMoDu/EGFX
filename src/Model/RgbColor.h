#ifndef _EGFX_RGB_COLOR_h
#define _EGFX_RGB_COLOR_h

#include "../Platform/Platform.h"

namespace Egfx
{
	using namespace IntegerSignal::FixedPoint::ScalarFraction;

	/// <summary>
	/// RGB color model (rgb_color_t) operands.
	/// Component (full/native) extraction and combination for 3-3-2/5-6-5/8-8-8 colors.
	/// Color conversion from/to 3-3-2/5-6-5/8-8-8.
	/// Interpolators for 565/888 colors.
	/// Native rgb_color_t abstraction.
	/// </summary>
	namespace Rgb
	{
		/// <summary>
		/// Red 8-bit component of 32-bit color (ARGB8888 / 0xAARRGGBB).
		/// </summary>
		inline constexpr uint8_t R8(const uint32_t color)
		{
			return static_cast<uint8_t>(color >> 16);
		}

		/// <summary>
		/// Green 8-bit component of 32-bit color (ARGB8888 / 0xAARRGGBB).
		/// </summary>
		inline constexpr uint8_t G8(const uint32_t color)
		{
			return static_cast<uint8_t>(color >> 8);
		}

		/// <summary>
		/// Blue 8-bit component of 32-bit color (ARGB8888 / 0xAARRGGBB).
		/// </summary>
		inline constexpr uint8_t B8(const uint32_t color)
		{
			return static_cast<uint8_t>(color);
		}

		/// <summary>
		/// Red 5-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t R5(const uint16_t color)
		{
			return (color >> 11) & 0b11111;
		}

		/// <summary>
		/// Green 6-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t G6(const uint16_t color)
		{
			return (color >> 5) & 0b111111;
		}

		/// <summary>
		/// Blue 5-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t B5(const uint16_t color)
		{
			return color & 0b11111;
		}

		/// <summary>
		/// Red 3-bit component of 8-bit color.
		/// </summary>
		inline constexpr uint8_t R3(const uint8_t color)
		{
			return (color >> 5) & 0b111;
		}

		/// <summary>
		/// Green 3-bit component of 8-bit color.
		/// </summary>
		inline constexpr uint8_t G3(const uint8_t color)
		{
			return (color >> 2) & 0b111;
		}

		/// <summary>
		/// Blue 2-bit component of 8-bit color.
		/// </summary>
		inline constexpr uint8_t B2(const uint8_t color)
		{
			return color & 0b11;
		}

		/// <summary>
		/// Red 8-bit component of 24-bit color.
		/// </summary>
		inline constexpr uint8_t R(const uint32_t color)
		{
			return R8(color);
		}

		/// <summary>
		/// Green 8-bit component of 24-bit color.
		/// </summary>
		inline constexpr uint8_t G(const uint32_t color)
		{
			return G8(color);
		}

		/// <summary>
		/// Blue 8-bit component of 24-bit color.
		/// </summary>
		inline constexpr uint8_t B(const uint32_t color)
		{
			return B8(color);
		}

		/// <summary>
		/// Red 8-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t R(const uint16_t color)
		{
			return (R5(color) << 3) | (R5(color) >> 2);
		}

		/// <summary>
		/// Green 8-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t G(const uint16_t color)
		{
			return (G6(color) << 2) | (G6(color) >> 4);
		}

		/// <summary>
		/// Blue 8-bit component of 16-bit color.
		/// </summary>
		inline constexpr uint8_t B(const uint16_t color)
		{
			return (B5(color) << 3) | (B5(color) >> 2);
		}

		/// <summary>
		/// Red 8-bit component of 8-bit color.
		/// </summary>
		inline constexpr uint8_t R(const uint8_t color)
		{
			return (R3(color) << 5)
				| (R3(color) << 2)
				| (R3(color) >> 1);
		}

		/// <summary>
		/// Green 8-bit component of 8 bit color.
		/// </summary>
		inline constexpr uint8_t G(const uint8_t color)
		{
			return (G3(color) << 5)
				| (G3(color) << 2)
				| (G3(color) >> 1);
		}

		/// <summary>
		/// Blue 8-bit component of 8-bit color.
		/// </summary>
		inline constexpr uint8_t B(const uint8_t color)
		{
			return (B2(color) << 6)
				| (B2(color) << 4)
				| (B2(color) << 2)
				| B2(color);
		}

		/// <summary>
		/// Convert 8-bit components to 24-bit color value.
		/// </summary>
		inline constexpr uint32_t Color888From888(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return ((uint32_t)r << 16)
				| ((uint16_t)g << 8)
				| b;
		}

		/// <summary>
		/// Convert 5/6-bit components to 16-bit color value.
		/// </summary>
		inline constexpr uint16_t Color565From565(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return (((uint16_t)r << 11))
				| (((uint16_t)g << 5))
				| (b);
		}

		/// <summary>
		/// Convert 2/3-bit components to 8-bit color value.
		/// </summary>
		inline constexpr uint16_t Color332From332(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return (r << 5) | (g << 2) | b;
		}

		/// <summary>
		/// Convert 8-8-8 color to 3-3-2.
		/// </summary>
		inline constexpr uint8_t Color332From888(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return static_cast<uint8_t>(((r >> 5) << 5) | ((g >> 5) << 2) | (b >> 6));
		}

		/// <summary>
		/// Convert 8-8-8 packed color to 3-3-2.
		/// </summary>
		inline constexpr uint8_t Color332From888(const uint32_t color)
		{
			return Color332From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 8-8-8 color to 5-6-5.
		/// </summary>
		inline constexpr uint16_t Color565From888(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return static_cast<uint16_t>(((uint16_t)(r >> 3) << 11)
				| ((uint16_t)(g >> 2) << 5)
				| ((uint16_t)(b >> 3)));
		}

		/// <summary>
		/// Convert 8-8-8 packed color to 5-6-5.
		/// </summary>
		inline constexpr uint16_t Color565From888(const uint32_t color)
		{
			return Color565From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 5-6-5 color to 8-8-8.
		/// </summary>
		inline constexpr uint32_t Color888(const uint16_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 3-3-2 color to 8-8-8.
		/// </summary>
		inline constexpr uint32_t Color888(const uint8_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 8 bit grayscale to color 8-8-8.
		/// </summary>
		inline constexpr uint32_t Grayscale888(const uint8_t scale)
		{
			return Color888From888(scale, scale, scale);
		}

		/// <summary>
		/// Convert 8-8-8 color to 5-6-5.
		/// </summary>
		inline constexpr uint16_t Color565(const uint32_t color)
		{
			return ((uint16_t)(R(color) >> 3) << 11)
				| ((uint16_t)(G(color) >> 2) << 5)
				| ((B(color) >> 3));
		}

		/// <summary>
		/// Convert 3-3-2 color to 5-6-5.
		/// </summary>
		inline constexpr uint16_t Color565(const uint8_t color)
		{
			return ((uint16_t)R3(color) << 13 | ((uint16_t)(R3(color) >> 1) << 10))
				| ((uint16_t)G3(color) << 8 | ((uint16_t)G3(color) << 5))
				| (B2(color) << 3 | (B2(color) << 1) | B2(color) >> 1);
		}

		/// <summary>
		/// Abstracts color construction from 3-3-2 to EGFX native rgb_color_t.
		/// </summary>
		inline constexpr rgb_color_t Color(const uint8_t color)
		{
#if defined(EGFX_PLATFORM_HDR)
			return Color888(color);
#else
			return Color565(color);
#endif
		}

		/// <summary>
		/// Abstracts color construction from 5-6-5 to EGFX native rgb_color_t.
		/// </summary>
		inline constexpr rgb_color_t Color(const uint16_t color)
		{
#if defined(EGFX_PLATFORM_HDR)
			return Color888(color);
#else
			return color;
#endif
		}

		/// <summary>
		/// Abstracts color construction from 8-8-8 to EGFX native rgb_color_t.
		/// </summary>
		inline constexpr rgb_color_t Color(const uint32_t color)
		{
#if defined(EGFX_PLATFORM_HDR)
			return color;
#else
			return Color565(color);
#endif
		}

		/// <summary>
		/// Abstracts color construction from 8-8-8 components to EGFX native rgb_color_t.
		/// </summary>
		inline constexpr rgb_color_t Color(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return Color(Color888From888(r, g, b));
		}

		/// <summary>
		/// Abstracts color construction from 5-6-5 to EGFX native rgb_color_t.
		/// </summary>
		inline constexpr rgb_color_t ColorFrom565(const uint8_t r, const uint8_t g, const uint8_t b)
		{
#if defined(EGFX_PLATFORM_HDR)
			return Color888(Color565From565(r, g, b));
#else
			return Color565From565(r, g, b);
#endif
		}

		/// <summary>
		/// Converts HSV to EGFX native rgb_color_t.
		/// </summary>
		inline rgb_color_t ColorFromHSV(const angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			static constexpr uint8_t Segments = 6;

			if (saturation == 0)
			{
				return Rgb::Color(value, value, value);
			}
			else
			{
				const uint16_t hueScaled = ((uint32_t)hue * Segments) >> 8;
				const uint8_t hueSegment = (hueScaled >> 8) % Segments;
				const uint8_t f = hueScaled & UINT8_MAX;

				const uint8_t p = (((uint16_t)value * (UINT8_MAX - saturation)) / UINT8_MAX);
				const uint8_t fs = (((uint16_t)f * saturation) / UINT8_MAX);
				const uint8_t q = (((uint16_t)value * (UINT8_MAX - fs)) / UINT8_MAX);
				const uint8_t fsR = (((uint16_t)(UINT8_MAX - f) * saturation) / UINT8_MAX);
				const uint8_t t = (((uint16_t)value * (UINT8_MAX - fsR)) / UINT8_MAX);

				switch (hueSegment % Segments)
				{
				case 0:
					return Rgb::Color(value, t, p);
				case 1:
					return Rgb::Color(q, value, p);
				case 2:
					return Rgb::Color(p, value, t);
				case 3:
					return Rgb::Color(p, q, value);
				case 4:
					return Rgb::Color(t, p, value);
				case (Segments - 1):
				default:
					return Rgb::Color(value, p, q);
				}
			}
		}

		inline constexpr uint16_t Power8(const uint8_t value)
		{
			return (static_cast<uint16_t>(value) * value);
		}

		template<typename ufraction_t, typename color_t>
		inline constexpr color_t InterpolateLinear(const ufraction_t fraction, const color_t from, const color_t to)
		{
			return Rgb::Color(static_cast<uint8_t>(UFraction16::Interpolate(fraction, R(from), R(to))),
				static_cast<uint8_t>(UFraction16::Interpolate(fraction, G(from), G(to))),
				static_cast<uint8_t>(UFraction16::Interpolate(fraction, B(from), B(to))));
		}

		template<typename ufraction_t, typename color_t>
		inline color_t Interpolate(const ufraction_t fraction, const color_t from, const color_t to)
		{
			return Rgb::Color(SquareRoot16(UFraction16::Interpolate(fraction, Power8(R(from)), Power8(R(to)))),
				SquareRoot16(UFraction16::Interpolate(fraction, Power8(G(from)), Power8(G(to)))),
				SquareRoot16(UFraction16::Interpolate(fraction, Power8(B(from)), Power8(B(to)))));
		}
	};

	static constexpr rgb_color_t RGB_COLOR_WHITE = Rgb::Color(uint32_t(0xFFFFFF));
	static constexpr rgb_color_t RGB_COLOR_RED = Rgb::Color(uint32_t(0xFF0000));
	static constexpr rgb_color_t RGB_COLOR_GREEN = Rgb::Color(uint32_t(0x00FF00));
	static constexpr rgb_color_t RGB_COLOR_BLUE = Rgb::Color(uint32_t(0x0000FF));
	static constexpr rgb_color_t RGB_COLOR_BLACK = Rgb::Color(uint32_t(0x000000));
}
#endif