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
					static constexpr bool IsVertical()
					{
						return (PlotStyle::PlotOrientation == PlotOrientationEnum::VerticalTopToBottom) ||
							(PlotStyle::PlotOrientation == PlotOrientationEnum::VerticalBottomToTop);
					}

					static constexpr bool IsReversed()
					{
						return (PlotStyle::PlotOrientation == PlotOrientationEnum::HorizontalRightToLeft) ||
							(PlotStyle::PlotOrientation == PlotOrientationEnum::VerticalBottomToTop);
					}

					static constexpr uint16_t SampleAxisLength()
					{
						return IsVertical() ? ParentLayout::Height() : ParentLayout::Width();
					}

					static constexpr uint16_t ValueAxisLength()
					{
						return IsVertical() ? ParentLayout::Width() : ParentLayout::Height();
					}

					static constexpr uint16_t WidthMinus1()
					{
						return static_cast<uint16_t>(SampleAxisLength() - 1);
					}

					static constexpr uint16_t EffectiveSampleAxisLength()
					{
						return static_cast<uint16_t>(SampleCellSize() * PlotStyle::SampleCount);
					}

					static constexpr uint16_t SampleCellSize()
					{
						return static_cast<uint16_t>(SampleAxisLength() / PlotStyle::SampleCount);
					}

					static constexpr uint16_t UnusedSampleAxisLength()
					{
						return static_cast<uint16_t>(SampleAxisLength() - EffectiveSampleAxisLength());
					}

					static constexpr uint16_t ValueAxisMinus1()
					{
						return static_cast<uint16_t>(ValueAxisLength() - 1);
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
					return (static_cast<uint32_t>(GetSampleX<dimension_t>(leftSampleIndex)) +
						static_cast<uint32_t>(GetSampleX<dimension_t>(rightSampleIndex)) + 1u) / 2u;
					}

					template<typename dimension_t>
					static constexpr uint32_t GetStepStartX(const count_t sampleIndex)
					{
					return (sampleIndex == 0)
						? 0u
						: GetStepBoundaryX<dimension_t>(static_cast<count_t>(sampleIndex - 1), sampleIndex);
					}

					template<typename dimension_t>
					static constexpr uint32_t GetStepEndXExclusive(const count_t sampleIndex)
					{
					return (sampleIndex >= static_cast<count_t>(PlotStyle::SampleCount - 1))
						? (static_cast<uint32_t>(WidthMinus1()) + 1u)
						: GetStepBoundaryX<dimension_t>(sampleIndex, static_cast<count_t>(sampleIndex + 1));
					}

					template<typename dimension_t>
					static constexpr uint32_t GetSegmentStartX(const count_t sampleIndex)
					{
						return static_cast<uint32_t>(sampleIndex) * SampleCellSize();
					}

					template<typename dimension_t>
					static constexpr uint32_t GetSegmentEndXExclusive(const count_t sampleIndex)
					{
						return GetSegmentStartX<dimension_t>(sampleIndex) +
							((SampleCellSize() > static_cast<uint32_t>(PlotStyle::SegmentSpacing))
								? static_cast<uint32_t>(SampleCellSize()) - static_cast<uint32_t>(PlotStyle::SegmentSpacing)
								: 0u);
					}

					template<typename dimension_t>
					static constexpr dimension_t TopBaseline()
					{
						return 0;
					}

					template<typename dimension_t>
					static constexpr dimension_t BottomBaseline()
					{
						return static_cast<dimension_t>(ValueAxisMinus1());
					}

					template<typename dimension_t>	
					static constexpr dimension_t CenterBaseline()
					{
						return static_cast<dimension_t>(ValueAxisMinus1() / 2);
					}
				};
			}
		}
	}
}
#endif