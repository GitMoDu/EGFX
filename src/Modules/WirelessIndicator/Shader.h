#ifndef _EGFX_MODULES_WIRELESS_INDICATOR_SHADER_h
#define _EGFX_MODULES_WIRELESS_INDICATOR_SHADER_h

#include "Constant.h"
#include "Assets.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Shader
			{
				using dimension_t = pixel_t;

				namespace Source
				{
					template<bool Monochrome>
					using Bar = typename TypeTraits::TypeConditional::conditional_type<
						Framework::Shader::Source::StaticColor<dimension_t, RGB_COLOR_WHITE>,
						Framework::Shader::Source::SingleColor<dimension_t, RGB_COLOR_WHITE>,
						Monochrome
					>::type;

					using DisabledOverlay = typename TypeTraits::TypeConditional::conditional_type <
						Framework::Shader::Source::StaticColor<dimension_t, RGB_COLOR_WHITE>,
						Framework::Shader::Source::SingleColor<dimension_t, RGB_COLOR_WHITE>,
						false
					>::type;
				}

				namespace Transform
				{
					template<dimension_t Width,
						dimension_t Height,
						bool Monochrome>
					using Bar = typename TypeTraits::TypeConditional::conditional_type<
						Framework::Assets::Shader::Transform::CheckerboardMask<
						dimension_t,
						false,
						Framework::Assets::Shader::Transform::ScaleDownX<dimension_t, Width, Height>
						>,
						Framework::Assets::Shader::Transform::ScaleDownX<dimension_t, Width, Height>,
						Monochrome
					>::type;
				}

				namespace Pixel
				{
					template<dimension_t Width,
						dimension_t Height,
						bool Monochrome>
					using Bar = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::Bar<Monochrome>,
						Framework::Shader::Color::NoShader<dimension_t>,
						Transform::Bar<Width, Height, Monochrome>
					>;

					using DisabledOverlay = Framework::Shader::Pixel::TemplateShader<dimension_t, Source::DisabledOverlay>;
				}

				namespace Primitive
				{
					template<dimension_t Width,
						dimension_t Height,
						bool Monochrome>
					using Bar = Framework::Shader::Primitive::TemplateShader<dimension_t,
						Pixel::Bar<Width, Height, Monochrome>
					>;

					using DisabledOverlay = Framework::Shader::Primitive::TemplateShader<dimension_t, Pixel::DisabledOverlay>;
				}
			}
		}
	}
}
#endif