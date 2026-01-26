#ifndef _EGFX_FRAMEWORK_LAYOUT_MARGIN_h
#define _EGFX_FRAMEWORK_LAYOUT_MARGIN_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Layout
		{
			/// <summary>
			/// Applies fixed margins (in pixels) to a parent layout, producing an inset child layout.
			///
			/// - The resulting layout is always top-left anchored to the (margined) area.
			/// - Each margin is clamped so it cannot exceed the available size at that step (prevents negative sizes).
			/// - Width/Height are reduced by the effective left+right / top+bottom margins.
			///
			/// Notes:
			/// - Margins are specified per side. For uniform margins, use the same value for all sides.
			/// - If the margins fully consume the parent width/height, the resulting width/height becomes 0.
			/// </summary>
			/// <typeparam name="ParentLayout">The layout type of the parent container.</typeparam>
			/// <typeparam name="MarginLeft">Left margin in pixels (>= 0).</typeparam>
			/// <typeparam name="MarginTop">Top margin in pixels (>= 0).</typeparam>
			/// <typeparam name="MarginRight">Right margin in pixels (>= 0).</typeparam>
			/// <typeparam name="MarginBottom">Bottom margin in pixels (>= 0).</typeparam>
			template<typename ParentLayout,
				pixel_t MarginLeft = 0,
				pixel_t MarginTop = 0,
				pixel_t MarginRight = 0,
				pixel_t MarginBottom = 0
			>
			struct Margin
			{
				static_assert(MarginLeft >= 0, "MarginLayout: MarginLeft must be >= 0.");
				static_assert(MarginTop >= 0, "MarginLayout: MarginTop must be >= 0.");
				static_assert(MarginRight >= 0, "MarginLayout: MarginRight must be >= 0.");
				static_assert(MarginBottom >= 0, "MarginLayout: MarginBottom must be >= 0.");

			private:
				static constexpr pixel_t Left()
				{
					return (MarginLeft >= ParentLayout::Width()) ? ParentLayout::Width() : MarginLeft;
				}

				static constexpr pixel_t Top()
				{
					return (MarginTop >= ParentLayout::Height()) ? ParentLayout::Height() : MarginTop;
				}

				static constexpr pixel_t WidthAfterLeft()
				{
					return ParentLayout::Width() - Left();
				}

				static constexpr pixel_t HeightAfterTop()
				{
					return ParentLayout::Height() - Top();
				}

				static constexpr pixel_t Right()
				{
					return (MarginRight >= WidthAfterLeft()) ? WidthAfterLeft() : MarginRight;
				}

				static constexpr pixel_t Bottom()
				{
					return (MarginBottom >= HeightAfterTop()) ? HeightAfterTop() : MarginBottom;
				}

			public:
				/// <summary>
				/// Top-left of the inset area (parent origin plus effective left/top margins).
				/// </summary>
				static constexpr pixel_t X() { return ParentLayout::X() + Left(); }
				static constexpr pixel_t Y() { return ParentLayout::Y() + Top(); }

				/// <summary>
				/// Size of the inset area (parent size minus effective margins).
				/// </summary>
				static constexpr pixel_t Width() { return WidthAfterLeft() - Right(); }
				static constexpr pixel_t Height() { return HeightAfterTop() - Bottom(); }
			};
		}
	}
}
#endif