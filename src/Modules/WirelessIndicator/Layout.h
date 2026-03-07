#ifndef _EGFX_MODULES_WIRELESS_INDICATOR_LAYOUT_h
#define _EGFX_MODULES_WIRELESS_INDICATOR_LAYOUT_h

#include "Constant.h"
#include "Selector.h"
#include "Assets.h"

namespace Egfx
{
	namespace Modules
	{
		namespace WirelessIndicator
		{
			namespace Layout
			{
				template<typename ParentLayout,
					int16_t BitmaskWidth>
				static constexpr uint8_t GetScaleX()
				{
					return MaxValue(1, ParentLayout::Width() / BitmaskWidth);
				}

				template<typename ParentLayout,
					int16_t BitmaskHeight>
				static constexpr uint8_t GetScaleY()
				{
					return MaxValue(1, ParentLayout::Height() / BitmaskHeight);
				}

				template<typename ParentLayout>
				using Bars8x7 = Framework::Layout::ConstrainedDimensions<ParentLayout, 7, 8, 7, 7>;

				template<typename ParentLayout>
				using Bars8x8 = Framework::Layout::ConstrainedDimensions<ParentLayout, 7, 8, 8, 8>;

				template<typename ParentLayout>
				using Bars10x7 = Framework::Layout::ConstrainedDimensions<ParentLayout, 9, 10, 7, 7>;

				template<typename ParentLayout>
				using Bars12x9 = Framework::Layout::ConstrainedDimensions<ParentLayout, 10, 12, 9, 9>;

				template<typename ParentLayout>
				using Bars22x16Constrained = Framework::Layout::ConstrainedDimensions<ParentLayout, 19, 22, 16, 16>;

				template<typename ParentLayout>
				using Bars22x16 = Framework::TemplateLayout <
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					MinValue<int16_t>(16, ParentLayout::Height())* GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars24x16Constrained = Framework::Layout::ConstrainedDimensions<ParentLayout, 19, 24, 16, 16>;

				template<typename ParentLayout>
				using Bars24x16 = Framework::TemplateLayout <
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					MinValue<int16_t>(16, ParentLayout::Height())* GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()
				>;
				template<typename ParentLayout>
				using Bars42x30Constrained = Framework::Layout::ConstrainedDimensions<ParentLayout, 13, 42, 15, 30>;

				template<typename ParentLayout>
				using Bars42x30 = Framework::TemplateLayout <
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					MinValue<int16_t>(30, ParentLayout::Height())* GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()
				>;

				template<typename ParentLayout>
				struct Primitives
				{
					static constexpr int16_t BarOffsetX(const uint8_t barIndex)
					{
						return (Dimensions::BarCount - 1) - barIndex;
					}

					static constexpr int16_t BarStartY(const uint8_t barIndex)
					{
						return ParentLayout::Y()
							+ ParentLayout::Height()
							- 1
							- (Dimensions::PrimitiveBarHeight * barIndex);
					}

					static constexpr int16_t BarStartX(const uint8_t barIndex)
					{
						return ParentLayout::X() + BarOffsetX(barIndex);
					}

					static constexpr int16_t BarEndY(const uint8_t barIndex)
					{
						return BarStartY(barIndex) + Dimensions::PrimitiveBarHeight - 1;
					}

					static constexpr int16_t BarEndX(const uint8_t barIndex)
					{
						return ParentLayout::X() + ParentLayout::Width() - 1 - ((Dimensions::BarCount - 1) - barIndex);
					}
				};

				template<typename ParentLayout>
				using Bars22x16Slice3 = Framework::TemplateLayout<
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					Assets::Bitmask22x16::Slice3Height* GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars22x16Slice2 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars22x16Slice3<ParentLayout>::Y() + (Assets::Bitmask22x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask22x16::Slice2Height* GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars22x16Slice1 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars22x16Slice2<ParentLayout>::Y() + (Assets::Bitmask22x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask22x16::Slice1Height* GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars22x16Slice0 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars22x16Slice1<ParentLayout>::Y() + (Assets::Bitmask22x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask22x16::Slice0Height* GetScaleY<ParentLayout, Assets::Bitmask22x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars24x16Slice3 = Framework::TemplateLayout<
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					Assets::Bitmask24x16::Slice3Height* GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars24x16Slice2 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars24x16Slice3<ParentLayout>::Y() + (Assets::Bitmask24x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask24x16::Slice2Height* GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars24x16Slice1 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars24x16Slice2<ParentLayout>::Y() + (Assets::Bitmask24x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask24x16::Slice1Height* GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars24x16Slice0 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars24x16Slice1<ParentLayout>::Y() + (Assets::Bitmask24x16::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask24x16::Slice0Height* GetScaleY<ParentLayout, Assets::Bitmask24x16::Height>()
				>;

				template<typename ParentLayout>
				using Bars42x30Slice3 = Framework::TemplateLayout<
					ParentLayout::X(),
					ParentLayout::Y(),
					ParentLayout::Width(),
					Assets::Bitmask42x30::Slice3Height* GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()
				>;

				template<typename ParentLayout>
				using Bars42x30Slice2 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars42x30Slice3<ParentLayout>::Y() + ((Assets::Bitmask42x30::SliceHeight + 1) * GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask42x30::Slice2Height* GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()
				>;

				template<typename ParentLayout>
				using Bars42x30Slice1 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars42x30Slice2<ParentLayout>::Y() + ((Assets::Bitmask42x30::SliceHeight + 1) * GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask42x30::Slice1Height* GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()
				>;

				template<typename ParentLayout>
				using Bars42x30Slice0 = Framework::TemplateLayout<
					ParentLayout::X(),
					Bars42x30Slice1<ParentLayout>::Y() + (Assets::Bitmask42x30::SliceHeight * GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()),
					ParentLayout::Width(),
					Assets::Bitmask42x30::Slice0Height* GetScaleY<ParentLayout, Assets::Bitmask42x30::Height>()
				>;

				template<typename ParentLayout>
				struct ParallelogramLayout
				{
					static constexpr int16_t X() { return ParentLayout::X(); }
					static constexpr int16_t Y() { return ParentLayout::Y(); }
					static constexpr int16_t Width() { return ParentLayout::Width(); }
					static constexpr int16_t Height() { return ParentLayout::Height(); }
					static constexpr int16_t Dimension() { return MinValue(Width(), Height()); }

					static constexpr int16_t Corner() { return MaxValue<int32_t>(1, Dimension() / Dimensions::ParallelogramCornerDivisor); }
					static constexpr uint8_t BackgroundLinesCount() { return MaxValue<int32_t>(1, Dimension() / Dimensions::ParallelogramBackgroundLinesDivisor); }

					static constexpr pixel_triangle_t Triangle1()
					{
						return pixel_triangle_t{
							{ 1, static_cast<pixel_t>(Height() - 1) },
							{ 1, static_cast<pixel_t>(Height() - 1 - Corner()) },
							{ static_cast<pixel_t>(Corner()), static_cast<pixel_t>(Height() - 1) }
						};
					}

					static constexpr pixel_triangle_t Triangle2()
					{
						return pixel_triangle_t{
							{ static_cast<pixel_t>(Width() - 1 - Corner()), 0  },
							{ static_cast<pixel_t>(Width() - 1), 0  },
							{ static_cast<pixel_t>(Width() - 1), Corner() }
						};
					}

					static constexpr pixel_triangle_t Triangle3()
					{
						return pixel_triangle_t{
							{ 0, static_cast<pixel_t>(Height() - 1 - Corner()) },
							{  static_cast<pixel_t>(Width() - 1 - Corner()), 0  },
							{  static_cast<pixel_t>(Width() - 1), Corner() }
						};
					}

					static constexpr pixel_triangle_t Triangle4()
					{
						return pixel_triangle_t{
							{ 0, static_cast<pixel_t>(Height() - 1 - Corner()) },
							{ Corner(), static_cast<pixel_t>(Height() - 1) },
							{ static_cast<pixel_t>(Width() - 1), Corner() }
						};
					}

					static constexpr pixel_line_t LineTop(const uint8_t index)
					{
						return pixel_line_t{
							{ 0, static_cast<pixel_t>(Height() - 2 - Corner() - index)},
							{ static_cast<pixel_t>(Width() - 1 - Corner() - index), 0  }
						};
					}

					static constexpr pixel_line_t LineBottom(const uint8_t index)
					{
						return pixel_line_t{
							{ static_cast<pixel_t>(Corner() + index + 1), static_cast<pixel_t>(Height() - 1) },
							{ static_cast<pixel_t>(Width() - 1), static_cast<pixel_t>(Corner() + index + 1) }
						};
					}
				};

				template<typename ParentLayout, bool Monochrome>
				struct LayoutSelector
				{
					using BaseSelector = Selector::BaseSelector<ParentLayout, Monochrome>;

					using PrimitiveWideOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bars8x8<ParentLayout>,
						Bars8x7<ParentLayout>,
						BaseSelector::UseWidePrimitive()>::type;

					using PrimitiveHighOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bars10x7<ParentLayout>,
						PrimitiveWideOr,
						BaseSelector::UseHighPrimitive()>::type;

					using PrimitiveTallOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bars12x9<ParentLayout>,
						PrimitiveHighOr,
						BaseSelector::UseTallPrimitive()>::type;

					using BitmaskMediumOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Bars24x16<ParentLayout>,
						Bars22x16<ParentLayout>,
						BaseSelector::UseMediumBitmask()>::type;

					using BitmaskTallOr = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						Layout::Bars42x30<ParentLayout>,
						BitmaskMediumOr,
						BaseSelector::UseTallBitmask()>::type;

					using BitmaskOrPrimitive = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						BitmaskTallOr,
						PrimitiveTallOr,
						BaseSelector::UseBitmask()>::type;

					using Bars = Framework::Layout::Align<ParentLayout, BitmaskOrPrimitive>;
				};
			}
		}
	}
}
#endif