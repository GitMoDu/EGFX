#ifndef _EGFX_MODULES_BATTERY_INDICATOR_SHADER_h
#define _EGFX_MODULES_BATTERY_INDICATOR_SHADER_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace BatteryIndicator
		{
			namespace Shader
			{
				namespace Color
				{
					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsLast = typename TypeTraits::TypeConditional::conditional_type<
						ColorShaderType,
						Modules::Kit::Shader::Color::Brightness<dimension_t>,
						Monochrome
					>::type;
				}

				namespace Transform
				{
					template<typename dimension_t,
						bool Monochrome,
						typename Base = Framework::Shader::Transform::NoTransform<dimension_t>
					>
					using BatteryBarsLast = typename TypeTraits::TypeConditional::conditional_type<
						Modules::Kit::Shader::Transform::DitherMask<dimension_t>,
						Base,
						Monochrome
					>::type;
				}

				namespace Pixel
				{
					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace
					>
					using BatteryBarsLast = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t>,
						Color::BatteryBarsLast<dimension_t, Monochrome, ColorShaderType>,
						Transform::BatteryBarsLast<dimension_t, Monochrome, TransformShaderType>,
						BlendMode
					>;
				}
			}
		}
	}
}

#endif