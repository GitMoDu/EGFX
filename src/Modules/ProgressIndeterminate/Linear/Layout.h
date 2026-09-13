#ifndef _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_LAYOUT_h
#define _EGFX_MODULES_PROGRESS_INDETERMINATE_LINEAR_LAYOUT_h

#include "Constant.h"


namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndeterminate
		{
			namespace Linear
			{
				namespace Layout
				{
					template<typename dimension_t, typename ParentLayout>
					struct Calc
					{
						static constexpr bool IsVertical()
						{
							return false;
							/*return (Style::Orientation == OrientationEnum::TopToBottom) ||
								(Style::Orientation == OrientationEnum::BottomToTop);*/
						}

						static constexpr bool IsReversed()
						{
							return false;
							/*return (Style::Orientation == OrientationEnum::RightToLeft) ||
								(Style::Orientation == OrientationEnum::BottomToTop);*/
						}

						static constexpr pixel_t AxisLength()
						{
							return IsVertical() ? ParentLayout::Height() : ParentLayout::Width();
						}

						static constexpr pixel_rectangle_t Bounds()
						{
							return pixel_rectangle_t{
								{ 0, 0},
								{ ParentLayout::Width() - 1, ParentLayout::Height() - 1 }
							};
						}

						static constexpr pixel_t RangeHorizontal()
						{
							return Bounds().bottomRight.x - Bounds().topLeft.x;
						}

						static constexpr pixel_t RangeVertical()
						{
							return Bounds().bottomRight.y - Bounds().topLeft.y;
						}

						static constexpr pixel_t RangeLength()
						{
							return IsVertical() ? RangeVertical() : RangeHorizontal();
						}
						
					};
				}
			}
		}
	}
}

#endif
