#ifndef _EGFX_MODULES_PLOT_LAYOUT_h
#define _EGFX_MODULES_PLOT_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace Plot
		{
			namespace Layout
			{
				template<typename ParentLayout,
					typename PlotStyle = TemplatePlotStyle<>
				>
				struct SeriesHorizontal
				{
					static constexpr uint16_t WidthMinus1()
					{
						return static_cast<uint16_t>(ParentLayout::Width() - 1);
					}

					template<typename dimension_t>
					static constexpr dimension_t GetSampleX(const count_t sampleIndex)
					{
						return static_cast<dimension_t>(
							((static_cast<uint32_t>(sampleIndex) * WidthMinus1())) / (static_cast<uint32_t>(PlotStyle::SampleCount) - 1u));
					}

					template<typename dimension_t>
					static constexpr uint32_t GetStepBoundaryX(const count_t leftSampleIndex, const count_t rightSampleIndex)
					{
						const uint32_t xLeft = static_cast<uint32_t>(GetSampleX<dimension_t>(leftSampleIndex));
						const uint32_t xRight = static_cast<uint32_t>(GetSampleX<dimension_t>(rightSampleIndex));

						return (xLeft + xRight + 1u) / 2u;
					}

					template<typename dimension_t>
					static constexpr uint32_t GetStepStartX(const count_t sampleIndex)
					{
						if (sampleIndex == 0)
						{
							return 0u;
						}

						return GetStepBoundaryX<dimension_t>(static_cast<count_t>(sampleIndex - 1), sampleIndex);
					}

					template<typename dimension_t>
					static constexpr uint32_t GetStepEndXExclusive(const count_t sampleIndex)
					{
						if (sampleIndex >= static_cast<count_t>(PlotStyle::SampleCount - 1))
						{
							return static_cast<uint32_t>(WidthMinus1()) + 1u;
						}

						return GetStepBoundaryX<dimension_t>(sampleIndex, static_cast<count_t>(sampleIndex + 1));
					}

					template<typename dimension_t>
					static constexpr dimension_t TopBaseline()
					{
						return 0;
					}

					template<typename dimension_t>
					static constexpr dimension_t BottomBaseline()
					{
						return static_cast<dimension_t>(ParentLayout::Height() - 1);
					}

					template<typename dimension_t>	
					static constexpr dimension_t CenterBaseline()
					{
						return static_cast<dimension_t>((ParentLayout::Height() - 1) / 2);
					}
				};
			}
		}
	}
}
#endif