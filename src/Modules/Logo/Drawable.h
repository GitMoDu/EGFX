#ifndef _INTEGERGLASS_MODULES_LOGO_DRAWABLE_h
#define _INTEGERGLASS_MODULES_LOGO_DRAWABLE_h

#include "Constant.h"
#include "Layout.h"
#include "Assets.h"
#include "Shader.h"
#include "../../Framework/Image/Bitmask/Drawable.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{
			namespace Drawable
			{
				template<typename ParentLayout, bool Monochrome>
				class GlassText : public Shader::Primitive::GlassText<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, Monochrome>
				{
				private:
					using Base = Shader::Primitive::GlassText<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, Monochrome>;

					using MonochromeTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Monochrome
						>::type;

					using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

				private:
					static constexpr dimension_t SquareOuterSize = Layout::Calc::DotMatrixLayout<ParentLayout>::SquareOuterSize();
					static constexpr dimension_t SquareInnerSize = Layout::Calc::DotMatrixLayout<ParentLayout>::SquareInnerSize();

				public:
					using Base::ColorSource;
					using Base::ColorShader;
					using Base::TransformShader;

				private:
					Shader::Dither::RngDitherer Ditherer{};

					uint8_t Alpha = UINT8_MAX;

				public:
					GlassText() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
					{
						SetColorSquareOuterSize(MonochromeTag{});
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}
					~GlassText() = default;

					void SetColors(const rgb_color_t color1, const rgb_color_t color2)
					{
						ColorSource.SetColors(color1, color2);
					}

					void SetAlpha(const uint8_t alpha)
					{
						Alpha = alpha;
					}

				private:
					void SetColorSquareOuterSize(TypeTraits::TypeDispatch::TrueType)
					{}

					void SetColorSquareOuterSize(TypeTraits::TypeDispatch::FalseType)
					{
						ColorSource.SetSquareOuterSize(SquareOuterSize);
					}

				public:

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						DrawG(frame);
						DrawL(frame);
						DrawA(frame);
						DrawSS<0>(frame);
						DrawSS<1>(frame);
					}

					void DrawG(IFrameBuffer* frame)
					{
						DrawLine<0>(frame, 1, 4, 0, MonochromeTag{});
						DrawLine<0>(frame, 0, 5, 1, MonochromeTag{});
						DrawLine<0>(frame, 0, 1, 2, MonochromeTag{});
						DrawLine<0>(frame, 0, 1, 3, MonochromeTag{});
						DrawLine<0>(frame, 3, 5, 3, MonochromeTag{});
						DrawLine<0>(frame, 0, 1, 4, MonochromeTag{});
						DrawLine<0>(frame, 3, 5, 4, MonochromeTag{});
						DrawLine<0>(frame, 0, 1, 5, MonochromeTag{});
						DrawLine<0>(frame, 4, 5, 5, MonochromeTag{});
						DrawLine<0>(frame, 0, 5, 6, MonochromeTag{});
						DrawLine<0>(frame, 1, 4, 7, MonochromeTag{});
					}

					void DrawL(IFrameBuffer* frame)
					{
						DrawLine<1>(frame, 0, 1, 0, MonochromeTag{});
						DrawLine<1>(frame, 0, 1, 1, MonochromeTag{});
						DrawLine<1>(frame, 0, 1, 2, MonochromeTag{});
						DrawLine<1>(frame, 0, 1, 3, MonochromeTag{});
						DrawLine<1>(frame, 0, 1, 4, MonochromeTag{});
						DrawLine<1>(frame, 0, 1, 5, MonochromeTag{});
						DrawLine<1>(frame, 0, 5, 6, MonochromeTag{});
						DrawLine<1>(frame, 0, 5, 7, MonochromeTag{});
					}

					void DrawA(IFrameBuffer* frame)
					{
						DrawLine<2>(frame, 2, 3, 0, MonochromeTag{});
						DrawLine<2>(frame, 1, 4, 1, MonochromeTag{});
						DrawLine<2>(frame, 0, 1, 2, MonochromeTag{});
						DrawLine<2>(frame, 4, 5, 2, MonochromeTag{});
						DrawLine<2>(frame, 0, 1, 3, MonochromeTag{});
						DrawLine<2>(frame, 4, 5, 3, MonochromeTag{});
						DrawLine<2>(frame, 0, 5, 4, MonochromeTag{});
						DrawLine<2>(frame, 0, 5, 5, MonochromeTag{});
						DrawLine<2>(frame, 0, 1, 6, MonochromeTag{});
						DrawLine<2>(frame, 4, 5, 6, MonochromeTag{});
						DrawLine<2>(frame, 0, 1, 7, MonochromeTag{});
						DrawLine<2>(frame, 4, 5, 7, MonochromeTag{});
					}

					template<uint8_t offset>
					void DrawSS(IFrameBuffer* frame)
					{
						DrawLine<3 + offset>(frame, 1, 4, 0, MonochromeTag{});
						DrawLine<3 + offset>(frame, 0, 5, 1, MonochromeTag{});
						DrawLine<3 + offset>(frame, 0, 1, 2, MonochromeTag{});
						DrawLine<3 + offset>(frame, 0, 4, 3, MonochromeTag{});
						DrawLine<3 + offset>(frame, 1, 5, 4, MonochromeTag{});
						DrawLine<3 + offset>(frame, 4, 5, 5, MonochromeTag{});
						DrawLine<3 + offset>(frame, 0, 5, 6, MonochromeTag{});
						DrawLine<3 + offset>(frame, 1, 4, 7, MonochromeTag{});
					}

					template<uint8_t letterIndex>
					void DrawLine(IFrameBuffer* frame, const dimension_t x1, const dimension_t x2, const dimension_t y, TypeTraits::TypeDispatch::TrueType)
					{
						using DotMatrixLayout = Layout::Calc::DotMatrixLayout<ParentLayout>;
						pixel_rectangle_t rectangle;

						for (dimension_t x = x1; x <= x2; x++)
						{
							rectangle = DotMatrixLayout::template GetSquare<letterIndex>(x, y);

							if (Ditherer.Dither(Alpha))
							{
								Base::RectangleFill(frame, rectangle.topLeft.x, rectangle.topLeft.y,
									rectangle.topLeft.x + SquareInnerSize - 1,
									rectangle.topLeft.y + SquareInnerSize - 1);
							}
						}
					}

					template<uint8_t letterIndex>
					void DrawLine(IFrameBuffer* frame, const dimension_t x1, const dimension_t x2, const dimension_t y, TypeTraits::TypeDispatch::FalseType)
					{
						using DotMatrixLayout = Layout::Calc::DotMatrixLayout<ParentLayout>;
						pixel_rectangle_t rectangle;

						for (dimension_t x = x1; x <= x2; x++)
						{
							rectangle = DotMatrixLayout::template GetSquare<letterIndex>(x, y);

							if (Ditherer.Dither(Alpha))
							{
								Base::RectangleFill(frame, rectangle.topLeft.x, rectangle.topLeft.y,
									rectangle.topLeft.x + SquareInnerSize - 1,
									rectangle.topLeft.y + SquareInnerSize - 1);
							}
						}
					}
				};

				template<typename ParentLayout, bool Monochrome
				>
				class Label : public ::IntegerGlass::Framework::Image::Bitmask::Drawable::StaticImage<
					typename ::IntegerGlass::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					ParentLayout,
					Assets::IntegerLabel::SourceType,
					::IntegerGlass::Framework::Image::TemplateImageStyle<
					::IntegerGlass::Framework::Layout::AlignmentEnum::MiddleCenter,
					false,
					RGB_COLOR_BLACK,
					MaxValue<uint8_t>(1, ParentLayout::Width() / Assets::IntegerLabel::SourceType::Width),
					MaxValue<uint8_t>(1, ParentLayout::Height() / Assets::IntegerLabel::SourceType::Height)>,
					::IntegerGlass::Framework::Shader::Source::StaticColor<typename ::IntegerGlass::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
					::IntegerGlass::Framework::Shader::Color::NoShader<typename ::IntegerGlass::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
					::IntegerGlass::Modules::Logo::Shader::Transform::AlphaDither<typename ::IntegerGlass::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
				>
				{

				private:
					using Base = ::IntegerGlass::Framework::Image::Bitmask::Drawable::StaticImage<
						typename ::IntegerGlass::Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						ParentLayout,
						Assets::IntegerLabel::SourceType,
						Framework::Image::TemplateImageStyle<
						Framework::Layout::AlignmentEnum::MiddleCenter,
						false,
						RGB_COLOR_BLACK,
						MaxValue<uint8_t>(1, ParentLayout::Width() / Assets::IntegerLabel::SourceType::Width),
						MaxValue<uint8_t>(1, ParentLayout::Height() / Assets::IntegerLabel::SourceType::Height)>,
						Framework::Shader::Source::StaticColor<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
						Framework::Shader::Color::NoShader<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>,
						Shader::Transform::AlphaDither<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>
					>;

				public:
					Label() : Base() {}
					~Label() = default;

					void SetAlpha(const uint8_t alpha)
					{
						Base::TransformShader.SetAlpha(alpha);
					}
				};

				template<
					typename ParentLayout,
					bool Monochrome,
					typename PrimitiveShaderType = Framework::Shader::Geometry::RectangleShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Shader::Pixel::Glass<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, Monochrome>>
					>
					class Glass : public Framework::Shader::Geometry::CircleRingShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					PrimitiveShaderType>
				{
				private:
					using Base = Framework::Shader::Geometry::CircleRingShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						PrimitiveShaderType>;

					using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;

				private:
					static constexpr dimension_t ArcWidth = Layout::Calc::GlassEdgesLayout<ParentLayout>::CornerRadius();

				public:
					Glass() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}
					~Glass() = default;

					void Draw(IFrameBuffer* frame)
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
						using QuadrantEnum = Framework::Shader::Geometry::QuadrantEnum;

						Base::Line(frame,
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineTop().start.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineTop().start.y),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineTop().end.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineTop().end.y));
						Base::Line(frame,
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineLeft().start.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineLeft().start.y),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineLeft().end.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineLeft().end.y));
						Base::Line(frame,
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineBottom().start.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineBottom().start.y),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineBottom().end.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineBottom().end.y));
						Base::Line(frame,
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineRight().start.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineRight().start.y),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineRight().end.x),
							static_cast<dimension_t>(Layout::Calc::GlassEdgesLayout<ParentLayout>::LineRight().end.y));

						Base::CircleQuadrantLine(frame,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcTopLeft::X() + ArcWidth,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcTopLeft::Y() + ArcWidth,
							ArcWidth, QuadrantEnum::TopLeft);
						Base::CircleQuadrantLine(frame,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcTopRight::X(),
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcTopRight::Y() + ArcWidth,
							ArcWidth, QuadrantEnum::TopRight);
						Base::CircleQuadrantLine(frame,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcBottomLeft::X() + ArcWidth,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcBottomLeft::Y(),
							ArcWidth, QuadrantEnum::BottomLeft);
						Base::CircleQuadrantLine(frame,
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcBottomRight::X(),
							Layout::Calc::GlassEdgesLayout<ParentLayout>::ArcBottomRight::Y(),
							ArcWidth, QuadrantEnum::BottomRight);
					}
				};

				template<typename ParentLayout, bool Monochrome>
				class Reflection : public Framework::Shader::Geometry::TriangleShader<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					Shader::Pixel::Reflection<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					static_cast<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>(ParentLayout::Width()),
					static_cast<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>(ParentLayout::Height()), Monochrome>>
				{
				private:
					using Base = Framework::Shader::Geometry::TriangleShader<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						Shader::Pixel::Reflection<
						typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
						static_cast<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>(ParentLayout::Width()),
						static_cast<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>(ParentLayout::Height()), Monochrome>>;

					using MonochromeTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						Monochrome>::type;

					using dimension_t = typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t;
					using signed_t = typename Framework::AutoDimension::ByDimension<dimension_t>::signed_t;

				private:
					void DrawTriangle(IFrameBuffer* frame, const pixel_triangle_t triangle)
					{
						// Our triangles go outside the bounds of the reflection shader,
						// so we need to clip and unroll the triangles fill to avoid drawing outside the shader bounds.
						Base::TriangleFillClipped(frame,
							static_cast<signed_t>(triangle.a.x) + OffsetX, triangle.a.y,
							static_cast<signed_t>(triangle.b.x) + OffsetX, triangle.b.y,
							static_cast<signed_t>(triangle.c.x) + OffsetX, triangle.c.y);
					}

				public:
					signed_t OffsetX = 0;
					bool Visible = true;

				public:
					Reflection() : Base(0, 0, ParentLayout::Width(), ParentLayout::Height())
					{
						Base::Prepare(ParentLayout::X(), ParentLayout::Y());
					}

					~Reflection() = default;

					void SetAlpha(const uint8_t alpha, TypeTraits::TypeDispatch::TrueType)
					{
						Base::TransformShader.SetAlpha(alpha);
					}

					void SetAlpha(const uint8_t alpha, TypeTraits::TypeDispatch::FalseType)
					{
						const rgb_color_t alphaColor = Rgb::Color(
							(static_cast<uint16_t>(Rgb::R(Colors::Glass)) * alpha) >> 8,
							(static_cast<uint16_t>(Rgb::G(Colors::Glass)) * alpha) >> 8,
							(static_cast<uint16_t>(Rgb::B(Colors::Glass)) * alpha) >> 8);

						Base::ColorSource.Color = alphaColor;
					}

					void SetAlpha(const uint8_t alpha)
					{
						SetAlpha(alpha, MonochromeTag{});
					}

					void Draw(IFrameBuffer* frame)
					{
						if (!Visible)
						{
							return;
						}

						Base::Prepare(ParentLayout::X(), ParentLayout::Y());

						DrawTriangle(frame, Layout::Calc::ReflectionLayout<ParentLayout>::Band1TriangleTop());
						DrawTriangle(frame, Layout::Calc::ReflectionLayout<ParentLayout>::Band1TriangleBottom());
						DrawTriangle(frame, Layout::Calc::ReflectionLayout<ParentLayout>::Band2TriangleTop());
						DrawTriangle(frame, Layout::Calc::ReflectionLayout<ParentLayout>::Band2TriangleBottom());
					}
				};
			}
		}
	}
}
#endif