#ifndef _EGFX_FRAMEWORK_ASSETS_DRAWABLE_SHAPE_ROUNDED_RECTANGLE_h
#define _EGFX_FRAMEWORK_ASSETS_DRAWABLE_SHAPE_ROUNDED_RECTANGLE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Drawable
			{
				namespace Shape
				{
					/// <summary>
					/// Draws a rounded rectangle outline using configurable layout and primitive shader.
					/// </summary>
					template<typename dimension_t,
						typename ParentLayout,
						dimension_t CornerRadius,
						dimension_t StrokeWidth = 1,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					class RoundedRectangle : public Framework::Shader::Geometry::CircleRingShader<dimension_t,
						Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>
					{
					public:
						using DrawableLayout = ParentLayout;

					private:
						using Base = Framework::Shader::Geometry::CircleRingShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;

						using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;
						using ArcBoundaryModeEnum = Framework::Shader::Geometry::ArcBoundaryModeEnum;
						using RingBoundaryModeEnum = Framework::Shader::Geometry::RingBoundaryModeEnum;

					private:
						static constexpr dimension_t MaximumRadius = static_cast<dimension_t>(
							MinValue(ParentLayout::Width(), ParentLayout::Height()) / 2);
						static constexpr dimension_t EffectiveCornerRadius = MinValue(CornerRadius, MaximumRadius);
						static constexpr dimension_t EffectiveStrokeWidth = MinValue(StrokeWidth, MaximumRadius);
						static constexpr dimension_t InnerCornerRadius = EffectiveCornerRadius > EffectiveStrokeWidth
							? static_cast<dimension_t>(EffectiveCornerRadius - EffectiveStrokeWidth) : 0;

						struct RoundedLayout
						{
							static constexpr pixel_line_t LineTop()
							{
								return pixel_line_t{
									{ EffectiveCornerRadius,  0},
									{ ParentLayout::Width() - 1 - EffectiveCornerRadius,  0}
								};
							}

							static constexpr pixel_line_t LineLeft()
							{
								return pixel_line_t{
									{ 0,  EffectiveCornerRadius},
									{ 0,  ParentLayout::Height() - 1 - EffectiveCornerRadius}
								};
							}

							static constexpr pixel_line_t LineBottom()
							{
								return pixel_line_t{
									{ EffectiveCornerRadius,  ParentLayout::Height() - 1},
									{ ParentLayout::Width() - 1 - EffectiveCornerRadius,  ParentLayout::Height() - 1}
								};
							}

							static constexpr pixel_line_t LineRight()
							{
								return pixel_line_t{
									{ ParentLayout::Width() - 1,  EffectiveCornerRadius},
									{ ParentLayout::Width() - 1,  ParentLayout::Height() - 1 - EffectiveCornerRadius}
								};
							}

							using ArcTopLeft = Framework::TemplateLayout<
								EffectiveCornerRadius,
								EffectiveCornerRadius,
								EffectiveCornerRadius * 2,
								EffectiveCornerRadius * 2
							>;

							using ArcTopRight = Framework::TemplateLayout<
								ParentLayout::Width() - 1 - EffectiveCornerRadius,
								EffectiveCornerRadius,
								EffectiveCornerRadius * 2,
								EffectiveCornerRadius * 2
							>;

							using ArcBottomLeft = Framework::TemplateLayout<
								EffectiveCornerRadius,
								ParentLayout::Height() - 1 - EffectiveCornerRadius,
								EffectiveCornerRadius * 2,
								EffectiveCornerRadius * 2
							>;

							using ArcBottomRight = Framework::TemplateLayout<
								ParentLayout::Width() - 1 - EffectiveCornerRadius,
								ParentLayout::Height() - 1 - EffectiveCornerRadius,
								EffectiveCornerRadius * 2,
								EffectiveCornerRadius * 2
							>;
						};

					public:
						RoundedRectangle() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{}
						~RoundedRectangle() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());

							if (EffectiveStrokeWidth > 1)
							{
								Base::RectangleFill(frame,
									RoundedLayout::LineTop().start.x,
									RoundedLayout::LineTop().start.y,
									RoundedLayout::LineTop().end.x,
									RoundedLayout::LineTop().end.y + EffectiveStrokeWidth - 1);
								Base::RectangleFill(frame,
									RoundedLayout::LineLeft().start.x,
									RoundedLayout::LineLeft().start.y,
									RoundedLayout::LineLeft().end.x + EffectiveStrokeWidth - 1,
									RoundedLayout::LineLeft().end.y);
								Base::RectangleFill(frame,
									RoundedLayout::LineBottom().start.x,
									RoundedLayout::LineBottom().start.y - EffectiveStrokeWidth + 1,
									RoundedLayout::LineBottom().end.x,

									RoundedLayout::LineBottom().end.y);
								Base::RectangleFill(frame,
									RoundedLayout::LineRight().start.x - EffectiveStrokeWidth + 1,
									RoundedLayout::LineRight().start.y,
									RoundedLayout::LineRight().end.x,
									RoundedLayout::LineRight().end.y);

								Base::CircleQuadrantRingFill(frame,
									RoundedLayout::ArcTopLeft::X(), RoundedLayout::ArcTopLeft::Y(),
									EffectiveCornerRadius, InnerCornerRadius, QuadrantEnum::TopLeft, ArcBoundaryModeEnum::ExcludeStartAndEnd, RingBoundaryModeEnum::ExcludeInner);
								Base::CircleQuadrantRingFill(frame,
									RoundedLayout::ArcTopRight::X(),
									RoundedLayout::ArcTopRight::Y(),
									EffectiveCornerRadius, InnerCornerRadius, QuadrantEnum::TopRight, ArcBoundaryModeEnum::ExcludeStartAndEnd, RingBoundaryModeEnum::ExcludeInner);
								Base::CircleQuadrantRingFill(frame,
									RoundedLayout::ArcBottomLeft::X(),
									RoundedLayout::ArcBottomLeft::Y(),
									EffectiveCornerRadius, InnerCornerRadius, QuadrantEnum::BottomLeft, ArcBoundaryModeEnum::ExcludeStartAndEnd, RingBoundaryModeEnum::ExcludeInner);
								Base::CircleQuadrantRingFill(frame,
									RoundedLayout::ArcBottomRight::X(),
									RoundedLayout::ArcBottomRight::Y(),
									EffectiveCornerRadius, InnerCornerRadius, QuadrantEnum::BottomRight, ArcBoundaryModeEnum::ExcludeStartAndEnd, RingBoundaryModeEnum::ExcludeInner);
							}
							else
							{
								static constexpr int8_t DebugOffset = 1;

								Base::Line(frame,
									RoundedLayout::LineTop().start.x + DebugOffset,
									RoundedLayout::LineTop().start.y,
									RoundedLayout::LineTop().end.x - DebugOffset,
									RoundedLayout::LineTop().end.y);
								Base::Line(frame,
									RoundedLayout::LineLeft().start.x,
									RoundedLayout::LineLeft().start.y + DebugOffset,
									RoundedLayout::LineLeft().end.x,
									RoundedLayout::LineLeft().end.y - DebugOffset);
								Base::Line(frame,
									RoundedLayout::LineBottom().start.x + DebugOffset,
									RoundedLayout::LineBottom().start.y,
									RoundedLayout::LineBottom().end.x - DebugOffset,
									RoundedLayout::LineBottom().end.y);
								Base::Line(frame,
									RoundedLayout::LineRight().start.x,
									RoundedLayout::LineRight().start.y + DebugOffset,
									RoundedLayout::LineRight().end.x,
									RoundedLayout::LineRight().end.y - DebugOffset);

								Base::CircleQuadrantLine(frame,
									RoundedLayout::ArcTopLeft::X(),
									RoundedLayout::ArcTopLeft::Y(),
									EffectiveCornerRadius, QuadrantEnum::TopLeft, ArcBoundaryModeEnum::Inclusive);
								Base::CircleQuadrantLine(frame,
									RoundedLayout::ArcTopRight::X(),
									RoundedLayout::ArcTopRight::Y(),
									EffectiveCornerRadius, QuadrantEnum::TopRight, ArcBoundaryModeEnum::Inclusive);
								Base::CircleQuadrantLine(frame,
									RoundedLayout::ArcBottomLeft::X(),
									RoundedLayout::ArcBottomLeft::Y(),
									EffectiveCornerRadius, QuadrantEnum::BottomLeft, ArcBoundaryModeEnum::Inclusive);
								Base::CircleQuadrantLine(frame,
									RoundedLayout::ArcBottomRight::X(),
									RoundedLayout::ArcBottomRight::Y(),
									EffectiveCornerRadius, QuadrantEnum::BottomRight, ArcBoundaryModeEnum::Inclusive);
							}
						}
					};

					/// <summary>
					/// Fills a rounded rectangle using configurable layout and primitive shader.
					/// </summary>
					template<typename dimension_t,
						typename ParentLayout,
						dimension_t CornerRadius,
						typename PixelShaderType = Framework::Shader::Pixel::TemplateShader<dimension_t>
					>
					class RoundedRectangleFill : public Framework::Shader::Geometry::CircleArcShader<dimension_t,
						Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>
					{
					public:
						using DrawableLayout = ParentLayout;

					private:
						using Base = Framework::Shader::Geometry::CircleArcShader<dimension_t,
							Framework::Shader::Geometry::RectangleShader<dimension_t, PixelShaderType>>;

					private:
						struct RoundedLayout
						{
							static constexpr pixel_rectangle_t BigRectangle()
							{
								return pixel_rectangle_t{
									{ 0, CornerRadius},
									{ ParentLayout::Width() - 1,  ParentLayout::Height() - 1 - CornerRadius}
								};
							}

							static constexpr pixel_rectangle_t TopRectangle()
							{
								return pixel_rectangle_t{
									{ CornerRadius,  0},
									{ ParentLayout::Width() - 1 - CornerRadius,  CornerRadius - 1}
								};
							}

							static constexpr pixel_rectangle_t BottomRectangle()
							{
								return pixel_rectangle_t{
									{ CornerRadius,  ParentLayout::Height() - 1 - CornerRadius},
									{ ParentLayout::Width() - 1 - CornerRadius,  ParentLayout::Height() - 1}
								};
							}

							using ArcTopLeft = Framework::TemplateLayout<
								0,
								0,
								CornerRadius * 2,
								CornerRadius * 2
							>;
							using ArcTopRight = Framework::TemplateLayout<
								ParentLayout::Width() - 1 - CornerRadius,
								0,
								CornerRadius * 2,
								CornerRadius * 2
							>;
							using ArcBottomLeft = Framework::TemplateLayout<
								0,
								ParentLayout::Height() - 1 - CornerRadius,
								CornerRadius * 2,
								CornerRadius * 2
							>;
							using ArcBottomRight = Framework::TemplateLayout<
								ParentLayout::Width() - 1 - CornerRadius,
								ParentLayout::Height() - 1 - CornerRadius,
								CornerRadius * 2,
								CornerRadius * 2
							>;
						};

					public:
						RoundedRectangleFill() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
						{}
						~RoundedRectangleFill() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
							using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

							Base::RectangleFill(frame,
								RoundedLayout::BigRectangle().topLeft.x,
								RoundedLayout::BigRectangle().topLeft.y,
								RoundedLayout::BigRectangle().bottomRight.x,
								RoundedLayout::BigRectangle().bottomRight.y);

							Base::RectangleFill(frame,
								RoundedLayout::TopRectangle().topLeft.x,
								RoundedLayout::TopRectangle().topLeft.y,
								RoundedLayout::TopRectangle().bottomRight.x,
								RoundedLayout::TopRectangle().bottomRight.y);

							Base::RectangleFill(frame,
								RoundedLayout::BottomRectangle().topLeft.x,
								RoundedLayout::BottomRectangle().topLeft.y,
								RoundedLayout::BottomRectangle().bottomRight.x,
								RoundedLayout::BottomRectangle().bottomRight.y);

							Base::CircleQuadrantFill(frame,
								RoundedLayout::ArcTopLeft::X() + CornerRadius,
								RoundedLayout::ArcTopLeft::Y() + CornerRadius,
								CornerRadius, QuadrantEnum::TopLeft);
							Base::CircleQuadrantFill(frame,
								RoundedLayout::ArcTopRight::X(),
								RoundedLayout::ArcTopRight::Y() + CornerRadius,
								CornerRadius, QuadrantEnum::TopRight);
							Base::CircleQuadrantFill(frame,
								RoundedLayout::ArcBottomLeft::X() + CornerRadius,
								RoundedLayout::ArcBottomLeft::Y(),
								CornerRadius, QuadrantEnum::BottomLeft);
							Base::CircleQuadrantFill(frame,
								RoundedLayout::ArcBottomRight::X(),
								RoundedLayout::ArcBottomRight::Y(),
								CornerRadius, QuadrantEnum::BottomRight);
						}
					};
				}
			}
		}
	}
}
#endif