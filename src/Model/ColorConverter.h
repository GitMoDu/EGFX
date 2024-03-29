// ColorConverter.h

#ifndef _COLOR_CONVERTER_h
#define _COLOR_CONVERTER_h

#include <stdint.h>
#include "GraphicsBuffer.h"

struct AbstractColorConverter1
{
	using color_t = uint8_t;

	static constexpr size_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return GraphicsBuffer::GetMonochromeBufferSize(width, height);
	}
};

struct AbstractColorConverter4
{
	using color_t = uint8_t;

	static constexpr size_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return GraphicsBuffer::GetLowColorBufferSize<4>(width, height);
	}
};

struct AbstractColorConverter8
{
	using color_t = uint8_t;

	static constexpr size_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return GraphicsBuffer::GetBufferSize<color_t>(width, height);
	}
};

struct AbstractColorConverter16
{
	using color_t = uint16_t;

	static constexpr size_t GetBufferSize(const uint8_t width, const uint8_t height)
	{
		return GraphicsBuffer::GetBufferSize<color_t>(width, height);
	}
};

/// <summary>
/// Converter for 8 bit color, 3-3-2 color format.
/// </summary>
struct ColorConverter8 : public AbstractColorConverter8
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return ((color.r & 0xE0) | ((color.g >> 3) & 0x1C) | (color.b >> 6));
	}
};

/// <summary>
/// Converter for 16 bit color, 5-6-5 color format.
/// </summary>
struct ColorConverter16 : public AbstractColorConverter16
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return ((((uint16_t)color.r << 8) & 0xF800) | (((uint16_t)color.g << 3) & 0x07E0) | ((uint16_t)color.b >> 3));
	}
};

/// <summary>
/// Converter for 8 bit grayscale.
/// </summary>
struct GrayScaleConverter8 : public AbstractColorConverter8
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return (((uint16_t)color.r * 77) | ((uint16_t)color.g * 150) | ((uint16_t)color.b * 29)) >> 8;
	}
};

/// <summary>
/// Converter for 4 bit grayscale.
/// </summary>
struct GrayScaleConverter4 : public AbstractColorConverter4
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return ((color.r >> 2) + (color.g >> 1) + (color.b >> 2));
	}
};

/// <summary>
/// Converter for 1 bit color, with configurable threshold.
/// Uses GrayScaleConverter8 for conversion before comparison.
/// </summary>
/// <typeparam name="threshold">Grayscale threshold for color.</typeparam>
template<const uint8_t threshold = 0>
struct MonochromeColorConverter1 : public AbstractColorConverter1
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return GrayScaleConverter8::GetRawColor(color) > threshold;
	}
};

/// <summary>
/// Converter for 1 bit color, with fixed threshold at 0.
/// </summary>
struct BinaryColorConverter1 : public AbstractColorConverter1
{
	static constexpr color_t GetRawColor(const RgbColor& color)
	{
		return color.r > 0 || color.g > 0 || color.b > 0;
	}
};
#endif