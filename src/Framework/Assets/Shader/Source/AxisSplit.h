#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_AXIS_SPLIT_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_SOURCE_AXIS_SPLIT_h

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
					/// Color source with 2 colors that splits the color into two halves along the X axis.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t>
					class AxisSplitX
					{
					private:
						static constexpr dimension_t MaxSplit = TypeTraits::TypeLimits::type_limits<dimension_t>::Max();

					public:
						rgb_color_t Color1 = RGB_COLOR_WHITE;
						rgb_color_t Color2 = RGB_COLOR_BLACK;

					private:
						dimension_t Split = MaxSplit;

					public:
						AxisSplitX() = default;
						~AxisSplitX() = default;

						void SetSplitX(const dimension_t split)
						{
							Split = split;
						}

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							if (x < Split)
							{
								return Color1;
							}
							else
							{
								return Color2;
							}
						}
					};

					/// <summary>
					/// Color source with 2 colors that splits the color into two halves along the Y axis.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					template<typename dimension_t>
					class AxisSplitY
					{
					private:
						static constexpr dimension_t MaxSplit = TypeTraits::TypeLimits::type_limits<dimension_t>::Max();

					public:
						rgb_color_t Color1 = RGB_COLOR_WHITE;
						rgb_color_t Color2 = RGB_COLOR_BLACK;

					private:
						dimension_t Split = MaxSplit;

					public:
						AxisSplitY() = default;
						~AxisSplitY() = default;

						void SetSplitY(const dimension_t split)
						{
							Split = split;
						}

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							if (y < Split)
							{
								return Color1;
							}
							else
							{
								return Color2;
							}
						}
					};
				}
			}
		}
	}
}
#endif