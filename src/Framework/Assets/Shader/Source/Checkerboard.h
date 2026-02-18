#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_CHECKERBOARD_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_CHECKERBOARD_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Source
				{
					/// <summary>
					/// Determines the color for a given pixel position using a checkerboard pattern with configurable colors and phase offset.
					/// Original color input is ignored.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t>
					class Checkerboard
					{
					public:
						rgb_color_t Color1 = RGB_COLOR_WHITE;
						rgb_color_t Color2 = RGB_COLOR_BLACK;

					private:
						uint8_t Offset = 0;

					protected:
						pixel_point_t Origin{ 0, 0 };

					public:
						Checkerboard() = default;
						~Checkerboard() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							const pixel_t globalX = Origin.x + x;
							const pixel_t globalY = Origin.y + y;

							return (((globalX + globalY + Offset) % 2) == 0) ? Color1 : Color2;
						}

						/// <summary>
						/// Pattern phase/shift.
						/// </summary>
						void SetOffset(const uint8_t offset)
						{
							Offset = offset;
						}

						uint8_t GetOffset() const
						{
							return Offset;
						}
					};
				}
			}
		}
	}
}
#endif