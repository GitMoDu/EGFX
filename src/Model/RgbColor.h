// RgbColor.h

#ifndef _RGB_COLOR_h
#define _RGB_COLOR_h

#include "../Platform/Platform.h"

namespace Egfx
{
	using Fraction::ufraction8_t;
	using Fraction::ufraction16_t;
	using Fraction::ufraction32_t;

	/// <summary>
	/// RGB color model (rgb_color_t) operands.
	/// Component (full/native) extraction and combination for 3-3-2/5-6-5/8-8-8 colors.
	/// Color conversion from/to 3-3-2/5-6-5/8-8-8.
	/// Interpolators for 565/888 colors.
	/// Native rgb_color_t abstraction.
	/// </summary>
	struct Rgb
	{
		/// <summary>
		/// Red 5-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>5 bit red component of color.</returns>
		static constexpr uint8_t R5(const uint16_t color)
		{
			return (color >> 11) & 0b11111;
		}

		/// <summary>
		/// Green 6-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>6 bit green component of color.</returns>
		static constexpr uint8_t G6(const uint16_t color)
		{
			return (color >> 5) & 0b111111;
		}

		/// <summary>
		/// Blue 5-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>5 bit blue component of color.</returns>
		static constexpr uint8_t B5(const uint16_t color)
		{
			return color & 0b11111;
		}

		/// <summary>
		/// Red 3-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>3 bit red component of color.</returns>
		static constexpr uint8_t R3(const uint8_t color)
		{
			return (color >> 5) & 0b111;
		}

		/// <summary>
		/// Green 3-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>3 bit green component of color.</returns>
		static constexpr uint8_t G3(const uint8_t color)
		{
			return (color >> 2) & 0b111;
		}

		/// <summary>
		/// Blue 2-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>Blue component of color.</returns>
		static constexpr uint8_t B2(const uint8_t color)
		{
			return color & 0b11;
		}

		/// <summary>
		/// Red 8-bit component of 24-bit color.
		/// </summary>
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>Red component of color.</returns>
		static constexpr uint8_t R(const uint32_t color)
		{
			return (uint8_t)(color >> 16);
		}

		/// <summary>
		/// Green 8-bit component of 24-bit color.
		/// </summary>
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>Green component of color.</returns>
		static constexpr uint8_t G(const uint32_t color)
		{
			return (uint8_t)(color >> 8);
		}

		/// <summary>
		/// Blue 8-bit component of 24-bit color.
		/// </summary>
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>Blue component of color.</returns>
		static constexpr uint8_t B(const uint32_t color)
		{
			return (uint8_t)(color);
		}

		/// <summary>
		/// Red 8-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>Red component of color.</returns>
		static constexpr uint8_t R(const uint16_t color)
		{
			return (R5(color) << 3) | (R5(color) >> 2);
		}

		/// <summary>
		/// Green 8-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>Green component of color.</returns>
		static constexpr uint8_t G(const uint16_t color)
		{
			return (G6(color) << 2) | (G6(color) >> 4);
		}

		/// <summary>
		/// Blue 8-bit component of 16-bit color.
		/// </summary>
		/// <param name="color">5-6-5 source color.</param>
		/// <returns>Blue component of color.</returns>
		static constexpr uint8_t B(const uint16_t color)
		{
			return (B5(color) << 3) | (B5(color) >> 2);
		}

		/// <summary>
		/// Red 8-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>Red component of color.</returns>
		static constexpr uint8_t R(const uint8_t color)
		{
			return (R3(color) << 5)
				| (R3(color) << 2)
				| (R3(color) >> 1);
		}

		/// <summary>
		/// Green 8-bit component of 8 bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>Green component of color.</returns>
		static constexpr uint8_t G(const uint8_t color)
		{
			return (G3(color) << 5)
				| (G3(color) << 2)
				| (G3(color) >> 1);
		}

		/// <summary>
		/// Blue 8-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>Blue component of color.</returns>
		static constexpr uint8_t B(const uint8_t color)
		{
			return (B2(color) << 6)
				| (B2(color) << 4)
				| (B2(color) << 2)
				| B2(color);
		}

		/// <summary>
		/// Convert 8-bit components to 24-bit color value.
		/// </summary>
		/// <param name="r">Red 8-bit component of source color.</param>
		/// <param name="g">Green 8-bit component of source color.</param>
		/// <param name="b">Blue 8-bit component of source color.</param>
		/// <returns>8-8-8 color.</returns>
		static constexpr uint32_t Color888From888(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return ((uint32_t)r << 16)
				| ((uint16_t)g << 8)
				| b;
		}

		/// <summary>
		/// Convert 5/6-bit components to 16-bit color value.
		/// </summary>
		/// <param name="r">Red 5-bit component of source color.</param>
		/// <param name="g">Green 6-bit component of source color.</param>
		/// <param name="b">Blue 5-bit component of source color.</param>
		/// <returns>5-6-5 color.</returns>
		static constexpr uint16_t Color565From565(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return (((uint16_t)r << 11))
				| (((uint16_t)g << 5))
				| (b);
		}

		/// <summary>
		/// Convert 2/3-bit components to 8-bit color value.
		/// </summary>
		/// <param name="r">Red 3-bit component of source color.</param>
		/// <param name="g">Green 3-bit component of source color.</param>
		/// <param name="b">Blue 2-bit component of source color.</param>
		/// <returns>2-3-2 color.</returns>
		static constexpr uint16_t Color332From332(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return (r << 5) | (g << 2) | b;
		}

		/// <summary>
		/// Convert 5-6-5 color to 8-8-8.
		/// </summary>
		/// <param name="color">5-6-5 color.</param>
		/// <returns>8-8-8 color.</returns>
		static constexpr uint32_t Color888(const uint16_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 3-3-2 color to 8-8-8.
		/// </summary>
		/// <param name="color">3-3-2 color.</param>
		/// <returns>8-8-8 color.</returns>
		static constexpr uint32_t Color888(const uint8_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 8 bit grayscale to color 8-8-8.
		/// </summary>
		/// <param name="scale">8 bit brightness value for the grayscale color.</param>
		/// <returns>8-8-8 grayscale color.</returns>
		static constexpr uint32_t Grayscale888(const uint8_t scale)
		{
			return Color888From888(scale, scale, scale);
		}

		/// <summary>
		/// Convert 8-8-8 color to 5-6-5.
		/// </summary>
		/// <param name="color">8-8-8 color.</param>
		/// <returns>5-6-5 color.</returns>	
		static constexpr uint16_t Color565(const uint32_t color)
		{
			return ((uint16_t)(R(color) >> 3) << 11)
				| ((uint16_t)(G(color) >> 2) << 5)
				| ((B(color) >> 3));
		}

		/// <summary>
		/// Convert 3-3-2 color to 5-6-5.
		/// </summary>
		/// <param name="color">3-3-2 color.</param>
		/// <returns>5-6-5 color.</returns>	
		static constexpr uint16_t Color565(const uint8_t color)
		{
			return ((uint16_t)R3(color) << 13 | ((uint16_t)(R3(color) >> 1) << 10))
				| ((uint16_t)G3(color) << 8 | ((uint16_t)G3(color) << 5))
				| (B2(color) << 3 | (B2(color) << 1) | B2(color) >> 1);
		}

		/// <summary>
		/// Abstracts color construction from 3-3-2 to EGFX native rgb_color_t.
		/// </summary>
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static constexpr rgb_color_t Color(const uint8_t color)
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
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static constexpr rgb_color_t Color(const uint16_t color)
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
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static constexpr rgb_color_t Color(const uint32_t color)
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
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static constexpr rgb_color_t Color(const uint8_t r, const uint8_t g, const uint8_t b)
		{
			return Color(Color888From888(r, g, b));
		}

		/// <summary>
		/// Abstracts color construction from 5-6-5 to EGFX native rgb_color_t.
		/// </summary>
		/// <param name="color">8-8-8 source color.</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static constexpr rgb_color_t ColorFrom565(const uint8_t r, const uint8_t g, const uint8_t b)
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
		/// <param name="hue">Hue angle [0 ; ANGLE_RANGE], corresponds to the 360 degrees color wheel.</param>
		/// <param name="saturation">Saturation value [0 ; 255].</param>
		/// <param name="value">Brightness value [0 ; 255].</param>
		/// <returns>EGFX native rgb_color_t.</returns>
		static rgb_color_t ColorFromHSV(const angle_t hue, const uint8_t saturation, const uint8_t value)
		{
			static constexpr uint8_t Segments = 6;

			if (saturation == 0)
			{
				// Achromatic (gray).
				return Rgb::Color(value, value, value);
			}
			else
			{
				// Scale hue to fit into the number of segments.
				const uint16_t hueScaled = ((uint32_t)hue * Segments) >> 8;
				const uint8_t hueSegment = (hueScaled >> 8) % Segments;
				const uint8_t f = hueScaled & UINT8_MAX;

				// Calculate intermediate values.
				const uint8_t p = (((uint16_t)value * (UINT8_MAX - saturation)) / UINT8_MAX);
				const uint8_t fs = (((uint16_t)f * saturation) / UINT8_MAX);
				const uint8_t q = (((uint16_t)value * (UINT8_MAX - fs)) / UINT8_MAX);
				const uint8_t fsR = (((uint16_t)(UINT8_MAX - f) * saturation) / UINT8_MAX);
				const uint8_t t = (((uint16_t)value * (UINT8_MAX - fsR)) / UINT8_MAX);

				// Determine the RGB values based on the hue segment.
				switch (hueSegment % Segments)
				{
				case 0:
					return Rgb::Color(value, t, p);	// Red is dominant.
				case 1:
					return Rgb::Color(q, value, p);	// Green is dominant.
				case 2:
					return Rgb::Color(p, value, t);	// Green is dominant.
				case 3:
					return Rgb::Color(p, q, value);	// Blue is dominant.
				case 4:
					return Rgb::Color(t, p, value);	// Blue is dominant.
				case (Segments - 1):
				default:
					return Rgb::Color(value, p, q);	// Red is dominant.
				}
			}
		}

	private:
		static constexpr uint16_t Power8(const uint8_t value)
		{
			return (uint16_t)value * value;
		}

	public:
		/// <summary>
		/// Interpolate linearly between 2 colors using a ufraction_t. Not as accurate as Interpolate, but much faster.
		/// </summary>
		/// <typeparam name="color_t">Type of the color values.</typeparam>
		/// <param name="fraction">Fraction value for interpolation, typically between 0 and UFRACTION8_1X/UFRACTION16_1X/UFRACTION32_1X.</param>
		/// <param name="from">Starting color value.</param>
		/// <param name="to">Ending color value.</param>
		/// <returns>Interpolated color value.</returns>
		template<typename ufraction_t, typename color_t>
		static constexpr color_t InterpolateLinear(const ufraction_t fraction, const color_t from, const color_t to)
		{
			return Rgb::Color(Fraction::Interpolate(fraction, R(from), R(to)),
				Fraction::Interpolate(fraction, G(from), G(to)),
				Fraction::Interpolate(fraction, B(from), B(to)));
		}


		template<typename color_t>
		static constexpr color_t InterpolateLinear2(const ufraction8_t fraction, const color_t from, const color_t to)
		{
			return Rgb::Color(Fraction::Interpolate(fraction, R(from), R(to)),
				Fraction::Interpolate(fraction, G(from), G(to)),
				Fraction::Interpolate(fraction, B(from), B(to)));
		}

		/// <summary>
		/// Interpolate between 2 colors using a ufraction_t.
		/// </summary>
		/// <typeparam name="color_t">Type of the color values.</typeparam>
		/// <param name="fraction">Fraction value for interpolation, typically between 0 and UFRACTION8_1X/UFRACTION16_1X/UFRACTION32_1X.</param>
		/// <param name="from">Starting color value.</param>
		/// <param name="to">Ending color value.</param>
		/// <returns>Interpolated color value.</returns>
		template<typename ufraction_t, typename color_t>
		static color_t Interpolate(const ufraction_t fraction, const color_t from, const color_t to)
		{
			return Rgb::Color(SquareRoot16(Fraction::Interpolate(fraction, Power8(R(from)), Power8(R(to)))),
				SquareRoot16(Fraction::Interpolate(fraction, Power8(G(from)), Power8(G(to)))),
				SquareRoot16(Fraction::Interpolate(fraction, Power8(B(from)), Power8(B(to)))));
		}
	};

	static constexpr rgb_color_t RGB_COLOR_WHITE = Rgb::Color(uint32_t(0xFFFFFF));
	static constexpr rgb_color_t RGB_COLOR_RED = Rgb::Color(uint32_t(0xFF0000));
	static constexpr rgb_color_t RGB_COLOR_GREEN = Rgb::Color(uint32_t(0x00FF00));
	static constexpr rgb_color_t RGB_COLOR_BLUE = Rgb::Color(uint32_t(0x0000FF));
	static constexpr rgb_color_t RGB_COLOR_BLACK = Rgb::Color(uint32_t(0x000000));
}
#endif