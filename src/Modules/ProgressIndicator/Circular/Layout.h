#ifndef _EGFX_MODULES_PROGRESS_INDICATOR_CIRCULAR_LAYOUT_h
#define _EGFX_MODULES_PROGRESS_INDICATOR_CIRCULAR_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Circular
			{
				namespace Layout
				{
					template<typename ParentLayout, typename Style>
					struct Calc
					{

						static constexpr pixel_t RadiusRaw()
						{
							return MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height()) / 2;
						}

						static constexpr bool IsRadiusOdd()
						{
							return RadiusRaw() % 2 == 1;
						}

						static constexpr bool HasFillMargin()
						{
							return Style::FillMargin > 0;
						}

						// Radius needs odd adjustment otherwise the outer ring will exceed the bounds of the layout and be clipped.
						static constexpr pixel_t Radius()
						{
							return MaxValue<pixel_t>(0, RadiusRaw() - (IsRadiusOdd() ? 0 : 1));
						}

						static constexpr pixel_t CenterX()
						{
							return ParentLayout::Width() / 2;
						}

						static constexpr pixel_t CenterY()
						{
							return ParentLayout::Height() / 2;
						}

						static constexpr pixel_t InnerRadius()
						{
							return LimitValue<pixel_t>(Radius() - Style::ArcThickness, 0, Radius());
						}

						static constexpr pixel_t OutlineOuterRadius()
						{
							return Radius();
						}

						static constexpr pixel_t OutlineInnerRadius()
						{
							return LimitValue<pixel_t>(Radius() - Style::InnerBorderStroke, 0, Radius());
						}

						// Outline margin influences the fill radius.
						static constexpr pixel_t OutlineMargin()
						{
							return (Style::OuterBorderStroke > 1) ? (Style::OuterBorderStroke - 0) : 0;
						}

						static constexpr pixel_t InnerLineOuterRadius()
						{
							return InnerRadius() + Style::InnerBorderStroke;
						}


						//  If the outline is more than a line,
						//  the fill radius needs to be reduced to avoid overlapping the thick outline.
						static constexpr pixel_t FillRadius()
						{
							return MaxValue<pixel_t>(0, Radius() - Style::IndicatorMargin - OutlineMargin());
						}

						static constexpr pixel_t FillOuterRadius()
						{
							return OutlineInnerRadius() - ((Style::IndicatorMargin > 0) ? (Style::IndicatorMargin - 1) : 0);
						}

						static constexpr pixel_t FillInnerRadius()
						{
							return InnerLineOuterRadius() + Style::IndicatorMargin;
						}

						static constexpr pixel_t TrackOuterRadius()
						{
							return OutlineInnerRadius() - 1 - ((Style::TrackMargin > 0) ? (Style::TrackMargin - 1) : 0);
						}

						static constexpr pixel_t TrackInnerRadius()
						{
							return InnerLineOuterRadius() + Style::TrackMargin;
						}

						static constexpr angle_t FillAngleRange()
						{
							return static_cast<angle_t>(Style::EndAngle - Style::StartAngle);
						}

						static constexpr bool HasOuterBorder()
						{
							return Style::OuterBorderStroke > 0;
						}

						static constexpr bool HasInnerBorder()
						{
							return Style::InnerBorderStroke > 0;
						}

						static constexpr bool DrawOuter()
						{
							return HasOuterBorder();
						}

						static constexpr bool DrawInner()
						{
							return HasInnerBorder();
						}

						static constexpr bool DrawFill()
						{
							return FillRadius() > 0;
						}

						static constexpr bool DrawFlatConnectors()
						{
							return DrawOuter() && DrawInner();
						}

					};
				}
			}
		}
	}
}

#endif
