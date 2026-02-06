#ifndef _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_INVERT_h
#define _EGFX_FRAMEWORK_ASSETS_SHADER_COLOR_INVERT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Shader
			{
				namespace Color
				{
					using namespace Framework::Shader::Color;

					/// <summary>
					/// Inverts the input color.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Base">The base shader type.</typeparam>
					template<typename dimension_t,
						typename Base = BaseShader<dimension_t>
					>
					struct Invert : BaseShader<dimension_t>
					{
						rgb_color_t Shade(const rgb_color_t color)
						{
							return ~BaseShader<dimension_t>::Shade(color);
						}
					};
				}
			}
		}
	}
}
#endif