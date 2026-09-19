#ifndef _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_LAYOUT_h
#define _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Layout
			{
				template<typename ParentLayout, typename Style>
				struct Linear
				{
					static constexpr bool IsVertical()
					{
						return (Style::Orientation == OrientationEnum::TopToBottom) ||
							(Style::Orientation == OrientationEnum::BottomToTop);
					}

					static constexpr bool IsReversed()
					{
						return (Style::Orientation == OrientationEnum::RightToLeft) ||
							(Style::Orientation == OrientationEnum::BottomToTop);
					}

					static constexpr pixel_t AxisLength()
					{
						return IsVertical() ? ParentLayout::Height() : ParentLayout::Width();
					}

					// Outline margin influences the fill radius.
					static constexpr pixel_t OutlineMargin()
					{
						return (Style::OutlineWidth > 1) ? (Style::OutlineWidth - 0) : 0;
					}

					static constexpr pixel_t MaximumStrokeWidth()
					{
						return static_cast<pixel_t>(MinValue(ParentLayout::Width(), ParentLayout::Height()) / 2);
					}

					static constexpr pixel_t EffectiveStrokeWidth()
					{
						return static_cast<pixel_t>(MaxValue<pixel_t>(0, MinValue<pixel_t>(Style::OutlineWidth, AxisLength() / 2)));
					}

					static constexpr pixel_t EffectiveCornerRadius()
					{
						return static_cast<pixel_t>(MinValue(Style::CornerRadius, MaxValue<int16_t>(0, (AxisLength() / 2))));
					}

					static constexpr bool HasRoundedCorners()
					{
						return Style::CornerRadius > 0 && EffectiveCornerRadius() > 0;
					}

					static constexpr bool HasOutline()
					{
						return Style::OutlineWidth > 0 && EffectiveStrokeWidth() > 0;
					}

					static constexpr pixel_t FillRoundedDimension()
					{
						//return EffectiveCornerRadius() - (Style::FillMargin > 0 ? (Style::FillMargin - 1) : 0);
						return EffectiveCornerRadius() - ((EffectiveStrokeWidth() > 0) ? (EffectiveStrokeWidth() - 1) : 0) - Style::FillMargin;
					}

					static constexpr pixel_t FillRoundedRadius()
					{
						return MaxValue<pixel_t>(0, EffectiveCornerRadius() - OutlineMargin() - Style::FillMargin);
					}

					static constexpr pixel_t FillRangeHorizontal()
					{
						return MaxValue<pixel_t>(0,
							AxisLength() - 1 - Style::FillMargin - (EffectiveCornerRadius() * 2));
					}

					static constexpr pixel_t FillRangeVertical()
					{
						return MaxValue<pixel_t>(0,
							AxisLength() - 1 - Style::FillMargin - (EffectiveCornerRadius() * 2));
					}

					static constexpr pixel_t FillRange()
					{
						return IsVertical()
							? FillRangeVertical()
							: FillRangeHorizontal();
					}

					static constexpr pixel_point_t FillHorizontalRoundedTopCenter()
					{
						return pixel_point_t{
							EffectiveCornerRadius(),
							EffectiveCornerRadius()
						};
					}

					static constexpr pixel_point_t FillHorizontalRoundedBottomCenter()
					{
						return pixel_point_t{
							EffectiveCornerRadius(),
							ParentLayout::Height() - 1 - FillRoundedRadius() - Style::FillMargin
						};
					}


					static constexpr bool HasRoundedFill()
					{
						return FillRoundedRadius() > 0;
					}

					static constexpr pixel_rectangle_t FillHorizontal()
					{
						return pixel_rectangle_t{
							{ EffectiveCornerRadius(), EffectiveStrokeWidth() + Style::FillMargin},
							{ EffectiveCornerRadius() + FillRange(), ParentLayout::Height() - 1 - EffectiveStrokeWidth() - Style::FillMargin}
						};
					}

					static constexpr pixel_rectangle_t FillVertical()
					{
						return pixel_rectangle_t{
							{ FillRoundedDimension() + 1, EffectiveStrokeWidth() + Style::FillMargin },
							{ FillRoundedDimension() + FillRange(), ParentLayout::Height() - 1 - EffectiveStrokeWidth() - Style::FillMargin }
						};
					}

					static constexpr pixel_rectangle_t FillHorizontalLeftMiddle()
					{
						return pixel_rectangle_t{
							{ EffectiveStrokeWidth() + Style::FillMargin, FillHorizontalRoundedTopCenter().y },
							{ FillHorizontalRoundedTopCenter().x - 1, FillHorizontalRoundedBottomCenter().y }
						};
					}

					static constexpr pixel_rectangle_t FillHorizontalRightMiddle()
					{
						return pixel_rectangle_t{
							{ FillHorizontalRoundedTopCenter().x + 1, FillHorizontalRoundedTopCenter().y },
							{ FillHorizontalRoundedTopCenter().x + FillRoundedRadius(), FillHorizontalRoundedBottomCenter().y}
						};
					}

					static constexpr pixel_t Inset()
					{
						return static_cast<pixel_t>(Style::OutlineWidth + Style::FillMargin);
					}

					struct RectangleFill
					{
						static constexpr pixel_rectangle_t Bounds()
						{
							return pixel_rectangle_t{
								{ Inset(), Inset() },
								{ ParentLayout::Width() - 1 - Inset(), ParentLayout::Height() - 1 - Inset() }
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

						static constexpr bool HasFill()
						{
							return RangeLength() > 0;
						}
					};

					struct RoundedRectangleFill
					{
						static constexpr pixel_rectangle_t HorizontalRectangleBounds()
						{
							return pixel_rectangle_t{
								{ EffectiveCornerRadius(), 0 },
								{ ParentLayout::Width() - 1 - Style::FillMargin - EffectiveCornerRadius(), ParentLayout::Height() - 1 }
							};
						}

						static constexpr pixel_rectangle_t VerticalRectangleBounds()
						{
							return pixel_rectangle_t{
								{ 0, EffectiveCornerRadius() },
								{ ParentLayout::Width() - 1, ParentLayout::Height() - 1 - Style::FillMargin - EffectiveCornerRadius() }
							};
						}

						static constexpr pixel_rectangle_t RectangleBounds()
						{
							return IsVertical() ? VerticalRectangleBounds() : HorizontalRectangleBounds();
						}

						static constexpr pixel_rectangle_t RectangleFillBounds()
						{
							return pixel_rectangle_t{
								{ RectangleBounds().topLeft.x, RectangleBounds().topLeft.y + Inset() },
								{ RectangleBounds().bottomRight.x, RectangleBounds().bottomRight.y - Inset() }
							};
						}

						static constexpr pixel_rectangle_t LeftCapBounds()
						{
							return pixel_rectangle_t{
								{ 0, 0 },
								{ EffectiveCornerRadius(), ParentLayout::Height() - 1 }
							};
						}

						static constexpr pixel_rectangle_t RightCapBounds()
						{
							return pixel_rectangle_t{
								{ EffectiveCornerRadius(), 0 },
								{ EffectiveCornerRadius() + EffectiveCornerRadius(), ParentLayout::Height() - 1 }
							};
						}

						static constexpr pixel_rectangle_t TopCapBounds()
						{
							return pixel_rectangle_t{
								{ 0, 0 },
								{ ParentLayout::Width() - 1, EffectiveCornerRadius() }
							};
						}

						static constexpr pixel_rectangle_t BottomCapBounds()
						{
							return pixel_rectangle_t{
								{ 0, EffectiveCornerRadius() },
								{ ParentLayout::Width() - 1, EffectiveCornerRadius() + EffectiveCornerRadius() }
							};
						}



						static constexpr pixel_t RangeHorizontal()
						{
							return RectangleBounds().bottomRight.x - RectangleBounds().topLeft.x;
						}

						static constexpr pixel_t RangeVertical()
						{
							return RectangleBounds().bottomRight.y - RectangleBounds().topLeft.y;
						}

						static constexpr pixel_t RangeLength()
						{
							return IsVertical() ? RangeVertical() : RangeHorizontal();
						}

						static constexpr bool HasFill()
						{
							return RangeLength() > 0;
						}

						static constexpr pixel_rectangle_t HorizontalStartCap()
						{
							return IsReversed() ? RightCapBounds() : LeftCapBounds();
						}

						static constexpr pixel_rectangle_t HorizontalEndCap()
						{
							return IsReversed() ? LeftCapBounds() : RightCapBounds();
						}

						static constexpr pixel_point_t HorizontalStartCapTop()
						{
							return pixel_point_t{
								HorizontalStartCap().bottomRight.x, HorizontalStartCap().topLeft.y + EffectiveCornerRadius()
							};
						}

						static constexpr pixel_point_t HorizontalStartCapBottom()
						{
							return pixel_point_t{
								HorizontalStartCap().bottomRight.x, HorizontalStartCap().bottomRight.y - EffectiveCornerRadius()
							};
						}

						static constexpr pixel_rectangle_t HorizontalStartCapMiddle()
						{
							return pixel_rectangle_t{
								{ EffectiveStrokeWidth() + Style::FillMargin, HorizontalStartCap().topLeft.y + EffectiveCornerRadius() },
								{ HorizontalStartCap().bottomRight.x - 1, HorizontalStartCap().bottomRight.y - EffectiveCornerRadius() - 0 }
							};
						}

						static constexpr pixel_point_t HorizontalEndCapTop()
						{
							return pixel_point_t{
								HorizontalStartCapTop().x,
								HorizontalStartCapTop().y
							};
						}

						static constexpr pixel_point_t HorizontalEndCapBottom()
						{
							return pixel_point_t{
								HorizontalStartCapBottom().x,
								HorizontalStartCapBottom().y
							};
						}

						static constexpr pixel_rectangle_t HorizontalEndCapMiddle()
						{
							return pixel_rectangle_t{
								{ HorizontalStartCapMiddle().topLeft.x + FillRoundedDimension(), HorizontalStartCapMiddle().topLeft.y },
								{ HorizontalStartCapMiddle().bottomRight.x + FillRoundedDimension(), HorizontalStartCapMiddle().bottomRight.y }
							};
						}

						static constexpr pixel_point_t HorizontalReverseStartCapTop()
						{
							return pixel_point_t{
								ParentLayout::Width() - 1 - EffectiveCornerRadius(), EffectiveCornerRadius()
							};
						}

						static constexpr pixel_point_t HorizontalReverseStartCapBottom()
						{
							return pixel_point_t{
								ParentLayout::Width() - 1 - EffectiveCornerRadius(),
								ParentLayout::Height() - 1 - EffectiveCornerRadius()
							};
						}

						static constexpr pixel_rectangle_t HorizontalReverseMiddle()
						{
							return pixel_rectangle_t{
								{ EffectiveStrokeWidth() + Style::FillMargin, EffectiveCornerRadius() },
								{ ParentLayout::Width() - 1 - EffectiveCornerRadius(),
									ParentLayout::Height() - 1 - EffectiveCornerRadius() }
							};
						}

						static constexpr pixel_point_t VerticalStartCapLeft()
						{
							return pixel_point_t{
								EffectiveCornerRadius(), ParentLayout::Height() - 1 - EffectiveCornerRadius()
							};
						}

						static constexpr pixel_point_t VerticalTopCapLeft()
						{
							return pixel_point_t{ EffectiveCornerRadius(), EffectiveCornerRadius() };
						}

						static constexpr pixel_point_t VerticalTopCapRight()
						{
							return pixel_point_t{
								ParentLayout::Width() - 1 - EffectiveCornerRadius(), EffectiveCornerRadius()
							};
						}

						static constexpr pixel_point_t VerticalStartCapRight()
						{
							return pixel_point_t{
								ParentLayout::Width() - 1 - EffectiveCornerRadius(),
								ParentLayout::Height() - 1 - EffectiveCornerRadius()
							};
						}

						static constexpr pixel_rectangle_t VerticalMiddle()
						{
							return pixel_rectangle_t{
								{ EffectiveStrokeWidth() + Style::FillMargin, EffectiveStrokeWidth() + Style::FillMargin },
								{ ParentLayout::Width() - 1 - EffectiveStrokeWidth() - Style::FillMargin,
									ParentLayout::Height() - 1 - EffectiveStrokeWidth() - Style::FillMargin }
							};
						}
					};
				};
			}
		}
	}
}

#endif
