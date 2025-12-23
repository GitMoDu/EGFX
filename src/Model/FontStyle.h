// FontStyle.h

#ifndef _EGFX_FONT_STYLE_h
#define _EGFX_FONT_STYLE_h

#include <stdint.h>
#include "RgbColor.h"

namespace Egfx
{
	class FontStyle
	{
	public:
		static constexpr uint16_t WIDTH_RATIO_WIDE = 287;
		static constexpr uint8_t WIDTH_RATIO_DEFAULT = 191;
		static constexpr uint8_t WIDTH_RATIO_THIN = 144;

		static constexpr uint8_t KERNING_RATIO_DEFAULT = 64;

	private:
		static constexpr uint8_t FONT_HEIGHT_MIN = 5;

	public:
		static constexpr uint8_t FONT_SIZE_MIN = FONT_HEIGHT_MIN;
		static constexpr uint8_t FONT_SIZE_REGULAR = FONT_HEIGHT_MIN + 2;
		static constexpr pixel_t FONT_SIZE_MAX = Egfx::MAX_PIXEL_SIZE / 2;

	public:
		rgb_color_t Color;

		pixel_t Height = FONT_HEIGHT_MIN;
		pixel_t Width = GetFontWidth(WIDTH_RATIO_DEFAULT, FONT_HEIGHT_MIN);
		uint8_t Kerning = GetKerning(KERNING_RATIO_DEFAULT, Width);

	public:
		FontStyle()
			: Color(Rgb::Color(INT8_MAX, INT8_MAX, INT8_MAX))
		{
		}
		pixel_t GetTextWidth(const uint8_t characterCount) const
		{
			return GetTextWidth(characterCount, Width, Kerning);
		}

		void SetStyle(const uint8_t r,
			const uint8_t g,
			const uint8_t b,
			const pixel_t height,
			const uint16_t widthRatio = WIDTH_RATIO_DEFAULT,
			const uint8_t kerningRatio = KERNING_RATIO_DEFAULT)
		{
			SetStyle(Rgb::Color(r, g, b), height, widthRatio, kerningRatio);
		}

		void SetStyle(const rgb_color_t color,
			const pixel_t height,
			const uint16_t widthRatio = WIDTH_RATIO_DEFAULT,
			const uint8_t kerningRatio = KERNING_RATIO_DEFAULT)
		{
			Color = color;

			SetHeight(height, widthRatio, kerningRatio);
		}

		void SetHeight(const pixel_t height,
			const uint16_t widthRatio = FontStyle::WIDTH_RATIO_DEFAULT,
			const uint8_t kerningRatio = FontStyle::KERNING_RATIO_DEFAULT)
		{
			if (height <= FontStyle::FONT_SIZE_MIN)
			{
				Height = FontStyle::FONT_SIZE_MIN;
			}
			else if (height >= FontStyle::FONT_SIZE_MAX)
			{
				Height = FontStyle::FONT_SIZE_MAX;
			}
			else
			{
				Height = height;
			}

			Width = GetFontWidth(widthRatio, Height);

			if (Width > Height)
			{
				Kerning = GetKerning(kerningRatio, Height);
			}
			else
			{
				Kerning = GetKerning(kerningRatio, Width);
			}
		}

	public:
		static constexpr pixel_t GetFontWidth(const uint16_t widthRatio, const pixel_t height)
		{
			return (((pixel_index_t)height) * widthRatio) / UINT8_MAX;
		}

		static constexpr pixel_t GetFontHeight(const uint16_t widthRatio, const pixel_t width)
		{
			return (((pixel_index_t)width) * (UINT8_MAX + widthRatio)) / UINT8_MAX;
		}

		static constexpr uint8_t GetKerning(const uint8_t ratio, const pixel_t value)
		{
			return 1 + (((pixel_index_t)value) * ratio) / UINT8_MAX;
		}

		static constexpr pixel_t GetTextWidth(const uint8_t characterCount, const pixel_t width, const uint8_t kerning)
		{
			return (characterCount > 0) * ((characterCount * width) + (kerning * (characterCount - 1)));
		}
	};

	template<uint16_t widthRatio = FontStyle::WIDTH_RATIO_DEFAULT,
		uint8_t kerningRatio = FontStyle::KERNING_RATIO_DEFAULT>
	class TemplateFontStyle : public FontStyle
	{
	public:
		void SetSize(const pixel_t size)
		{
			FontStyle::SetHeight(size, widthRatio, kerningRatio);
		}
	};
}
#endif