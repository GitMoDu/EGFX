#ifndef _EGFX_MODULES_BATTERY_INDICATOR_LAYOUT_h
#define _EGFX_MODULES_BATTERY_INDICATOR_LAYOUT_h

#include "Constant.h"

namespace Egfx
{
	namespace Modules
	{
		namespace BatteryIndicator
		{
			namespace Layout
			{
				using namespace IntegerSignal::FixedPoint::FactorScale;

				namespace Orient
				{
					template<typename ParentLayout>
					static constexpr pixel_rectangle_t AsRect(const pixel_rectangle_t r)
					{
						return r;
					}

					template<typename ParentLayout>
					static constexpr pixel_rectangle_t TranslateToParent(const pixel_rectangle_t r)
					{
						return pixel_rectangle_t{
							pixel_point_t{
								static_cast<pixel_t>(ParentLayout::X() + r.topLeft.x),
								static_cast<pixel_t>(ParentLayout::Y() + r.topLeft.y)
							},
							pixel_point_t{
								static_cast<pixel_t>(ParentLayout::X() + r.bottomRight.x),
								static_cast<pixel_t>(ParentLayout::Y() + r.bottomRight.y)
							}
						};
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_t LocalWidth()
					{
						return ((Orientation == OrientationEnum::TopToBottom) || (Orientation == OrientationEnum::BottomToTop))
							? ParentLayout::Height()
							: ParentLayout::Width();
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_t LocalHeight()
					{
						return ((Orientation == OrientationEnum::TopToBottom) || (Orientation == OrientationEnum::BottomToTop))
							? ParentLayout::Width()
							: ParentLayout::Height();
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_rectangle_t FlipX(const pixel_rectangle_t r)
					{
						return pixel_rectangle_t{
							pixel_point_t{
								static_cast<pixel_t>(LocalWidth<ParentLayout, Orientation>() - 1 - r.bottomRight.x),
								r.topLeft.y
							},
							pixel_point_t{
								static_cast<pixel_t>(LocalWidth<ParentLayout, Orientation>() - 1 - r.topLeft.x),
								r.bottomRight.y
							}
						};
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_rectangle_t FlipY(const pixel_rectangle_t r)
					{
						return pixel_rectangle_t{
							pixel_point_t{
								r.topLeft.x,
								static_cast<pixel_t>(LocalHeight<ParentLayout, Orientation>() - 1 - r.bottomRight.y)
							},
							pixel_point_t{
								r.bottomRight.x,
								static_cast<pixel_t>(LocalHeight<ParentLayout, Orientation>() - 1 - r.topLeft.y)
							}
						};
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_rectangle_t Rotate90CW(const pixel_rectangle_t r)
					{
						return pixel_rectangle_t{
							pixel_point_t{
								r.topLeft.y,
								static_cast<pixel_t>(LocalWidth<ParentLayout, Orientation>() - 1 - r.bottomRight.x)
							},
							pixel_point_t{
								r.bottomRight.y,
								static_cast<pixel_t>(LocalWidth<ParentLayout, Orientation>() - 1 - r.topLeft.x)
							}
						};
					}

					template<typename ParentLayout, OrientationEnum Orientation>
					struct ApplyImpl;

					template<typename ParentLayout>
					struct ApplyImpl<ParentLayout, OrientationEnum::LeftToRight>
					{
						static constexpr pixel_rectangle_t Apply(const pixel_rectangle_t r)
						{
							return r;
						}
					};

					template<typename ParentLayout>
					struct ApplyImpl<ParentLayout, OrientationEnum::RightToLeft>
					{
						static constexpr pixel_rectangle_t Apply(const pixel_rectangle_t r)
						{
							return FlipX<ParentLayout, OrientationEnum::RightToLeft>(r);
						}
					};

					template<typename ParentLayout>
					struct ApplyImpl<ParentLayout, OrientationEnum::TopToBottom>
					{
						static constexpr pixel_rectangle_t Apply(const pixel_rectangle_t r)
						{
							return Rotate90CW<ParentLayout, OrientationEnum::TopToBottom>(
								FlipX<ParentLayout, OrientationEnum::TopToBottom>(r));
						}
					};

					template<typename ParentLayout>
					struct ApplyImpl<ParentLayout, OrientationEnum::BottomToTop>
					{
						static constexpr pixel_rectangle_t Apply(const pixel_rectangle_t r)
						{
							return Rotate90CW<ParentLayout, OrientationEnum::TopToBottom>(r);
						}
					};

					template<typename ParentLayout, OrientationEnum Orientation>
					static constexpr pixel_rectangle_t Apply(const pixel_rectangle_t r)
					{
						return TranslateToParent<ParentLayout>(ApplyImpl<ParentLayout, Orientation>::Apply(r));
					}
				}

				template<typename ParentLayout,
					typename Style = TemplateBatteryStyle<>
				>
				struct Battery
				{
					static constexpr bool IsVertical()
					{
						return (Style::Orientation == OrientationEnum::TopToBottom) || (Style::Orientation == OrientationEnum::BottomToTop);
					}

					static constexpr pixel_t EffectiveWidth()
					{
						return IsVertical() ? ParentLayout::Height() : ParentLayout::Width();
					}

					static constexpr pixel_t EffectiveHeight()
					{
						return IsVertical() ? ParentLayout::Width() : ParentLayout::Height();
					}

					static constexpr pixel_t StrokeWidth()
					{
						return MaxValue<pixel_t>(0, Fraction16::Fraction<pixel_t>(Style::Stroke, EffectiveHeight() / 2));
					}

					static constexpr pixel_t HeadHeight()
					{
						return MaxValue<pixel_t>(0, Fraction16::Fraction<pixel_t>(Style::HeadHeight, EffectiveHeight() / 2));
					}

					static constexpr pixel_t HeadWidth()
					{
						return MaxValue<pixel_t>(0, Fraction16::Fraction(Style::HeadWidth, EffectiveHeight()));
					}

					static constexpr pixel_t HeadMarginTop()
					{
						return MaxValue<pixel_t>(0, (EffectiveHeight() - HeadHeight()) / 2);
					}

				private:
					static constexpr pixel_rectangle_t HeadTopCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth()), HeadMarginTop() },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - 1), static_cast<pixel_t>(HeadMarginTop() + StrokeWidth() - 1) }
						};
					}

					static constexpr pixel_rectangle_t HeadBottomCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth()), static_cast<pixel_t>(EffectiveHeight() - 1 - HeadMarginTop()) },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - 1), static_cast<pixel_t>(EffectiveHeight() - StrokeWidth() - HeadMarginTop()) }
						};
					}

					static constexpr pixel_rectangle_t HeadRightCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - StrokeWidth()), static_cast<pixel_t>(HeadMarginTop() + StrokeWidth()) },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - 1), static_cast<pixel_t>(EffectiveHeight() - 1 - HeadMarginTop() - StrokeWidth()) }
						};
					}

					static constexpr pixel_rectangle_t HeadLeftTopCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth()), StrokeWidth() },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth() + StrokeWidth() - 1), HeadMarginTop() }
						};
					}

					static constexpr pixel_rectangle_t HeadLeftBottomCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth()), static_cast<pixel_t>(EffectiveHeight() - 1 - StrokeWidth()) },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth() + StrokeWidth() - 1), static_cast<pixel_t>(EffectiveHeight() - 1 - HeadMarginTop()) }
						};
					}

					static constexpr pixel_rectangle_t OutsideLeftCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ 0, StrokeWidth() },
							pixel_point_t{ static_cast<pixel_t>(StrokeWidth() - 1), static_cast<pixel_t>(EffectiveHeight() - StrokeWidth()) }
						};
					}

					static constexpr pixel_rectangle_t OutsideTopCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ 0, 0 },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth() + StrokeWidth() - 1), static_cast<pixel_t>(StrokeWidth() - 1) }
						};
					}

					static constexpr pixel_rectangle_t OutsideBottomCanonical()
					{
						return pixel_rectangle_t{
							pixel_point_t{ 0, static_cast<pixel_t>(EffectiveHeight() - StrokeWidth()) },
							pixel_point_t{ static_cast<pixel_t>(EffectiveWidth() - HeadWidth() + StrokeWidth() - 1), static_cast<pixel_t>(EffectiveHeight() - 1) }
						};
					}

				public:
					static constexpr pixel_rectangle_t HeadTop()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(HeadTopCanonical());
					}

					static constexpr pixel_rectangle_t HeadBottom()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(HeadBottomCanonical());
					}

					static constexpr pixel_rectangle_t HeadRight()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(HeadRightCanonical());
					}

					static constexpr pixel_rectangle_t HeadLeftTop()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(HeadLeftTopCanonical());
					}

					static constexpr pixel_rectangle_t HeadLeftBottom()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(HeadLeftBottomCanonical());
					}

					static constexpr pixel_rectangle_t OutsideLeft()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(OutsideLeftCanonical());
					}

					static constexpr pixel_rectangle_t OutsideTop()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(OutsideTopCanonical());
					}

					static constexpr pixel_rectangle_t OutsideBottom()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(OutsideBottomCanonical());
					}

					static constexpr pixel_rectangle_t HeadTopUnoriented()
					{
						return HeadTopCanonical();
					}

					static constexpr pixel_rectangle_t HeadBottomUnoriented()
					{
						return HeadBottomCanonical();
					}
				};

				template<typename ParentLayout,
					typename Style = TemplateBatteryStyle<>
				>
				struct ProgressBars
				{
					using Battery = typename Layout::Battery<ParentLayout, Style>;

					static constexpr pixel_t InnerMargin()
					{
						return Fraction16::Fraction<pixel_t>(Style::InnerMargin, Battery::EffectiveHeight() / 2);
					}

					using InnerLayout = Framework::TemplateLayout<
						ParentLayout::X() + Battery::StrokeWidth() + InnerMargin(),
						ParentLayout::Y() + Battery::StrokeWidth() + InnerMargin(),
						Battery::EffectiveWidth() - Battery::HeadWidth() - Battery::StrokeWidth() - InnerMargin(),
						Battery::EffectiveHeight() - (Battery::StrokeWidth() * 2) - (InnerMargin() * 2)
					>;

					static constexpr pixel_t BarWidth()
					{
						return (Battery::EffectiveWidth() - (Battery::StrokeWidth() * 2) - (InnerMargin() * 2)) / Style::BarCount;
					}

					static constexpr pixel_t BarRemainder()
					{
						return MaxValue<pixel_t>(0, InnerLayout::Width() - (BarWidth() * Style::BarCount) - InnerMargin());
					}
					static constexpr pixel_rectangle_t BarN(const uint8_t index)
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(pixel_rectangle_t{
							{ InnerLayout::X() + (BarWidth() * index) + 1, InnerLayout::Y() },
							{ InnerLayout::X() + (BarWidth() * (index + 1)) - 1 - InnerMargin(), InnerLayout::Y() + InnerLayout::Height() - 1 }
							});
					}

					static constexpr pixel_rectangle_t BarLastBody()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(pixel_rectangle_t{
							{ InnerLayout::X() + (BarWidth() * (Style::BarCount - 1)) + 1, InnerLayout::Y() },
							{ Battery::HeadTopUnoriented().topLeft.x - 1 - InnerMargin(), InnerLayout::Y() + InnerLayout::Height() - 1 }
							});
					}

					static constexpr pixel_rectangle_t BarLastHead()
					{
						return Orient::Apply<ParentLayout, Style::Orientation>(pixel_rectangle_t{
							{ Battery::HeadTopUnoriented().topLeft.x - InnerMargin(), Battery::HeadTopUnoriented().topLeft.y + Battery::StrokeWidth() + InnerMargin() },
							{ Battery::HeadTopUnoriented().bottomRight.x - 1 - Battery::StrokeWidth() - InnerMargin(), Battery::HeadBottomUnoriented().topLeft.y - Battery::StrokeWidth() - InnerMargin() }
							});
					}

					static constexpr pixel_t X() { return ParentLayout::X(); }
					static constexpr pixel_t Y() { return ParentLayout::Y(); }
					static constexpr pixel_t Width() { return Battery::EffectiveWidth() - (Battery::StrokeWidth() * 2) - (InnerMargin() * 2); }
					static constexpr pixel_t Height() { return Battery::EffectiveHeight() - (Battery::StrokeWidth() * 2) - (InnerMargin() * 2); }
				};
			}
		}
	}
}
#endif