#ifndef _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_CIRCULAR_DRAWABLE_h
#define _INTEGERGLASS_MODULES_PROGRESS_INDICATOR_CIRCULAR_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"
#include "../../../Modules/Kit/Include.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Circular
			{
				namespace Drawable
				{
					namespace Shared
					{
						template<typename dimension_t>
						class EmptyProgressArc
						{
						public:
							EmptyProgressArc() {}
							~EmptyProgressArc() = default;

							static constexpr bool IsVisible() { return false; }
							static constexpr void Draw(IFrameBuffer* /*frame*/) {}
							static constexpr void SetFilledAngleSweep(const angle_t /*angleSweep*/) {}
							static constexpr void SetBounds(const dimension_t /*left*/, const dimension_t /*top*/,
								const dimension_t /*right*/, const dimension_t /*bottom*/) {}
							static constexpr void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
						};
					}

					namespace Continuous
					{
						template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
							typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>>
							class Border : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
							>
						{
						private:
							using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
								Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;

							using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
							using LayoutCalc = Layout::Calc<ParentLayout, Style>;

						public:
							Border() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
							~Border() = default;

							void Draw(IFrameBuffer* frame)
							{
								Base::Prepare(ParentLayout::X(), ParentLayout::Y());

								if (LayoutCalc::DrawOuter())
								{
									Base::CircleRingFill(frame, LayoutCalc::CenterX(), LayoutCalc::CenterY(), LayoutCalc::Radius(), LayoutCalc::OutlineInnerRadius(), RingBoundaryModeEnum::ExcludeOuter);
								}

								if (LayoutCalc::DrawInner())
								{
									Base::CircleRingFill(frame, LayoutCalc::CenterX(), LayoutCalc::CenterY(), LayoutCalc::InnerLineOuterRadius(), LayoutCalc::InnerRadius(), RingBoundaryModeEnum::ExcludeInner);
								}
							}
						};

						template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
							typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>>
							class Indicator : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
							>
						{
						private:
							using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
								Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;
							using LayoutCalc = Layout::Calc<ParentLayout, Style>;
							using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
							using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
							using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

						private:
							static constexpr bool DrawFill = LayoutCalc::FillRadius() > 0;

						private:
							angle_t EndAngle = Style::StartAngle;

						public:
							Indicator() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
							~Indicator() = default;

							void SetProgress(const ufraction16_t progress)
							{
								EndAngle = Style::StartAngle + UFraction16::Fraction(progress, LayoutCalc::FillAngleRange());
							}

							void SetFilledAngleSweep(const angle_t angleSweep)
							{
								EndAngle = Style::StartAngle + MinValue<angle_t>(angleSweep, LayoutCalc::FillAngleRange());
							}

							void Draw(IFrameBuffer* frame)
							{
								if (!LayoutCalc::DrawFill())
									return;

								Base::Prepare(ParentLayout::X(), ParentLayout::Y());

								Base::CircleArcFill(frame,
									LayoutCalc::CenterX(), LayoutCalc::CenterY(),
									Style::StartAngle, EndAngle,
									LayoutCalc::FillOuterRadius(), LayoutCalc::FillInnerRadius(),
									ArcBoundaryModeEnum::Inclusive, RingBoundaryModeEnum::ExcludeInnerAndOuter);
							}
						};

						template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
							typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>>
							class ProgressTrack : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
							>
						{
						private:
							using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
								Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;
							using LayoutCalc = Layout::Calc<ParentLayout, Style>;
							using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
							using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
							using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

						private:
							static constexpr bool DrawFill = LayoutCalc::FillRadius() > 0;

						private:
							angle_t EndAngle = Style::StartAngle;

						public:
							ProgressTrack() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
							~ProgressTrack() = default;

							void SetFilledAngleSweep(const angle_t angleSweep)
							{
								EndAngle = Style::StartAngle + MinValue<angle_t>(angleSweep, LayoutCalc::FillAngleRange());
							}

							void SetProgress(const ufraction16_t progress)
							{
								EndAngle = Style::StartAngle + UFraction16::Fraction(progress, LayoutCalc::FillAngleRange());
							}

							void Draw(IFrameBuffer* frame)
							{
								if (!LayoutCalc::DrawFill())
									return;

								Base::Prepare(ParentLayout::X(), ParentLayout::Y());

								Base::CircleArcFill(frame,
									LayoutCalc::CenterX(), LayoutCalc::CenterY(),
									EndAngle, Style::EndAngle,
									LayoutCalc::TrackOuterRadius(), LayoutCalc::TrackInnerRadius(),
									ArcBoundaryModeEnum::Inclusive, RingBoundaryModeEnum::ExcludeInnerAndOuter);
							}
						};
					}

					namespace Segmented
					{
						template<typename dimension_t,
							typename ParentLayout,
							typename Style = TemplateCircularStyle<>,
							typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
						>
						class Border : public Framework::Shader::Geometry::CircleArcShader<dimension_t, Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>
						{
						private:
							using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
								Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;
							using wide_signed_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

							using LayoutCalc = Layout::Calc<ParentLayout, Style>;

							struct ConnectorValues
							{
								wide_signed_t radialX;
								wide_signed_t radialY;
								wide_signed_t tangentX;
								wide_signed_t tangentY;
								wide_signed_t fillTangentX;
								wide_signed_t fillTangentY;
								angle_t angle;
								int8_t direction;
							};

						private:
							static constexpr bool DrawFill = LayoutCalc::FillRadius() > 0;

						private:
							ConnectorValues StartConnector;
							ConnectorValues EndConnector;
							const wide_signed_t ConnectorCenterX;
							const wide_signed_t ConnectorCenterY;
							const wide_signed_t ConnectorHalfWidth;
							const dimension_t ConnectorOuterRadius;
							const dimension_t ConnectorInnerRadius;

						public:
							Border() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()),
								StartConnector(CalculateConnectorValues(Style::StartAngle, 1)),
								EndConnector(CalculateConnectorValues(Style::EndAngle, -1)),
								ConnectorCenterX(ParentLayout::Width() / 2),
								ConnectorCenterY(ParentLayout::Height() / 2),
								ConnectorHalfWidth((static_cast<wide_signed_t>(Style::OutlineWidth) - 1) / 2),
								ConnectorOuterRadius(static_cast<dimension_t>(Radius() - 1)),
								ConnectorInnerRadius(static_cast<dimension_t>(InnerInnerRadius()))
							{

							}

							~Border() = default;

							//tODO: separate into 3 drawables: outer, inner, connectors.
							void Draw(IFrameBuffer* frame)
							{
								using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;

								static constexpr ArcBoundaryModeEnum outlineBoundary = LayoutCalc::DrawFlatConnectors()
									? ArcBoundaryModeEnum::ExcludeStartAndEnd
									: ArcBoundaryModeEnum::Inclusive;

								Base::Prepare(ParentLayout::X(), ParentLayout::Y());

								if (LayoutCalc::DrawOuter())
								{
									Base::CircleArcFill(frame, LayoutCalc::CenterX(), LayoutCalc::CenterY(),
										Style::StartAngle, Style::EndAngle,
										LayoutCalc::OutlineOuterRadius(), LayoutCalc::OutlineInnerRadius(),
										outlineBoundary);
								}

								if (LayoutCalc::DrawInner())
								{
									Base::CircleArcFill(frame, LayoutCalc::CenterX(), LayoutCalc::CenterY(), Style::StartAngle,
										Style::EndAngle, LayoutCalc::InnerLineOuterRadius(), LayoutCalc::InnerRadius(), ArcBoundaryModeEnum::Inclusive);
								}

								if (LayoutCalc::DrawFlatConnectors())
								{
									DrawRadialConnector(frame, StartConnector);
									DrawRadialConnector(frame, EndConnector);
								}
							}

						private:
							void DrawRadialConnector(IFrameBuffer* frame, const ConnectorValues& connector)
							{
								if (Style::OutlineWidth < 3)
								{
									const wide_signed_t innerX = ConnectorCenterX +
										(connector.radialX * (static_cast<wide_signed_t>(ConnectorInnerRadius) + 1)) / ConnectorScale();
									const wide_signed_t innerY = ConnectorCenterY +
										(connector.radialY * (static_cast<wide_signed_t>(ConnectorInnerRadius) + 1)) / ConnectorScale();
									const wide_signed_t outerX = ConnectorCenterX +
										(connector.radialX * static_cast<wide_signed_t>(ConnectorOuterRadius)) / ConnectorScale();
									const wide_signed_t outerY = ConnectorCenterY +
										(connector.radialY * static_cast<wide_signed_t>(ConnectorOuterRadius)) / ConnectorScale();
									Base::Line(frame,
										static_cast<dimension_t>(innerX), static_cast<dimension_t>(innerY),
										static_cast<dimension_t>(outerX), static_cast<dimension_t>(outerY));
								}
								else
								{
									const angle_t sweep = static_cast<angle_t>(
										(static_cast<uint32_t>(Style::OutlineWidth - 1) * ANGLE_RANGE * 333) /
										(static_cast<uint32_t>(ConnectorInnerRadius > 0 ? ConnectorInnerRadius : 1) * 6283));
									Base::CircleArcFill(frame, ConnectorCenterX, ConnectorCenterY,
										connector.direction > 0 ? connector.angle : static_cast<angle_t>(connector.angle - sweep),
										connector.direction > 0 ? static_cast<angle_t>(connector.angle + sweep) : connector.angle,
										ConnectorOuterRadius, ConnectorInnerRadius,
										Framework::Shader::Geometry::ArcBoundaryModeEnum::Inclusive);
								}
							}

						private:
							static constexpr wide_signed_t ConnectorScale()
							{
								return sizeof(dimension_t) == 1 ? 64 : 16384;
							}

							static constexpr bool HasOuterBorder()
							{
								return Style::OuterBorderStroke > 0;
							}

							static constexpr bool HasInnerBorder()
							{
								return Style::InnerBorderStroke > 0;
							}

							static constexpr bool DrawsOuter()
							{
								return HasOuterBorder();
							}

							static constexpr pixel_t ArcInset()
							{
								return static_cast<pixel_t>((DrawsOuter() ? Style::OutlineWidth : 0) + Style::FillMargin);
							}
							static constexpr pixel_t ArcOuterRadius()
							{
								return static_cast<pixel_t>(Radius() - 1 - ArcInset());
							}
							static constexpr pixel_t InnerOverlap()
							{
								return Style::FillMargin != 0 ? 1 : 0;
							}
							static constexpr pixel_t InnerOuterRadius()
							{
								return static_cast<pixel_t>(ArcOuterRadius() - Style::ArcThickness -
									Style::FillMargin + 1 - InnerOverlap());
							}
							static constexpr pixel_t InnerInnerRadius()
							{
								return static_cast<pixel_t>(InnerOuterRadius() - Style::OutlineWidth + 1);
							}

							static constexpr pixel_t Radius()
							{
								return MinValue<pixel_t>(ParentLayout::Width(), ParentLayout::Height()) / 2;
							}

							static constexpr pixel_t CenterX()
							{
								return ParentLayout::Width() / 2;
							}

							static constexpr pixel_t CenterY()
							{
								return ParentLayout::Height() / 2;
							}

							static ConnectorValues CalculateConnectorValues(const angle_t angle, const int8_t fillDirection)
							{
								const wide_signed_t tangentX = sizeof(dimension_t) == 1
									? Trigonometry::Sine8(angle + ANGLE_RANGE / 4)
									: Trigonometry::Sine16(angle + ANGLE_RANGE / 4);
								const wide_signed_t tangentY = sizeof(dimension_t) == 1
									? Trigonometry::Sine8(angle)
									: Trigonometry::Sine16(angle);

								if (sizeof(dimension_t) == 1)
								{
									return ConnectorValues{
										Trigonometry::Sine8(angle),
										-Trigonometry::Sine8(angle + ANGLE_RANGE / 4),
										tangentX,
										tangentY,
										tangentX * fillDirection,
										tangentY * fillDirection,
										angle,
										fillDirection
									};
								}

								return ConnectorValues{
									Trigonometry::Sine16(angle),
									-Trigonometry::Sine16(angle + ANGLE_RANGE / 4),
									tangentX,
									tangentY,
									tangentX * fillDirection,
									tangentY * fillDirection,
									angle,
									fillDirection
								};
							}
						};

						template<typename dimension_t,
							typename ParentLayout,
							typename Style = TemplateCircularStyle<>,
							typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
						>
						class Indicator : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
						>
						{
						private:
							using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
								Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;

							using LayoutCalc = Layout::Calc<ParentLayout, Style>;

						private:
							angle_t EndAngle = Style::StartAngle;

						public:
							Indicator() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}
							~Indicator() = default;

							void SetProgress(const ufraction16_t progress)
							{
								EndAngle = Style::StartAngle + UFraction16::Fraction(progress, LayoutCalc::FillAngleRange());
							}

							void Draw(IFrameBuffer* frame)
							{
								if (!LayoutCalc::Drawfill())
									return;

								Base::Prepare(ParentLayout::X(), ParentLayout::Y());
								Base::CircleArcFill(frame,
									LayoutCalc::CenterX(), LayoutCalc::CenterY(),
									Style::StartAngle, EndAngle,
									LayoutCalc::FillOuterRadius(), LayoutCalc::FillInnerRadius());
							}
						};
					}


					template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					using CircularIndicator = typename TypeTraits::TypeConditional::conditional_type<
						Continuous::Indicator<dimension_t, ParentLayout, Style, PixelShaderType>,
						Segmented::Indicator<dimension_t, ParentLayout, Style, PixelShaderType>,
						angle_t(Style::EndAngle - Style::StartAngle) == ANGLE_RANGE>::type;

					template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					using CircularBorder = typename TypeTraits::TypeConditional::conditional_type<
						Continuous::Border<dimension_t, ParentLayout, Style, PixelShaderType>,
						Segmented::Border<dimension_t, ParentLayout, Style, PixelShaderType>,
						angle_t(Style::EndAngle - Style::StartAngle) == ANGLE_RANGE>::type;


					template<typename dimension_t, typename ParentLayout, typename Style = TemplateCircularStyle<>,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					using CircularTrack = typename TypeTraits::TypeConditional::conditional_type<
						Shared::EmptyProgressArc<dimension_t>,
						Continuous::ProgressTrack<dimension_t, ParentLayout, Style, PixelShaderType>,
						TypeTraits::TypeDispatch::is_same<PixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value
					>::type;
				}
			}
		}
	}
}
#endif