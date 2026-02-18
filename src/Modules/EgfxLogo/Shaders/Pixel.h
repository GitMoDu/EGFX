#ifndef _EGFX_MODULES_EGFX_LOGO_SHADER_PIXEL_h
#define _EGFX_MODULES_EGFX_LOGO_SHADER_PIXEL_h

#include "../Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace Shader
			{
				namespace Pixel
				{
					using dimension_t = Dimensions::dimension_t;

					/// <summary>
					/// Templated pixel shader for rendering the screen effect in monochrome mode.
					/// </summary>
					using ScreenMonochrome = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::StaticColor<dimension_t>,
						Color::ScreenDitherAlpha,
						Framework::Assets::Shader::Transform::Translate<dimension_t>
					>;

					/// <summary>
					/// Templated pixel shader for rendering the screen effect in color mode.
					/// </summary>
					using ScreenColor = Framework::Shader::Pixel::TemplateShader<
						dimension_t,
						Framework::Shader::Source::SingleColor<dimension_t>,
						Color::ScreenColorAlpha,
						Framework::Assets::Shader::Transform::Translate<dimension_t>
					>;

					/// <summary>
					/// Forwards the appropriate screen pixel shader based on the monochrome flag.
					/// </summary>
					/// <typeparam name="Monochrome"></typeparam>
					template<bool Monochrome>
					using Screen = typename TypeTraits::TypeConditional::conditional_type<
						ScreenMonochrome,
						ScreenColor,
						Monochrome>::type;
				}
			}
		}
	}
}
#endif