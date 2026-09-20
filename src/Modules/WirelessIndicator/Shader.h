#ifndef _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_SHADER_h
#define _INTEGERGLASS_MODULES_WIRELESS_INDICATOR_SHADER_h

#include "Constant.h"
#include "Assets.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Shader
			{
				namespace Source
				{
					template<typename dimension_t>
					using Bar = Framework::Shader::Source::SingleColor<dimension_t, RGB_COLOR_WHITE>;

					template<typename dimension_t, bool Monochrome>
					using DisabledOverlay = typename TypeTraits::TypeConditional::conditional_type <
						Framework::Shader::Source::StaticColor<dimension_t, RGB_COLOR_WHITE>,
						Framework::Shader::Source::SingleColor<dimension_t, RGB_COLOR_WHITE>,
						Monochrome
					>::type;
				}

				namespace Color
				{
					template<typename dimension_t, bool Monochrome>
					using Bar = typename TypeTraits::TypeConditional::conditional_type<
						Framework::Shader::Color::NoShader<dimension_t>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Monochrome>::type;
				}

				namespace Transform
				{

					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool Monochrome,
						typename BaseTransformType = Framework::Shader::Transform::NoTransform<dimension_t>>
					using Bar = typename TypeTraits::TypeConditional::conditional_type<
						Modules::Kit::Shader::Transform::CheckerboardMask<
						dimension_t,
						false,
						Modules::Kit::Shader::Transform::ScaleDownX<dimension_t, Width, Height, BaseTransformType>
						>,
						Modules::Kit::Shader::Transform::ScaleDownX<dimension_t, Width, Height, BaseTransformType>,
						Monochrome
					>::type;
				}

				namespace Pixel
				{
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
						Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace>
					using Bar = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::Bar<dimension_t>,
						ColorShaderType,
						Transform::Bar<dimension_t, Width, Height, Monochrome, TransformShaderType>,
						BlendMode
					>;
				}

			}
		}
	}
}
#endif