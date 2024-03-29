// FontStyle.h

#ifndef _FONT_STYLE_h
#define _FONT_STYLE_h

#include <stdint.h>
#include "RgbColor.h"

namespace TinyFont
{
	static constexpr uint16_t FONT_SIZE = 7;
	static constexpr uint16_t WIDTH_RATIO = 223;
};

namespace DefaultFont
{

	static constexpr uint16_t FONT_SIZE = 9;
	static constexpr uint16_t WIDTH_RATIO = 176;
	static constexpr uint16_t KERNING_RATIO = 32;
};

namespace HugeFont
{
	static constexpr uint16_t FONT_SIZE = 28;
	static constexpr uint16_t WIDTH_RATIO = 112;
};

class FontStyle
{
public:
	static constexpr uint16_t MIN_FONT_SIZE = TinyFont::FONT_SIZE;
	static constexpr uint16_t MAX_FONT_SIZE = HugeFont::FONT_SIZE;

	static constexpr uint16_t MIN_FONT_HEIGHT = 7;
	static constexpr uint16_t MIN_FONT_WIDTH = 4;

public:
	RgbColor Color;

	uint8_t Height = 0;
	uint8_t Width = 0;
	uint8_t Kerning = 0;

public:
	void SetStyle(const RgbColor& color,
		const uint8_t height,
		const uint16_t widthRatio = DefaultFont::WIDTH_RATIO,
		const uint8_t kerningRatio = DefaultFont::KERNING_RATIO)
	{
		SetStyle(color.r, color.g, color.b,
			height, widthRatio, kerningRatio);
	}

	void SetStyleByWidth(const RgbColor& color,
		const uint8_t height,
		const uint8_t width,
		const uint8_t kerningRatio = DefaultFont::KERNING_RATIO)
	{
		SetStyleByWidth(color.r, color.g, color.b,
			height, width, kerningRatio);
	}

	void SetStyleByWidth(const uint8_t r,
		const uint8_t g,
		const uint8_t b,
		const uint8_t height,
		const uint8_t width,
		const uint8_t kerningRatio = DefaultFont::KERNING_RATIO)
	{
		if (height < MIN_FONT_HEIGHT)
		{
			Height = MIN_FONT_HEIGHT;
		}
		else if (height > MAX_FONT_SIZE)
		{
			Height = MAX_FONT_SIZE;
		}
		else
		{
			Height = height;
		}

		if (width < MIN_FONT_WIDTH)
		{
			Width = MIN_FONT_WIDTH;
		}
		else if (width > MAX_FONT_SIZE)
		{
			Width = MAX_FONT_SIZE;
		}
		else
		{
			Width = width;
		}

		if (Width > Height)
		{
			Kerning = GetRatioValue(kerningRatio, Height);
		}
		else
		{
			Kerning = GetRatioValue(kerningRatio, Width);
		}

		Color.r = r;
		Color.g = g;
		Color.b = b;
	}

	const uint8_t GetTextWidth(const uint8_t characterCount)
	{
		if (characterCount > 0)
		{
			return (characterCount * Width) + (Kerning * (characterCount - 1));
		}
		else
		{
			return 0;
		}
	}

	void SetStyle(const uint8_t r,
		const uint8_t g,
		const uint8_t b,
		const uint8_t height,
		const uint16_t widthRatio = DefaultFont::WIDTH_RATIO,
		const uint8_t kerningRatio = DefaultFont::KERNING_RATIO)
	{
		if (height < MIN_FONT_HEIGHT)
		{
			Height = MIN_FONT_HEIGHT;
		}
		else if (height > MAX_FONT_SIZE)
		{
			Height = MAX_FONT_SIZE;
		}
		else
		{
			Height = height;
		}

		Width = GetWidth(widthRatio, Height);

		if (Width > Height)
		{
			Kerning = GetRatioValue(kerningRatio, Height);
		}
		else
		{
			Kerning = GetRatioValue(kerningRatio, Width);
		}

		Color.r = r;
		Color.g = g;
		Color.b = b;
	}

public:
	static constexpr uint8_t GetWidth(const uint16_t widthRatio, const uint8_t value)
	{
		return (((uint16_t)value) * widthRatio) / UINT8_MAX;
	}

	static constexpr uint8_t GetHeight(const uint16_t widthRatio, const uint8_t value)
	{
		return (((uint16_t)value) * (UINT8_MAX + widthRatio)) / UINT8_MAX;
	}

private:
	static constexpr uint8_t GetRatioValue(const uint8_t strokeRatio, const uint8_t value)
	{
		return 1 + (((uint16_t)value) * strokeRatio) / UINT8_MAX;
	}
};
#endif