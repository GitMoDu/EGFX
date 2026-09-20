#ifndef _INTEGERGLASS_MODULES_PLOT_DRAWABLE_h
#define _INTEGERGLASS_MODULES_PLOT_DRAWABLE_h

#include "DrawableBar.h"
#include "DrawableFill.h"
#include "DrawableLine.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Drawable
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename PlotStyle = TemplatePlotStyle<>,
					typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
					Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace,
					typename PlotValueSourceType = Data::LinearBuffer<typename PlotStyle::value_t, PlotStyle::SampleCount>>
					using TemplatePlot = typename TypeTraits::TypeConditional::conditional_type<
					Bar<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>,
					typename TypeTraits::TypeConditional::conditional_type<
					Step<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>,
					typename TypeTraits::TypeConditional::conditional_type<
					Fill<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>,
					Line<dimension_t, ParentLayout, PlotStyle, ColorShaderType, TransformShaderType, BlendMode, PlotValueSourceType>,
					(PlotStyle::PlotType == PlotTypeEnum::FillLow)
					|| (PlotStyle::PlotType == PlotTypeEnum::FillCenter)
					|| (PlotStyle::PlotType == PlotTypeEnum::FillHigh)
					>::type,
					PlotStyle::PlotType == PlotTypeEnum::Step
					>::type,
					(PlotStyle::PlotType == PlotTypeEnum::BarLow)
					|| (PlotStyle::PlotType == PlotTypeEnum::BarCenter)
					|| (PlotStyle::PlotType == PlotTypeEnum::BarHigh)
					>::type;
			}
		}
	}
}

#endif