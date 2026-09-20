#ifndef _INTEGERGLASS_MODULES_PLOT_CONSTANT_h
#define _INTEGERGLASS_MODULES_PLOT_CONSTANT_h

#include "../../IntegerGlassFramework.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			using count_t = uint16_t;

			/// <summary>
			/// Selects the geometry used to render the plot samples.
			/// </summary>
			enum class PlotTypeEnum : uint8_t
			{
				// Draws straight line segments between adjacent samples.
				Line,

				// Draws horizontal or vertical step segments using each sample value.
				Step,

				// Fills the area between the sample line and the low-value baseline.
				FillLow,

				// Fills the area between the sample line and the center baseline.
				FillCenter,

				// Fills the area between the sample line and the high-value baseline.
				FillHigh,

				// Draws bars from the low-value baseline to each sample value.
				BarLow,

				// Draws bars from the center baseline to each sample value.
				BarCenter,

				// Draws bars from the high-value baseline to each sample value.
				BarHigh
			};

			/// <summary>
			/// Selects the direction in which samples advance across the plot.
			/// </summary>
			enum class PlotOrientationEnum : uint8_t
			{
				// Samples advance from the left edge to the right edge.
				HorizontalLeftToRight,

				// Samples advance from the right edge to the left edge.
				HorizontalRightToLeft,

				// Samples advance from the top edge to the bottom edge.
				VerticalTopToBottom,

				// Samples advance from the bottom edge to the top edge.
				VerticalBottomToTop
			};

			/// <summary>
			/// Unsigned integer type used for plot sample counts and indices.
			/// </summary>
			using count_t = uint16_t;

			/// <summary>
			/// Compile-time configuration for a plot series.
			/// </summary>
			/// <typeparam name="sampleCount">Number of samples retained and rendered by the series.</typeparam>
			/// <typeparam name="plotOrientation">Direction in which samples advance across the plot.</typeparam>
			/// <typeparam name="plotType">Geometry used to render the samples.</typeparam>
			/// <typeparam name="lineThickness">Thickness of line, step, or bar geometry in pixels.</typeparam>
			template<
				count_t sampleCount = 16,
				typename plot_value_type = uint8_t,
				PlotOrientationEnum plotOrientation = PlotOrientationEnum::HorizontalLeftToRight,
				PlotTypeEnum plotType = PlotTypeEnum::Line,
				pixel_t lineThickness = 1,
				pixel_t segmentSpacing = 0
			>
			struct TemplatePlotStyle
			{
				using value_t = plot_value_type;

				static constexpr count_t SampleCount = sampleCount;
				static constexpr PlotOrientationEnum PlotOrientation = plotOrientation;
				static constexpr PlotTypeEnum PlotType = plotType;
				static constexpr pixel_t LineThickness = lineThickness;
				static constexpr pixel_t SegmentSpacing = segmentSpacing;
			};
		}
	}
}

#endif