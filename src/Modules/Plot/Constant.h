#ifndef _EGFX_MODULES_PLOT_CONSTANT_h
#define _EGFX_MODULES_PLOT_CONSTANT_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			enum class PlotTypeEnum : uint8_t
			{
				Line,
				Step,
				FillLow,
				FillCenter,
				FillHigh
			};

			using count_t = uint16_t;

			template<count_t sampleCount = 16,
				PlotTypeEnum plotType = PlotTypeEnum::Line,
				bool flipOrientation = false
			>
			struct TemplatePlotStyle
			{
				static constexpr count_t SampleCount = sampleCount;
				static constexpr PlotTypeEnum PlotType = plotType;
				static constexpr bool FlipOrientation = flipOrientation;
			};
		}
	}
}

#endif