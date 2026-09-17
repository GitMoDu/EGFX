#ifndef _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_DRAWABLE_h
#define _EGFX_MODULES_PROGRESS_INDICATOR_LINEAR_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"
#include "../../../Modules/Kit/Include.h"

namespace Egfx
{
	namespace Modules
	{
		namespace ProgressIndicator
		{
			namespace Drawable
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename Style,
					typename PixelShaderType
				>
				class IndicatorRectangle : public Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>;
					using LayoutCalc = Layout::Linear<ParentLayout, Style>;
					using LayoutFill = typename LayoutCalc::RectangleFill;

				private:
					dimension_t FilledDimension = 0;

				public:
					IndicatorRectangle() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}

					~IndicatorRectangle() = default;

					void SetFilledDimension(const dimension_t filledDimension)
					{
						FilledDimension = LimitValue<dimension_t, 0, LayoutFill::RangeLength()>(filledDimension);
					}

					dimension_t GetFilledDimension() const
					{
						return FilledDimension;
					}

					void Draw(IFrameBuffer* frame)
					{
						if (!LayoutFill::HasFill() || FilledDimension < 1)
							return;

						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						if (LayoutCalc::IsVertical())
						{
							if (LayoutCalc::IsReversed())
							{
								Base::RectangleFill(frame, LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().topLeft.y + FilledDimension);
							}
							else
							{
								Base::RectangleFill(frame, LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().bottomRight.y - FilledDimension,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().bottomRight.y);
							}
						}
						else
						{
							if (LayoutCalc::IsReversed())
							{
								Base::RectangleFill(frame, LayoutFill::Bounds().bottomRight.x - FilledDimension, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().bottomRight.y);
							}
							else
							{
								Base::RectangleFill(frame, LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().topLeft.x + FilledDimension, LayoutFill::Bounds().bottomRight.y);
							}
						}
					}
				};

				/// <summary>
				/// Dynamic fill for a rounded rectangle. The fill is drawn in the center of the rectangle, with the fill area being limited by the corner radius and outline width.
				/// TODO: Correct ArcBoundaryModeEnum to ExcludeStartAndEnd after CircleQuadrantFill has been fixed.
				/// </summary>
				template<typename dimension_t, typename ParentLayout,
					typename Style,
					typename PixelShaderType
				>
				class IndicatorRoundedRectangle : public Framework::Shader::Geometry::CircleRingShader<dimension_t,
					Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>
				{
				private:
					using Base = Framework::Shader::Geometry::CircleRingShader<dimension_t,
						Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;

					using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
					using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
					using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

					using LayoutCalc = Layout::Linear<ParentLayout, Style>;
					using LayoutFill = typename LayoutCalc::RoundedRectangleFill;

				private:
					dimension_t FilledDimension = 0;

				public:
					IndicatorRoundedRectangle() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height()) {}

					~IndicatorRoundedRectangle() = default;

					void SetFilledDimension(const dimension_t filledDimension)
					{
						FilledDimension = LimitValue<dimension_t, 0, LayoutFill::RangeLength()>(filledDimension);
					}

					dimension_t GetFilledDimension() const
					{
						return FilledDimension;
					}

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());

						if (LayoutCalc::IsVertical())
						{
							if (LayoutCalc::IsReversed())
							{
								const RingBoundaryModeEnum ringBoundaryMode = (Style::FillMargin > 0) ? RingBoundaryModeEnum::ExcludeInner : RingBoundaryModeEnum::ExcludeInnerAndOuter;
								const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::ExcludeStartAndEnd;
								const pixel_t startY = LayoutFill::VerticalStartCapLeft().y;
								const pixel_t endY = static_cast<pixel_t>(startY - FilledDimension);

								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalStartCapLeft().x, startY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalStartCapRight().x, startY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
									arcBoundaryMode, ringBoundaryMode);
								if (LayoutFill::HasFill())
									Base::RectangleFill(frame,
										LayoutFill::VerticalMiddle().topLeft.x, endY,
										LayoutFill::VerticalMiddle().bottomRight.x, startY);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalTopCapLeft().x, endY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalTopCapRight().x, endY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
									arcBoundaryMode, ringBoundaryMode);
							}
							else
							{
								const RingBoundaryModeEnum ringBoundaryMode = (Style::FillMargin > 0) ? RingBoundaryModeEnum::ExcludeInner : RingBoundaryModeEnum::ExcludeInnerAndOuter;
								const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::ExcludeStartAndEnd;
								const pixel_t startY = LayoutFill::VerticalTopCapLeft().y;
								const pixel_t endY = static_cast<pixel_t>(startY + FilledDimension);

								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalTopCapLeft().x, startY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalTopCapRight().x, startY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
									arcBoundaryMode, ringBoundaryMode);
								if (LayoutFill::HasFill())
									Base::RectangleFill(frame,
										LayoutFill::VerticalMiddle().topLeft.x, startY,
										LayoutFill::VerticalMiddle().bottomRight.x, endY);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalStartCapLeft().x, endY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									LayoutFill::VerticalStartCapRight().x, endY,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
									arcBoundaryMode, ringBoundaryMode);
							}
						}
						else
						{
							if (LayoutCalc::IsReversed())
							{
								const RingBoundaryModeEnum ringBoundaryMode = (Style::FillMargin > 0) ? RingBoundaryModeEnum::ExcludeInner : RingBoundaryModeEnum::ExcludeInnerAndOuter;
								const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::ExcludeStartAndEnd;
								const pixel_t startX = LayoutFill::HorizontalReverseStartCapTop().x;
								const pixel_t endX = static_cast<pixel_t>(startX - FilledDimension);

								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalReverseStartCapTop().x, LayoutFill::HorizontalReverseStartCapTop().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalReverseStartCapBottom().x, LayoutFill::HorizontalReverseStartCapBottom().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
									arcBoundaryMode, ringBoundaryMode);

								if (LayoutFill::HasFill())
									Base::RectangleFill(frame,
										endX, LayoutFill::HorizontalReverseMiddle().topLeft.y,
										startX, LayoutFill::HorizontalReverseMiddle().bottomRight.y);

								Base::CircleQuadrantFill(frame,
									endX, LayoutFill::HorizontalReverseStartCapTop().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::CircleQuadrantFill(frame,
									endX, LayoutFill::HorizontalReverseStartCapBottom().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomLeft,
									arcBoundaryMode, ringBoundaryMode);
							}
							else
							{
								// Fill start cap.
								const RingBoundaryModeEnum ringBoundaryMode = (Style::FillMargin > 0) ? RingBoundaryModeEnum::ExcludeInner : RingBoundaryModeEnum::ExcludeInnerAndOuter;
								const ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::ExcludeStartAndEnd;

								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalStartCapTop().x, LayoutFill::HorizontalStartCapTop().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopLeft,
									arcBoundaryMode, ringBoundaryMode);
								Base::RectangleFill(frame,
									LayoutFill::HorizontalStartCapMiddle().topLeft.x, LayoutFill::HorizontalStartCapMiddle().topLeft.y,
									LayoutFill::HorizontalStartCapMiddle().bottomRight.x, LayoutFill::HorizontalStartCapMiddle().bottomRight.y);
								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalStartCapBottom().x, LayoutFill::HorizontalStartCapBottom().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomLeft,
									arcBoundaryMode, ringBoundaryMode);


								// Fill in center rectangle area.
								if (LayoutFill::HasFill())
									Base::RectangleFill(frame,
										LayoutFill::RectangleFillBounds().topLeft.x, LayoutFill::RectangleFillBounds().topLeft.y,
										LayoutFill::RectangleFillBounds().topLeft.x + FilledDimension, LayoutFill::RectangleFillBounds().bottomRight.y);

								// Fill end cap.
								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalEndCapTop().x + FilledDimension, LayoutFill::HorizontalEndCapTop().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
									arcBoundaryMode, ringBoundaryMode);
								Base::RectangleFill(frame,
									LayoutFill::HorizontalEndCapMiddle().topLeft.x + FilledDimension, LayoutFill::HorizontalEndCapMiddle().topLeft.y,
									LayoutFill::HorizontalEndCapMiddle().bottomRight.x + FilledDimension, LayoutFill::HorizontalEndCapMiddle().bottomRight.y);
								Base::CircleQuadrantFill(frame,
									LayoutFill::HorizontalEndCapBottom().x + FilledDimension, LayoutFill::HorizontalEndCapBottom().y,
									LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
									arcBoundaryMode, ringBoundaryMode);

							}
						}
					}
				};


				template<typename dimension_t,
					typename ParentLayout,
					typename Style,
					typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
				>
				class TrackRectangle : public Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>;
					using LayoutCalc = Layout::Linear<ParentLayout, Style>;
					using LayoutFill = typename LayoutCalc::RectangleFill;

				private:
					dimension_t FilledDimension = 0;

				public:
					TrackRectangle() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}

					~TrackRectangle() = default;

					void SetFilledDimension(const dimension_t filledDimension)
					{
						FilledDimension = LimitValue<dimension_t, 0, LayoutFill::RangeLength()>(filledDimension);
					}

					dimension_t GetFilledDimension() const
					{
						return FilledDimension;
					}

					void Draw(IFrameBuffer* frame)
					{
						if (!LayoutFill::HasFill() || FilledDimension >= LayoutFill::RangeLength())
							return;

						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						if (LayoutCalc::IsVertical())
						{
							if (LayoutCalc::IsReversed())
							{
								Base::RectangleFill(frame,
									LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().topLeft.y + FilledDimension,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().bottomRight.y);
							}
							else
							{
								Base::RectangleFill(frame,
									LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().bottomRight.y - FilledDimension);
							}
						}
						else
						{
							if (LayoutCalc::IsReversed())
							{
								Base::RectangleFill(frame,
									LayoutFill::Bounds().topLeft.x, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().bottomRight.x - FilledDimension, LayoutFill::Bounds().bottomRight.y);
							}
							else
							{
								Base::RectangleFill(frame, LayoutFill::Bounds().topLeft.x + FilledDimension, LayoutFill::Bounds().topLeft.y,
									LayoutFill::Bounds().bottomRight.x, LayoutFill::Bounds().bottomRight.y);
							}
						}
					}
				};

				template<typename dimension_t,
					typename ParentLayout,
					typename Style,
					typename PixelShaderType
				>
				class TrackRoundedRectangle : public IndicatorRoundedRectangle<dimension_t, ParentLayout, Style, PixelShaderType>
				{
				private:
					using Base = IndicatorRoundedRectangle<dimension_t, ParentLayout, Style, PixelShaderType>;

					using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;
					using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
					using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;
					using LayoutCalc = Layout::Linear<ParentLayout, Style>;
					using LayoutFill = typename LayoutCalc::RoundedRectangleFill;

				private:
					dimension_t FilledDimension = 0;

				public:
					TrackRoundedRectangle() : Base()
					{
						Base::SetFilledDimension(LayoutFill::RangeLength());
					}

					~TrackRoundedRectangle() = default;

					void SetFilledDimension(const dimension_t filledDimension)
					{
						FilledDimension = LimitValue<dimension_t, 0, LayoutFill::RangeLength()>(filledDimension);
					}

					dimension_t GetFilledDimension() const
					{
						return FilledDimension;
					}


					void Draw(IFrameBuffer* frame)
					{
						static constexpr RingBoundaryModeEnum ringBoundaryMode = (Style::FillMargin > 0) ? RingBoundaryModeEnum::ExcludeInner : RingBoundaryModeEnum::ExcludeInnerAndOuter;
						//static constexpr RingBoundaryModeEnum ringBoundaryMode =RingBoundaryModeEnum::ExcludeInnerAndOuter;
						static constexpr ArcBoundaryModeEnum arcBoundaryMode = ArcBoundaryModeEnum::ExcludeStartAndEnd;


						Base::Prepare(ParentLayout::X(), ParentLayout::Y());

						if (LayoutCalc::IsVertical())
						{
							if (LayoutCalc::IsReversed())
							{
								//TODO: Implement after basic left-to-right fill is working correctly.
							}
							else
							{
								//TODO: Implement after basic left-to-right fill is working correctly.
							}
						}
						else
						{
							if (LayoutCalc::IsReversed())
							{
								//TODO: Implement after basic left-to-right fill is working correctly.
							}
							else
							{
								// Fill start cap.

								if (FilledDimension == 0)
								{
									Base::CircleQuadrantFill(frame,
										LayoutFill::HorizontalStartCapTop().x, LayoutFill::HorizontalStartCapTop().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopLeft,
										arcBoundaryMode, ringBoundaryMode);
									Base::RectangleFill(frame,
										LayoutFill::HorizontalStartCapMiddle().topLeft.x, LayoutFill::HorizontalStartCapMiddle().topLeft.y,
										LayoutFill::HorizontalStartCapMiddle().bottomRight.x, LayoutFill::HorizontalStartCapMiddle().bottomRight.y);
									Base::CircleQuadrantFill(frame,
										LayoutFill::HorizontalStartCapBottom().x, LayoutFill::HorizontalStartCapBottom().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomLeft,
										arcBoundaryMode, ringBoundaryMode);
								}


								// Fill end cap outside of the rectangle area to ensure that the fill is drawn outside of the end cap area.
								if (FilledDimension < static_cast<dimension_t>(LayoutFill::RangeLength() - (LayoutCalc::FillRoundedDimension() / 2)))
								{
									// Fill in center rectangle area.
									if (LayoutFill::HasFill())
										Base::RectangleFill(frame,
											LayoutFill::RectangleFillBounds().topLeft.x + FilledDimension + LayoutCalc::FillRoundedDimension(), LayoutFill::RectangleFillBounds().topLeft.y,
											LayoutFill::RectangleFillBounds().bottomRight.x, LayoutFill::RectangleFillBounds().bottomRight.y);

									// Fill outside rounded corners.
									Base::CircleQuadrantFillOutside(frame,
										LayoutFill::HorizontalEndCapTop().x + FilledDimension, LayoutFill::HorizontalEndCapTop().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
										arcBoundaryMode, RingBoundaryModeEnum::ExcludeInnerAndOuter);
									Base::CircleQuadrantFillOutside(frame,
										LayoutFill::HorizontalEndCapBottom().x + FilledDimension, LayoutFill::HorizontalEndCapBottom().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
										arcBoundaryMode, RingBoundaryModeEnum::ExcludeInnerAndOuter);

									// Fill end cap.
									Base::CircleQuadrantFill(frame,
										LayoutFill::HorizontalEndCapTop().x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapTop().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::TopRight,
										arcBoundaryMode, ringBoundaryMode);
									Base::RectangleFill(frame,
										LayoutFill::HorizontalEndCapMiddle().topLeft.x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapMiddle().topLeft.y,
										LayoutFill::HorizontalEndCapMiddle().bottomRight.x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapMiddle().bottomRight.y);
									Base::CircleQuadrantFill(frame,
										LayoutFill::HorizontalEndCapBottom().x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapBottom().y,
										LayoutCalc::FillRoundedDimension(), QuadrantEnum::BottomRight,
										arcBoundaryMode, ringBoundaryMode);
								}
								else
								{
									// Draw dynamic arc fill for the end cap when the filled dimension is greater than the fill range but less than the total range length.
									const dimension_t incursion = LayoutCalc::FillRange() - FilledDimension;
									Base::CircleQuadrantRingFill(frame,
										LayoutFill::HorizontalEndCapTop().x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapTop().y,
										LayoutCalc::FillRoundedDimension(), LayoutCalc::FillRoundedDimension() - incursion,
										QuadrantEnum::TopRight,
										arcBoundaryMode, RingBoundaryModeEnum::ExcludeInnerAndOuter);
									Base::RectangleFill(frame,
										LayoutFill::HorizontalEndCapMiddle().topLeft.x + incursion, LayoutFill::HorizontalEndCapMiddle().topLeft.y,
										LayoutFill::HorizontalEndCapMiddle().bottomRight.x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapMiddle().bottomRight.y);
									Base::CircleQuadrantRingFill(frame,
										LayoutFill::HorizontalEndCapTop().x + LayoutCalc::FillRange(), LayoutFill::HorizontalEndCapBottom().y,
										LayoutCalc::FillRoundedDimension(), LayoutCalc::FillRoundedDimension() - incursion,
										QuadrantEnum::BottomRight,
										arcBoundaryMode, RingBoundaryModeEnum::ExcludeInnerAndOuter);
								}
							}
						}
					}
				};

				template<typename dimension_t>
				class EmptyProgressArc
				{
				public:
					EmptyProgressArc() {}
					~EmptyProgressArc() = default;

					static constexpr void Draw(IFrameBuffer* /*frame*/) {}
					static constexpr void SetFilledDimension(const dimension_t /*filledDimension*/) {}
					static constexpr void SetBounds(const dimension_t /*left*/, const dimension_t /*top*/,
						const dimension_t /*right*/, const dimension_t /*bottom*/) {}
					static constexpr void SetTranslation(const int16_t /*x*/, const int16_t /*y*/) {}
				};


				template<typename dimension_t,
					typename ParentLayout,
					typename Style = TemplateLinearProgressStyle<>,
					typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
				>
				using BorderRectRounded = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
					Modules::Kit::Drawable::Shape::RoundedRectangle<dimension_t, ParentLayout,
					Style::CornerRadius, Layout::Linear<ParentLayout, Style>::EffectiveStrokeWidth(), PixelShaderType>,
					Modules::Kit::Drawable::Shape::Rectangle<dimension_t, ParentLayout,
					Layout::Linear<ParentLayout, Style>::EffectiveStrokeWidth(), PixelShaderType>,
					Layout::Linear<ParentLayout, Style>::HasRoundedCorners()
				>::type;


				template<typename dimension_t, typename ParentLayout,
					typename Style = TemplateLinearProgressStyle<>,
					typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
				>
				using Border = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
					BorderRectRounded<dimension_t, ParentLayout, Style, PixelShaderType>,
					Framework::Drawable::MockDrawable<dimension_t, PixelShaderType>,
					Layout::Linear<ParentLayout, Style>::HasOutline()
				>::type;

				template<typename dimension_t, typename ParentLayout,
					typename Style = TemplateLinearProgressStyle<>,
					typename IndicatorPixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>,
					typename BorderPixelShaderType = IndicatorPixelShaderType
				>
				using Indicator = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
					IndicatorRoundedRectangle<dimension_t, ParentLayout, Style, BorderPixelShaderType>,
					IndicatorRectangle<dimension_t, ParentLayout, Style, IndicatorPixelShaderType>,
					Layout::Linear<ParentLayout, Style>::HasRoundedCorners()
				>::type;

				template<typename dimension_t, typename ParentLayout,
					typename Style = TemplateLinearProgressStyle<>,
					typename PixelShaderType = Framework::Shader::Pixel::MockShader<dimension_t>
				>
				using Track = typename TypeTraits::TypeConditional::conditional_type<
					EmptyProgressArc<dimension_t>,
					typename TypeTraits::TypeConditional::conditional_type<
					TrackRoundedRectangle<dimension_t, ParentLayout, Style, PixelShaderType>,
					TrackRectangle<dimension_t, ParentLayout, Style, PixelShaderType>,
					Layout::Linear<ParentLayout, Style>::HasRoundedCorners()
					>::type,
					TypeTraits::TypeDispatch::is_same<PixelShaderType, Framework::Shader::Pixel::MockShader<dimension_t>>::value
				>::type;
			}
		}
	}
}

#endif
