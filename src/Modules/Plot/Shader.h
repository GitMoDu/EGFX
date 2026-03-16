#ifndef _EGFX_MODULES_PLOT_SHADER_h
#define _EGFX_MODULES_PLOT_SHADER_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Shader
			{
				namespace Source
				{
					template<typename dimension_t>
					using PlotSeries = Framework::Shader::Source::SingleColor<dimension_t, RGB_COLOR_WHITE>;
				}

				namespace Color
				{
					template<typename dimension_t>
					using PlotSeries = Framework::Shader::Color::NoShader<dimension_t>;
				}

				namespace Transform
				{
				}

				namespace Pixel
				{
					template<typename dimension_t>
					using PlotSeries = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::PlotSeries<dimension_t>,
						Color::PlotSeries<dimension_t>,
						Framework::Shader::Transform::NoTransform<dimension_t>
					>;

					template<typename dimension_t>
					using PlotDither = Framework::Shader::Pixel::TemplateShader<dimension_t,
						Source::PlotSeries<dimension_t>,
						Color::PlotSeries<dimension_t>,
						Framework::Assets::Shader::Transform::CheckerboardMask<dimension_t>
					>;
				}
			}
		}
	}
}

#endif