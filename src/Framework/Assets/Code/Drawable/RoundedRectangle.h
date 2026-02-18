#ifndef _EGFX_FRAMEWORK_ASSETS_CODE_DRAWABLE_ROUNDED_RECTANGLE_h
#define _EGFX_FRAMEWORK_ASSETS_CODE_DRAWABLE_ROUNDED_RECTANGLE_h

#include <EgfxFramework.h>
#include "../../Shader/Geometry/Circle.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Code
			{
				namespace Drawable
				{
					/// <summary>
					/// Draws a rounded rectangle outline using configurable layout and primitive shader.
					/// </summary>
					template<typename ParentLayout,
						typename dimension_t,
						dimension_t CornerRadius,
						typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
					>
					class RoundedRectangle : public Framework::Assets::Shader::Geometry::CircleShader<dimension_t, PrimitiveShaderType>
					{
					private:
						using Base = Framework::Assets::Shader::Geometry::CircleShader<dimension_t, PrimitiveShaderType>;

					private:
						struct RoundedLayout
						{
							static constexpr pixel_line_t LineTop()
							{
								return pixel_line_t{
									{ CornerRadius,  0},
									{ ParentLayout::Width() - 1 - CornerRadius,  0}
								};
							}

							static constexpr pixel_line_t LineLeft()
							{
								return pixel_line_t{
									{ 0,  CornerRadius},
									{ 0,  ParentLayout::Height() - 1 - CornerRadius}
								};
							}

							static constexpr pixel_line_t LineBottom()
							{
								return pixel_line_t{
									{ CornerRadius,  ParentLayout::Height() - 1},
									{ ParentLayout::Width() - 1 - CornerRadius,  ParentLayout::Height() - 1}
								};
							}

							static constexpr pixel_line_t LineRight()
							{
								return pixel_line_t{
									{ ParentLayout::Width() - 1,  CornerRadius},
									{ ParentLayout::Width() - 1,  ParentLayout::Height() - 1 - CornerRadius}
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
						RoundedRectangle() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						}
						~RoundedRectangle() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::Line(frame,
								RoundedLayout::LineTop().start.x,
								RoundedLayout::LineTop().start.y,
								RoundedLayout::LineTop().end.x,
								RoundedLayout::LineTop().end.y);
							Base::Line(frame,
								RoundedLayout::LineLeft().start.x,
								RoundedLayout::LineLeft().start.y,
								RoundedLayout::LineLeft().end.x,
								RoundedLayout::LineLeft().end.y);
							Base::Line(frame,
								RoundedLayout::LineBottom().start.x,
								RoundedLayout::LineBottom().start.y,
								RoundedLayout::LineBottom().end.x,
								RoundedLayout::LineBottom().end.y);
							Base::Line(frame,
								RoundedLayout::LineRight().start.x,
								RoundedLayout::LineRight().start.y,
								RoundedLayout::LineRight().end.x,
								RoundedLayout::LineRight().end.y);

							Base::CircleQuarterTopLeft(frame,
								RoundedLayout::ArcTopLeft::X() + CornerRadius,
								RoundedLayout::ArcTopLeft::Y() + CornerRadius,
								CornerRadius);
							Base::CircleQuarterTopRight(frame,
								RoundedLayout::ArcTopRight::X(),
								RoundedLayout::ArcTopRight::Y() + CornerRadius,
								CornerRadius);
							Base::CircleQuarterBottomLeft(frame,
								RoundedLayout::ArcBottomLeft::X() + CornerRadius,
								RoundedLayout::ArcBottomLeft::Y(),
								CornerRadius);
							Base::CircleQuarterBottomRight(frame,
								RoundedLayout::ArcBottomRight::X(),
								RoundedLayout::ArcBottomRight::Y(),
								CornerRadius);
						}
					};

					/// <summary>
					/// Fills a rounded rectangle using configurable layout and primitive shader.
					/// </summary>
					template<typename ParentLayout,
						typename dimension_t,
						dimension_t CornerRadius,
						typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
					>
					class RoundedRectangleFill : public Framework::Assets::Shader::Geometry::CircleShader<dimension_t, PrimitiveShaderType>
					{
					private:
						using Base = Framework::Assets::Shader::Geometry::CircleShader<dimension_t, PrimitiveShaderType>;

					private:
						struct RoundedLayout
						{
							static constexpr pixel_rectangle_t Rectangle()
							{
								return pixel_rectangle_t{
									{ CornerRadius,  CornerRadius},
									{ ParentLayout::Width() - 1 - CornerRadius,  ParentLayout::Height() - 1 - CornerRadius}
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
						RoundedRectangleFill() : Base()
						{
							Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						}
						~RoundedRectangleFill() = default;

						void Draw(IFrameBuffer* frame)
						{
							Base::RectangleFill(frame,
								RoundedLayout::Rectangle().topLeft.x,
								RoundedLayout::Rectangle().topLeft.y,
								RoundedLayout::Rectangle().bottomRight.x,
								RoundedLayout::Rectangle().bottomRight.y);

							Base::CircleQuarterTopLeftFill(frame,
								RoundedLayout::ArcTopLeft::X() + CornerRadius,
								RoundedLayout::ArcTopLeft::Y() + CornerRadius,
								CornerRadius);
							Base::CircleQuarterTopRightFill(frame,
								RoundedLayout::ArcTopRight::X(),
								RoundedLayout::ArcTopRight::Y() + CornerRadius,
								CornerRadius);
							Base::CircleQuarterBottomLeftFill(frame,
								RoundedLayout::ArcBottomLeft::X() + CornerRadius,
								RoundedLayout::ArcBottomLeft::Y(),
								CornerRadius);
							Base::CircleQuarterBottomRightFill(frame,
								RoundedLayout::ArcBottomRight::X(),
								RoundedLayout::ArcBottomRight::Y(),
								CornerRadius);
						}
					};
				}
			}
		}
	}
}
#endif