#ifndef _EGFX_FRAMEWORK_SHADER_COLOR_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_COLOR_MODEL_h

#include "../../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Color
			{
				/// <summary>
				/// No-op color shader that returns the input color unchanged.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct BaseShader
				{
					static constexpr rgb_color_t Shade(const rgb_color_t color)
					{
						return color;
					}
				};

				namespace Contract
				{
					/// <summary>
					/// Defines the contract for color shading, uses only coordinate inputs to determine the color output.
					/// This class serves as documentation only, and is not intended to be used directly.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="BaseShader">The base shader to inherit from.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					struct ColorShader : Base
					{
						// Shade a pixel color at the given local-space coordinates.
						rgb_color_t Shade(const rgb_color_t color)
						{
							// Color shading implementation.
							return 0;
						}
					};
				}
			}
		}
	}
}
#endif