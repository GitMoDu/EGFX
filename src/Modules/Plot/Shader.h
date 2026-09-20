#ifndef _INTEGERGLASS_MODULES_PLOT_SHADER_h
#define _INTEGERGLASS_MODULES_PLOT_SHADER_h

#include "Constant.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Shader
			{
				namespace Pixel
				{
					template<typename dimension_t,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					using Line = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t>, ColorShaderType, TransformShaderType, BlendMode>;
				}
			}
		}
	}
}
#endif