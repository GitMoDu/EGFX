#ifndef _INTEGERGLASS_MODULES_PLOT_DRAWABLE_SELECTOR_h
#define _INTEGERGLASS_MODULES_PLOT_DRAWABLE_SELECTOR_h

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
				template<
					typename dimension_t,
					typename ParentLayout,
					typename PlotStyle = TemplatePlotStyle<>,
					typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename PlotValueSourceType = Data::LinearBuffer<typename PlotStyle::value_t, PlotStyle::SampleCount>
				>
				using TemplatePlot = typename TypeTraits::TypeConditional::conditional_type<
					/* IfTrue  */ Bar<dimension_t, ParentLayout, PlotStyle, PixelShaderType, PlotValueSourceType>,
					/* IfFalse */ typename TypeTraits::TypeConditional::conditional_type<
					/* IfTrue  */ Step<dimension_t, ParentLayout, PlotStyle, PixelShaderType, PlotValueSourceType>,
					/* IfFalse */ typename TypeTraits::TypeConditional::conditional_type<
					/* IfTrue  */ Fill<dimension_t, ParentLayout, PlotStyle, PixelShaderType, PlotValueSourceType>,
					/* IfFalse */ Line<dimension_t, ParentLayout, PlotStyle, PixelShaderType, PlotValueSourceType>,
					/* Condition */ (PlotStyle::PlotType == PlotTypeEnum::FillLow)
					|| (PlotStyle::PlotType == PlotTypeEnum::FillCenter)
					|| (PlotStyle::PlotType == PlotTypeEnum::FillHigh)
					>::type,
					/* Condition */ PlotStyle::PlotType == PlotTypeEnum::Step
					>::type,
					/* Condition */ (PlotStyle::PlotType == PlotTypeEnum::BarLow)
					|| (PlotStyle::PlotType == PlotTypeEnum::BarCenter)
					|| (PlotStyle::PlotType == PlotTypeEnum::BarHigh)
				>::type;
			}
		}
	}
}
#endif
