#ifndef _INTEGERGLASS_MODULES_LOGO_LAYOUT_h
#define _INTEGERGLASS_MODULES_LOGO_LAYOUT_h

#include "Constant.h"
#include "Assets.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{
			namespace Layout
			{
				using namespace IntegerSignal::FixedPoint::FactorScale;

				namespace Calc
				{
					template<typename ParentLayout>
					struct Scale
					{
						static constexpr uint8_t ScaleUpX()
						{
							return ParentLayout::Width() >= Dimensions::LayoutAspectRatioWidth ?
								MaxValue<int32_t>(1, ParentLayout::Width() / Dimensions::LayoutAspectRatioWidth) : 1;
						}

						static constexpr uint8_t ScaleUpY()
						{
							return ParentLayout::Height() >= Dimensions::LayoutAspectRatioHeight ?
								MaxValue<int32_t>(1, ParentLayout::Height() / Dimensions::LayoutAspectRatioHeight) : 1;
						}

						static constexpr uint8_t ScaleUp()
						{
							return MinValue<uint8_t>(ScaleUpX(), ScaleUpY());
						}

						static constexpr uint8_t ScaleDownX()
						{
							return ParentLayout::Width() >= Dimensions::LayoutAspectRatioWidth ? 1 :
								(1 + ((Dimensions::LayoutAspectRatioWidth) / ParentLayout::Width()));
						}
						static constexpr uint8_t ScaleDownY()
						{
							return ParentLayout::Height() >= Dimensions::LayoutAspectRatioHeight ? 1 :
								(1 + ((Dimensions::LayoutAspectRatioHeight) / ParentLayout::Height()));
						}

						static constexpr uint8_t ScaleDown()
						{
							return MinValue<uint8_t>(MaxValue<uint8_t>(ScaleDownX(), ScaleDownY()), Dimensions::LabelScaleUp);
						}
					};

					template<typename ParentLayout>
					struct DotMatrixLayout
					{
						static constexpr pixel_t SquareOuterSize()
						{
							return ((ParentLayout::Height() + 1) / Dimensions::RowCount);
						}

						static constexpr pixel_t SquareMargin()
						{
							return SquareOuterSize() > 1 ? 1 : 0;
						}

						static constexpr pixel_t SquareInnerSize()
						{
							return SquareOuterSize() - SquareMargin();
						}

						template<uint8_t letterIndex>
						static constexpr pixel_rectangle_t GetSquare(const uint8_t x, const uint8_t y)
						{
							return pixel_rectangle_t{
							static_cast<pixel_t>((SquareOuterSize() * (Dimensions::FontColumnCount + 1) * letterIndex) + (SquareOuterSize() * x)),
							static_cast<pixel_t>(SquareOuterSize() * y),
							static_cast<pixel_t>((SquareOuterSize() * (Dimensions::FontColumnCount + 1) * letterIndex) + (SquareOuterSize() * x) + SquareInnerSize() - 1),
							static_cast<pixel_t>(SquareOuterSize() * y + SquareInnerSize() - 1)
							};
						}
					};

					template<typename ParentLayout>
					struct ReflectionLayout
					{
						static constexpr pixel_t Margin()
						{
							return 1;
						}

						static constexpr pixel_t RelativeWidth()
						{
							return Scale16::Scale(Dimensions::LineAngleRatio, ParentLayout::Height());
						}

						static constexpr pixel_t Band1Width()
						{
							return Fraction(Dimensions::Band1WidthRatio, ParentLayout::Height());
						}

						static constexpr pixel_t Band2Width()
						{
							return Fraction(Dimensions::Band2WidthRatio, ParentLayout::Height());
						}

						static constexpr pixel_t BandSpace()
						{
							return Fraction(Dimensions::BandSpaceRatio, Band1Width());
						}

						static constexpr pixel_t Width()
						{
							return RelativeWidth() + Band1Width() + BandSpace() + Band2Width();
						}

						static constexpr pixel_t Height()
						{
							return ParentLayout::Height();
						}

						static constexpr pixel_t Band1StartX()
						{
							return 0;
						}

						static constexpr pixel_t Band2StartX()
						{
							return Band1Width() + BandSpace();
						}

						static constexpr pixel_triangle_t Band1TriangleBottom()
						{
							return pixel_triangle_t
							{
								pixel_point_t{ Band1StartX(), ParentLayout::Height() - Margin()},
								pixel_point_t{ Band1StartX() + Band1Width(), ParentLayout::Height() - Margin()},
								pixel_point_t{ Band1StartX() + RelativeWidth(), Margin()}
							};
						}
						static constexpr pixel_triangle_t Band1TriangleTop()
						{
							return pixel_triangle_t
							{
								pixel_point_t{ Band1StartX() + RelativeWidth(),  Margin()},
								pixel_point_t{ Band1StartX() + RelativeWidth() + Band1Width(),  Margin()},
								pixel_point_t{ Band1StartX() + Band1Width(), ParentLayout::Height() - Margin()}
							};
						}

						static constexpr pixel_triangle_t Band2TriangleBottom()
						{
							return pixel_triangle_t
							{
								pixel_point_t{ Band2StartX(), ParentLayout::Height() - Margin()},
								pixel_point_t{ Band2StartX() + Band2Width(), ParentLayout::Height() - Margin()},
								pixel_point_t{ Band2StartX() + RelativeWidth(), Margin()}
							};
						}
						static constexpr pixel_triangle_t Band2TriangleTop()
						{
							return pixel_triangle_t
							{
								pixel_point_t{ Band2StartX() + RelativeWidth(), Margin()},
								pixel_point_t{ Band2StartX() + RelativeWidth() + Band2Width(),  Margin()},
								pixel_point_t{ Band2StartX() + Band2Width(), ParentLayout::Height() - Margin()}
							};
						}
					};

					template<typename ParentLayout>
					struct GlassEdgesLayout
					{
						using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

						static constexpr dimension_t X()
						{
							return ParentLayout::X();
						}

						static constexpr dimension_t Y()
						{
							return ParentLayout::Y();
						}
						
						static constexpr dimension_t Width()
						{
							return ParentLayout::Width();
						}

						static constexpr dimension_t Height()
						{
							return ParentLayout::Height();
						}

						static constexpr dimension_t CornerRadius()
						{
							return Fraction(Dimensions::CornerRadiusRatio, ParentLayout::Width());
						}

						static constexpr pixel_line_t LineTop()
						{
							return pixel_line_t{
								{ CornerRadius() + 1,  0},
								{ ParentLayout::Width() - 1 - CornerRadius(),  0}
							};
						}

						static constexpr pixel_line_t LineLeft()
						{
							return pixel_line_t{
								{ 0,  CornerRadius() + 1},
								{ 0,  ParentLayout::Height() - 1 - CornerRadius()}
							};
						}

						static constexpr pixel_line_t LineBottom()
						{
							return pixel_line_t{
								{ CornerRadius() + 1,  ParentLayout::Height() - 1},
								{ ParentLayout::Width() - 1 - CornerRadius(),  ParentLayout::Height() - 1}
							};
						}

						static constexpr pixel_line_t LineRight()
						{
							return pixel_line_t{
								{ ParentLayout::Width() - 1,  CornerRadius() + 1},
								{ ParentLayout::Width() - 1,  ParentLayout::Height() - 1 - CornerRadius()}
							};
						}

						using ArcTopLeft = Framework::TemplateLayout<
							0,
							0,
							CornerRadius() * 2,
							CornerRadius() * 2
						>;

						using ArcTopRight = Framework::TemplateLayout<
							ParentLayout::Width() - 1 - CornerRadius(),
							0,
							CornerRadius() * 2,
							CornerRadius() * 2
						>;

						using ArcBottomLeft = Framework::TemplateLayout<
							0,
							ParentLayout::Height() - 1 - CornerRadius(),
							CornerRadius() * 2,
							CornerRadius() * 2
						>;

						using ArcBottomRight = Framework::TemplateLayout<
							ParentLayout::Width() - 1 - CornerRadius(),
							ParentLayout::Height() - 1 - CornerRadius(),
							CornerRadius() * 2,
							CornerRadius() * 2
						>;
					};

					template<typename ParentLayout>
					struct View
					{
						static constexpr uint8_t ScaleUp = Scale<ParentLayout>::ScaleUp();
						static constexpr uint8_t ScaleDown = Scale<ParentLayout>::ScaleDown();

						static constexpr pixel_t ViewWidth()
						{
							return (static_cast<int32_t>(Dimensions::LayoutAspectRatioWidth) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t ViewHeight()
						{
							return (static_cast<int32_t>(Dimensions::LayoutAspectRatioHeight) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t LabelWidth()
						{
							return (static_cast<int32_t>(Dimensions::LabelRatioWidth) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t LabelHeight()
						{
							return (static_cast<int32_t>(Dimensions::LabelRatioHeight) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t LabelMargin()
						{
							return (static_cast<int32_t>(Dimensions::LabelRatioMargin) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t GlassWidth()
						{
							return (static_cast<int32_t>(Dimensions::GlassAspectRatioWidth) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t GlassHeight()
						{
							return (static_cast<int32_t>(Dimensions::GlassAspectRatioHeight) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t TextWidth()
						{
							return (static_cast<int32_t>(Dimensions::TextAspectRatioWidth) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t TextHeight()
						{
							return (static_cast<int32_t>(Dimensions::TextAspectRatioHeight) * ScaleUp) / ScaleDown;
						}

						static constexpr pixel_t X()
						{
							return ParentLayout::X();
						}

						static constexpr pixel_t Y()
						{
							return ParentLayout::Y();
						}

						static constexpr pixel_t Width()
						{
							return ViewWidth();
						}

						static constexpr pixel_t Height()
						{
							return ViewHeight();
						}

						using Label = Framework::TemplateLayout<0, 0, LabelWidth(), LabelHeight()>;

						using GlassCandidate = Framework::TemplateLayout<0, 0, GlassWidth(), GlassHeight()>;

						using TextCandidate = Framework::TemplateLayout<0, 0, TextWidth(), TextHeight()>;

						using Text = Framework::TemplateLayout<0, 0,
							DotMatrixLayout<TextCandidate>::SquareOuterSize()* Dimensions::ColumnCount,
							DotMatrixLayout<TextCandidate>::SquareOuterSize()* Dimensions::RowCount
						>;

						using Glass = Framework::TemplateLayout<0, 0,
							Text::Width() + GlassCandidate::Height() - Text::Height(),
							GlassCandidate::Height()
						>;
					};

					template<typename ParentLayout>
					using ViewLayout = Framework::Layout::Align<ParentLayout,
						Calc::View<ParentLayout>,
						Framework::Layout::AlignmentEnum::MiddleCenter
					>;
				}

				template<typename ParentLayout>
				using Label = Framework::Layout::Align<Calc::ViewLayout<ParentLayout>,
					typename Calc::View<ParentLayout>::Label,
					Framework::Layout::AlignmentEnum::TopCenter
				>;

				template<typename ParentLayout>
				using Glass = Framework::Layout::Translate<
					Framework::Layout::Align<Calc::ViewLayout<ParentLayout>,
					typename Calc::View<ParentLayout>::Glass,
					Framework::Layout::AlignmentEnum::TopCenter>,
					0, Calc::View<ParentLayout>::LabelHeight() + Calc::View<ParentLayout>::LabelMargin()
				>;

				template<typename ParentLayout>
				using Text = Framework::Layout::Align<Glass<ParentLayout>,
					typename Calc::View<ParentLayout>::Text,
					Framework::Layout::AlignmentEnum::MiddleCenter
				>;

				template<typename ParentLayout>
				using Reflection = Glass<ParentLayout>;
			}
		}
	}
}
#endif