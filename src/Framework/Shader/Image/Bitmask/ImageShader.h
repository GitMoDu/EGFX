#ifndef _EGFX_FRAMEWORK_SHADER_IMAGE_BITMASK_IMAGE_SHADER_h
#define _EGFX_FRAMEWORK_SHADER_IMAGE_BITMASK_IMAGE_SHADER_h

#include "Unscaled.h"
#include "Scaled.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Image
			{
				namespace Bitmask
				{
					// Template selector for Scaled vs Unscaled bitmask shaders based on ScaleX and ScaleY.
					template<typename dimension_t,
						Framework::DataSourceTypeEnum DataSourceType = Framework::DataSourceTypeEnum::Ram,
						bool UseTransparency = true,
						rgb_color_t TransparentColor = RGB_COLOR_BLACK,
						uint8_t ScaleX = 1,
						uint8_t ScaleY = 1,
						Framework::Image::ScaledTransformOrderEnum TransformOrder = Framework::Image::ScaledTransformOrderEnum::TransformThenScale,
						typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
						typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
						Shader::Pixel::BlendModeEnum BlendMode = Shader::Pixel::BlendModeEnum::Replace
					>
					using ImageShader = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Scaled<dimension_t, ScaleX, ScaleY, DataSourceType, UseTransparency, TransparentColor, TransformOrder, ColorShaderType, TransformShaderType, BlendMode>,
						Unscaled<dimension_t, DataSourceType, UseTransparency, TransparentColor, ColorShaderType, TransformShaderType, BlendMode>,
						(ScaleX > 1 || ScaleY > 1)>::type;
				}
			}
		}
	}
}
#endif