#ifndef _EGFX_FRAMEWORK_LAYOUT_CONSTRAINED_DIMENSIONS_h
#define _EGFX_FRAMEWORK_LAYOUT_CONSTRAINED_DIMENSIONS_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Constrains a child rectangle to min/max dimensions within a parent layout.
			///
			/// Behavior:
			/// - Computes Width/Height by clamping the parent size into the provided min/max range:
			///   - Width  = clamp(ParentWidth,  minWidth,  maxWidth)
			///   - Height = clamp(ParentHeight, minHeight, maxHeight)
			/// - The result is always top-left anchored to the parent (X/Y match the parent).
			///
			/// Notes:
			/// - The min/max bounds are compile-time constants.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="minWidth">Minimum width (pixels).</typeparam>
			/// <typeparam name="maxWidth">Maximum width (pixels).</typeparam>
			/// <typeparam name="minHeight">Minimum height (pixels).</typeparam>
			/// <typeparam name="maxHeight">Maximum height (pixels).</typeparam>
			template<typename ParentLayout,
				pixel_t minWidth,
				pixel_t maxWidth,
				pixel_t minHeight,
				pixel_t maxHeight
			>
			struct ConstrainedDimensions
			{
				static_assert(minWidth <= maxWidth, "ConstrainedDimensions requires minWidth <= maxWidth.");
				static_assert(minHeight <= maxHeight, "ConstrainedDimensions requires minHeight <= maxHeight.");

				static constexpr pixel_t MinWidth() { return minWidth; }
				static constexpr pixel_t MaxWidth() { return maxWidth; }
				static constexpr pixel_t MinHeight() { return minHeight; }
				static constexpr pixel_t MaxHeight() { return maxHeight; }

				/// <summary>
				/// Constrained width of the result (clamped parent width).
				/// </summary>
				static constexpr pixel_t Width()
				{
					return MinValue<pixel_t>(
						MaxValue<pixel_t>(MinWidth(), ParentLayout::Width()),
						MaxWidth());
				}

				/// <summary>
				/// Constrained height of the result (clamped parent height).
				/// </summary>
				static constexpr pixel_t Height()
				{
					return MinValue<pixel_t>(
						MaxValue<pixel_t>(MinHeight(), ParentLayout::Height()),
						MaxHeight());
				}

				/// <summary>
				/// Top-left anchor of the constrained rectangle.
				/// </summary>
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }
			};
		}
	}
}
#endif