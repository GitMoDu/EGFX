#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_COLOR_SOURCE_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_COLOR_SOURCE_h

#include "../../../Image/Model.h"
#include "../../../Vector/Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Vector
				{
					/// <summary>
					/// Wrapper for PixelShader color source that provides a dynamic color for given section of the vector image.
					/// </summary>
					/// <typeparam name="dimension_t"></typeparam>
					template<typename dimension_t>
					struct ColorSource
					{
						// Color remains constant during individual draw operations.
						static constexpr bool IsConstantColor = true;

						rgb_color_t Color = RGB_COLOR_WHITE;

						/// <summary>
						/// Wrapper for Color Source, returns the currently set color.
						/// </summary>
						/// <param name="x">The x coordinate (ignored).</param>
						/// <param name="y">The y coordinate (ignored).</param>
						/// <returns>The currently set color.</returns>
						rgb_color_t Source(const dimension_t /*x*/, const dimension_t /*y*/) const
						{
							return Color;
						}
					};

					template<rgb_color_t StaticColor>
					struct StaticColorPalette
					{
						static constexpr rgb_color_t GetColor(const uint8_t /*colorIndex*/)
						{
							return StaticColor;
						}
					};

					template<uint8_t PaletteSize>
					struct ColorPalette
					{
						rgb_color_t Palette[PaletteSize]{};

						rgb_color_t GetColor(const uint8_t colorIndex) const
						{
							return Palette[colorIndex % PaletteSize];
						}

						void SetColor(const uint8_t colorIndex, const rgb_color_t color)
						{
							Palette[colorIndex % PaletteSize] = color;
						}
					};
				}
			}
		}
	}
}
#endif