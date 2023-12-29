// FontStyle.h

#ifndef _FONT_STYLE_h
#define _FONT_STYLE_h

#include <stdint.h>

class FontStyle
{
public:
	static constexpr uint16_t DefaultWidthRatio = 192;
	static constexpr uint8_t DefaultStrokeRatio = 24;
	static constexpr uint8_t DefaultKerningRatio = 76;

public:
	RgbColor Color;

	uint8_t Height = 0;
	uint8_t Width = 0;
	uint8_t Stroke = 0;
	uint8_t Kerning = 0;

public:
	void SetStyle(const RgbColor& color,
		const uint8_t height,
		const uint16_t widthRatio = DefaultWidthRatio,
		const uint8_t strokeRatio = DefaultStrokeRatio,
		const uint8_t kerningRatio = DefaultKerningRatio)
	{
		SetStyle(color.r, color.g, color.b,
			height, widthRatio, strokeRatio, kerningRatio);
	}

	void SetStyle(const uint8_t r,
		const uint8_t g,
		const uint8_t b,
		const uint8_t height,
		const uint16_t widthRatio = DefaultWidthRatio,
		const uint8_t strokeRatio = DefaultStrokeRatio,
		const uint8_t kerningRatio = DefaultKerningRatio)
	{
		if (Height != height)
		{
			Height = height;
			Width = GetWidth(widthRatio, height);

			if (Width > height)
			{
				Stroke = GetRatioValue(strokeRatio, height);
				Kerning = GetRatioValue(kerningRatio, height);
			}
			else
			{
				Stroke = GetRatioValue(strokeRatio, Width);
				Kerning = GetRatioValue(kerningRatio, Width);
			}
		}
		Color.r = r;
		Color.g = g;
		Color.b = b;
	}

private:
	static constexpr uint8_t GetWidth(const uint16_t widthRatio, const uint8_t value)
	{
		return (((uint16_t)value) * widthRatio) / UINT8_MAX;
	}

	static constexpr uint8_t GetRatioValue(const uint8_t strokeRatio, const uint8_t value)
	{
		return 1 + (((uint16_t)value) * strokeRatio) / UINT8_MAX;
	}
};
#endif