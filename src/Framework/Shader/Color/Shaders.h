#ifndef _EGFX_FRAMEWORK_SHADER_COLOR_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_COLOR_SHADERS_h

#include "Model.h"

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
				using NoShader = BaseShader<dimension_t>;
			}
		}
	}
}
#endif