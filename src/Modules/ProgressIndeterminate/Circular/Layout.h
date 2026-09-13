#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_LAYOUT_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_CIRCULAR_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Circular
			{
				namespace Layout
				{
					template<typename dimension_t, typename ParentLayout, dimension_t ArcThickness>
					struct Calc
					{
						static constexpr dimension_t RadiusRaw()
						{
							return MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height()) / 2;
						}

						static constexpr bool IsRadiusOdd()
						{
							return RadiusRaw() % 2 == 1;
						}

						// Radius needs odd adjustment otherwise the outer ring will exceed the bounds of the layout and be clipped.
						static constexpr dimension_t Radius()
						{
							return MaxValue<pixel_t>(0, RadiusRaw() - (IsRadiusOdd() ? 0 : 1));
						}

						static constexpr dimension_t OuterRadius()
						{
							return Radius();
						}
						static constexpr dimension_t InnerRadius()
						{
							return LimitValue<pixel_t>(Radius() - ArcThickness, 0, Radius());
						}

						static constexpr dimension_t CenterX()
						{
							return ParentLayout::Width() / 2;
						}

						static constexpr dimension_t CenterY()
						{
							return ParentLayout::Height() / 2;
						}
					};
				}
			}
		}
	}
}

#endif
