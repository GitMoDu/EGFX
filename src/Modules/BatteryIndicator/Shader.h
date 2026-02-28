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
				namespace Source
				{
					template<typename dimension_t>
					using BatteryOutsideMonochrome = Framework::Shader::Source::StaticColor<dimension_t>;

					template<typename dimension_t>
					using BatteryOutsideColor = Framework::Shader::Source::SingleColor<dimension_t>;

					template<typename dimension_t,
						bool Monochrome>
					using BatteryOutside = typename TypeTraits::TypeConditional::conditional_type<
						BatteryOutsideMonochrome<dimension_t>,
						BatteryOutsideColor<dimension_t>,
						Monochrome
					>::type;

					template<typename dimension_t>
					using BatteryBarsWholeMonochrome = Framework::Shader::Source::StaticColor<dimension_t>;

					template<typename dimension_t>
					using BatteryBarsWholeColor = Framework::Shader::Source::SingleColor<dimension_t>;

					template<typename dimension_t,
						bool Monochrome>
					using BatteryBarsWhole = typename TypeTraits::TypeConditional::conditional_type<
						BatteryBarsWholeMonochrome<dimension_t>,
						BatteryBarsWholeColor<dimension_t>,
						Monochrome
					>::type;

					template<typename dimension_t>
					using BatteryBarsLastMonochrome = Framework::Shader::Source::StaticColor<dimension_t>;

					template<typename dimension_t>
					using BatteryBarsLastColor = Framework::Shader::Source::SingleColor<dimension_t>;

					template<typename dimension_t,
						bool Monochrome>
					using BatteryBarsLast = typename TypeTraits::TypeConditional::conditional_type<
						BatteryBarsLastMonochrome<dimension_t>,
						BatteryBarsLastColor<dimension_t>,
						Monochrome
					>::type;
				}

				namespace Color
				{
					template<typename dimension_t,
						typename Base = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryOutsideMonochrome = Base;

					template<typename dimension_t,
						typename Base = Framework::Shader::Color::BaseShader<dimension_t>
					>
					using BatteryOutsideColor = Framework::Shader::Color::NoShader<dimension_t>;

					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryOutside = typename TypeTraits::TypeConditional::conditional_type<
						BatteryOutsideMonochrome<dimension_t, ColorShaderType>,
						BatteryOutsideColor<dimension_t, ColorShaderType>,
						Monochrome
					>::type;

					template<typename dimension_t,
						typename Base = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsWholeMonochrome = Base;

					template<typename dimension_t,
						typename Base = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsWholeColor = Base;

					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsWhole = typename TypeTraits::TypeConditional::conditional_type<
						BatteryBarsWholeMonochrome<dimension_t>,
						BatteryBarsWholeColor<dimension_t>,
						Monochrome
					>::type;


					template<typename dimension_t,
						typename Base = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsLastMonochrome = Base;

					template<typename dimension_t,
						typename Base = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsLastColor = Framework::Assets::Shader::Color::Brightness<dimension_t, Base>;

					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>
					>
					using BatteryBarsLast = typename TypeTraits::TypeConditional::conditional_type<
						BatteryBarsLastMonochrome<dimension_t>,
						BatteryBarsLastColor<dimension_t>,
						Monochrome
					>::type;
				}

				namespace Transform
				{
					template<typename dimension_t,
						bool Monochrome,
						typename Base = Framework::Shader::Transform::NoTransform<dimension_t>
					>
					using BatteryOutside = Base;

					template<typename dimension_t,
						bool Monochrome,
						typename Base = Framework::Shader::Transform::NoTransform<dimension_t>
					>
					using BatteryBarsWhole = Base;

					template<typename dimension_t,
						bool Monochrome,
						typename Base = Framework::Shader::Transform::NoTransform<dimension_t>
					>
					using BatteryBarsLast = typename TypeTraits::TypeConditional::conditional_type<
						Framework::Assets::Shader::Transform::DitherMask<dimension_t>,
						Base,
						Monochrome
					>::type;
				}

				namespace Pixel
				{
					template<typename dimension_t,
						bool Monochrome,
						typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>
					>
					using BatteryOutside = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::BatteryOutside<dimension_t, Monochrome>,
						Color::BatteryOutside<dimension_t, Monochrome, ColorShaderType>,
						Transform::BatteryOutside<dimension_t, Monochrome, TransformShaderType>
					>;

					template<typename dimension_t,
						bool Monochrome>
					using BatteryBarsWhole = Framework::Shader::Pixel::TemplateShader < dimension_t,
						Source::BatteryBarsWhole<dimension_t, Monochrome>,
						Color::BatteryBarsWhole<dimension_t, Monochrome>,
						Transform::BatteryBarsWhole<dimension_t, Monochrome>
					>;

					template<typename dimension_t,
						bool Monochrome>
					using BatteryBarsLast = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::BatteryBarsLast<dimension_t, Monochrome>,
						Color::BatteryBarsLast<dimension_t, Monochrome>,
						Transform::BatteryBarsLast<dimension_t, Monochrome>
					>;
				}
			}
		}
	}
}

#endif