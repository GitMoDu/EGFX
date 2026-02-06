#ifndef _EGFX_FRAMEWORK_LAYOUT_CONSTRAINED_RATIO_h
#define _EGFX_FRAMEWORK_LAYOUT_CONSTRAINED_RATIO_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Constrains a child area to a fixed aspect ratio inside a parent layout.
			///
			/// Behavior:
			/// - Computes the largest rectangle with aspect (AspectWidth:AspectHeight) that fits inside ParentLayout.
			/// - Result is top-left anchored to the parent.
			/// </summary>
			/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
			/// <typeparam name="AspectWidth">Aspect ratio numerator (must be > 0).</typeparam>
			/// <typeparam name="AspectHeight">Aspect ratio denominator (must be > 0).</typeparam>
			template<typename ParentLayout,
				pixel_t AspectWidth,
				pixel_t AspectHeight
			>
			struct ConstrainedRatio
			{
				static_assert(AspectHeight > 0 && AspectWidth > 0, "Aspect ratio values must be greater than zero.");

			private:
				static constexpr pixel_t AvailableWidth() { return ParentLayout::Width(); }
				static constexpr pixel_t AvailableHeight() { return ParentLayout::Height(); }

				// True when height is the limiting dimension (parent is wider than target aspect).
				static constexpr bool HeightLimited()
				{
					return (uint32_t(AvailableWidth()) * uint32_t(AspectHeight))
						>= (uint32_t(AvailableHeight()) * uint32_t(AspectWidth));
				}

			public:
				/// <summary>
				/// Constrained width of the aspect-fit rectangle.
				/// </summary>
				static constexpr pixel_t Width()
				{
					return HeightLimited()
						? static_cast<pixel_t>((uint32_t(AvailableHeight()) * uint32_t(AspectWidth)) / uint32_t(AspectHeight))
						: AvailableWidth();
				}

				/// <summary>
				/// Constrained height of the aspect-fit rectangle.
				/// </summary>
				static constexpr pixel_t Height()
				{
					return HeightLimited()
						? AvailableHeight()
						: static_cast<pixel_t>((uint32_t(AvailableWidth()) * uint32_t(AspectHeight)) / uint32_t(AspectWidth));
				}

				// Top-left aligned contract.
				static constexpr pixel_t X() { return ParentLayout::X(); }
				static constexpr pixel_t Y() { return ParentLayout::Y(); }
			};
		}
	}
}
#endif