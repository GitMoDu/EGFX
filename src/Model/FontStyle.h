// FontStyle.h

#ifndef _FONT_STYLE_h
#define _FONT_STYLE_h

#include <stdint.h>
#include "RgbColor.h"

class FontStyle
{
public:
	static constexpr uint16_t WIDTH_RATIO_WIDE = 287;
	static constexpr uint8_t WIDTH_RATIO_DEFAULT = 191;
	static constexpr uint8_t WIDTH_RATIO_THIN = 144;

	static constexpr uint8_t KERNING_RATIO_DEFAULT = 32;

private:
	static constexpr uint8_t FONT_HEIGHT_MIN = 5;

public:
	static constexpr uint8_t FONT_SIZE_MIN = FONT_HEIGHT_MIN;
	static constexpr uint8_t FONT_SIZE_REGULAR = FONT_HEIGHT_MIN + 2;
	static constexpr uint8_t FONT_SIZE_MAX = INT8_MAX;

public:
	RgbColor Color;

	uint8_t Height = FONT_HEIGHT_MIN;
	uint8_t Width = GetFontWidth(WIDTH_RATIO_DEFAULT, FONT_HEIGHT_MIN);
	uint8_t Kerning = GetKerning(KERNING_RATIO_DEFAULT, Width);

public:
	FontStyle()
		: Color(INT8_MAX, INT8_MAX, INT8_MAX)
	{}
	const uint8_t GetTextWidth(const uint8_t characterCount) const
	{
		return GetTextWidth(characterCount, Width, Kerning);
	}

	void SetStyle(const RgbColor& color,
		const uint8_t height,
		const uint16_t widthRatio = WIDTH_RATIO_DEFAULT,
		const uint8_t kerningRatio = KERNING_RATIO_DEFAULT)
	{
		SetStyle(color.r, color.g, color.b,
			height, widthRatio, kerningRatio);
	}

	void SetStyle(const uint8_t r,
		const uint8_t g,
		const uint8_t b,
		const uint8_t height,
		const uint16_t widthRatio = KERNING_RATIO_DEFAULT,
		const uint8_t kerningRatio = KERNING_RATIO_DEFAULT)
	{
		Color.r = r;
		Color.g = g;
		Color.b = b;

		SetHeight(height, widthRatio, kerningRatio);
	}

	void SetHeight(const uint8_t height,
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
	static constexpr uint8_t GetFontWidth(const uint16_t widthRatio, const uint8_t height)
	{
		return (((uint16_t)height) * widthRatio) / UINT8_MAX;
	}

	static constexpr uint8_t GetFontHeight(const uint16_t widthRatio, const uint8_t width)
	{
		return (((uint16_t)width) * (UINT8_MAX + widthRatio)) / UINT8_MAX;
	}

	static constexpr uint8_t GetKerning(const uint8_t ratio, const uint8_t value)
	{
		return 1 + (((uint16_t)value) * ratio) / UINT8_MAX;
	}

	static constexpr uint8_t GetTextWidth(const uint8_t characterCount, const uint8_t width, const uint8_t kerning)
	{
		return (characterCount > 0) * ((characterCount * width) + (kerning * (characterCount - 1)));
	}
};

template<const uint16_t widthRatio = FontStyle::WIDTH_RATIO_DEFAULT,
	const uint8_t kerningRatio = FontStyle::KERNING_RATIO_DEFAULT>
class TemplateFontStyle : public FontStyle
{
public:
	void SetSize(const uint8_t size)
	{
		FontStyle::SetHeight(size, widthRatio, kerningRatio);
	}
};
#endif