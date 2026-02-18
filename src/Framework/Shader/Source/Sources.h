#ifndef _EGFX_FRAMEWORK_SHADER_SOURCE_SOURCES_h
#define _EGFX_FRAMEWORK_SHADER_SOURCE_SOURCES_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Source
			{
				/// <summary>
				/// Stateless color source that always returns a static color.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="StaticColor">The static color to return for all positions. Defaults to white.</typeparam>
				template<typename dimension_t,
					rgb_color_t Color = RGB_COLOR_WHITE>
				struct StaticColor
				{
					static constexpr bool IsConstantColor = true;

					static constexpr rgb_color_t Source(const dimension_t /*x*/, const dimension_t /*y*/)
					{
						return Color;
					}
				};

				/// <summary>
				/// Represents a single solid color pattern that returns the same color for any coordinate position.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="DefaultColor">The default RGB color value. Defaults to RGB_COLOR_WHITE.</typeparam>
				template<typename dimension_t, rgb_color_t DefaultColor = RGB_COLOR_WHITE>
				struct SingleColor
				{
					static constexpr bool IsConstantColor = true;

					rgb_color_t Color = DefaultColor;

					rgb_color_t Source(const dimension_t /*x*/, const dimension_t /*y*/) const
					{
						return Color;
					}
				};

				/// <summary>
				/// Compile-time extractor for ColorSourceType::IsConstantColor.
				/// Defaults to false when the member does not exist.
				/// </summary>
				template<typename T, typename = void>
				struct ConstantColorFlag
				{
					static constexpr bool value = false;
				};

				template<typename T>
				struct ConstantColorFlag<T,
					typename IntegerSignal::TypeTraits::TypeEnableIf::enable_if<(T::IsConstantColor), void>::type>
				{
					static constexpr bool value = true;
				};
			}
		}
	}
}
#endif