// RgbColor.h

#ifndef _RGB_COLOR_h
#define _RGB_COLOR_h

#include "../Platform/Platform.h"

namespace Egfx
{
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
		static constexpr uint8_t G3(const uint16_t color)
		{
			return (color >> 2) & 0b111;
		}

		/// <summary>
		/// Blue 2-bit component of 8-bit color.
		/// </summary>
		/// <param name="color">3-3-2 source color.</param>
		/// <returns>Blue component of color.</returns>
		static constexpr uint8_t B2(const uint16_t color)
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
			return (B5(color) << 3)
				| (B5(color) >> 2);
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
		/// <param name="color"></param>
		/// <returns></returns>
		static constexpr uint32_t Color888(const uint16_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 3-3-2 color to 8-8-8.
		/// </summary>
		/// <param name="color"></param>
		/// <returns></returns>	
		static constexpr uint32_t Color888(const uint8_t color)
		{
			return Color888From888(R(color), G(color), B(color));
		}

		/// <summary>
		/// Convert 8 bit grayscale to color 8-8-8.
		/// </summary>
		/// <param name="color"></param>
		/// <returns></returns>
		static constexpr uint32_t Grayscale888(const uint8_t scale)
		{
			return Color888From888(scale, scale, scale);
		}

		/// <summary>
		/// Convert 8-8-8 color to 5-6-5.
		/// </summary>
		/// <param name="color"></param>
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
		/// <param name="color"></param>
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


	private:
		/// <summary>
		/// Fast square root for uint16_t.
		/// </summary>
		/// <param name="x"></param>
		/// <returns></returns>
		static const uint8_t SqrtU16(const uint16_t x)
		{
			uint8_t res = 0;
			uint8_t add = 0x80;

			for (uint_fast8_t i = 0; i < 8; i++)
			{
				const uint8_t temp = res | add;
				const uint16_t g2 = (uint16_t)temp * temp;
				if (x >= g2)
				{
					res = temp;
				}
				add >>= 1;
			}

			return res;
		}

		static const uint8_t InterpolateProgressSquared(const uint8_t a, const uint8_t b, const uint8_t progress)
		{
			const uint16_t aa = ((uint32_t)(UINT8_MAX - progress) * (((uint16_t)a * a))) >> 8;
			const uint16_t bb = ((uint32_t)progress * ((uint16_t)b * b)) >> 8;

			return SqrtU16(aa + bb);
		}

		static constexpr uint8_t InterpolateProgressLinear(const uint8_t a, const uint8_t b, const uint8_t progress)
		{
			return (((uint16_t)b * progress) + ((uint16_t)a * (UINT8_MAX - progress))) >> 8;
		}

	public:
		/// <summary>
		/// Interpolate between 2 colors in RGB space.
		/// </summary>
		/// <param name="color1">5-6-5 source color.</param>
		/// <param name="color2">5-6-5 source color.</param>
		/// <param name="progress">Interpolation progress [0;UINT8_MAX].</param>
		/// <returns>5-6-5 color.</returns>
		static const uint16_t Interpolate(const uint16_t color1, const uint16_t color2, const uint8_t progress)
		{
			return Color565(Interpolate(Color888(color1), Color888(color2), progress));
		}

		/// <summary>
		/// Interpolate between 2 colors linearly.
		/// Not as accurate as Interpolate, but much faster.
		/// </summary>
		/// <param name="color1">5-6-5 source color.</param>
		/// <param name="color2">5-6-5 source color.</param>
		/// <param name="progress">Interpolation progress [0;UINT8_MAX].</param>
		/// <returns>5-6-5 color.</returns>
		static constexpr uint16_t InterpolateLinear(const uint16_t color1, const uint16_t color2, const uint8_t progress)
		{
			return Color565From565(InterpolateProgressLinear(R5(color1), R5(color2), progress)
				, InterpolateProgressLinear(G6(color1), G6(color2), progress)
				, InterpolateProgressLinear(B5(color1), B5(color2), progress));
		}

		/// <summary>
		/// Interpolate between 2 colors in RGB space.
		/// </summary>
		/// <param name="color1">8-8-8 source color.</param>
		/// <param name="color2">8-8-8 source color.</param>
		/// <param name="progress">Interpolation progress [0;UINT8_MAX].</param>
		/// <returns>8-8-8 color.</returns>
		static const uint32_t Interpolate(const uint32_t color1, const uint32_t color2, const uint8_t progress)
		{
			return Color888From888(InterpolateProgressSquared(R(color1), R(color2), progress)
				, InterpolateProgressSquared(G(color1), G(color2), progress)
				, InterpolateProgressSquared(B(color1), B(color2), progress));
		}

		/// <summary>
		/// Interpolate between 2 colors linearly.
		/// Not as accurate as Interpolate, but much faster.
		/// </summary>
		/// <param name="color1">8-8-8 source color.</param>
		/// <param name="color2">8-8-8 source color.</param>
		/// <param name="progress">Interpolation progress [0;UINT8_MAX].</param>
		/// <returns>8-8-8 color.</returns>
		static constexpr uint32_t InterpolateLinear(const uint32_t color1, const uint32_t color2, const uint8_t progress)
		{
			return Color888From888(InterpolateProgressLinear(R(color1), R(color2), progress)
				, InterpolateProgressLinear(G(color1), G(color2), progress)
				, InterpolateProgressLinear(B(color1), B(color2), progress));
		}
	};
}
#endif