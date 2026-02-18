#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_RECTANGLE_H
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_RECTANGLE_H

#include "Line.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				/// <summary>
				/// Geometry shader stage that rasterizes an axis-aligned filled rectangle on top of a line/pixel shader pipeline.
				/// Uses scanline emission via LineHorizontal and selects fast paths when the source is constant and/or the transform is disabled.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader type providing Pixel(...), PixelBlend(...), and the ColorSource/ColorShader/TransformShader members.</typeparam>
				template<typename dimension_t,
					typename PixelShaderType = Shader::Pixel::TemplateShader<dimension_t>
				>
				class RectangleShader : public LineShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = LineShader<dimension_t, PixelShaderType>;

				protected:
					using Base::PixelBlend;

				public:
					using Base::ColorSource;
					using Base::ColorShader;
					using Base::TransformShader;

				private:
					// Extract shader types for compile-time optimizations.
					using ColorSourceType = typename PixelShaderType::color_source_t;
					using ColorShaderType = typename PixelShaderType::color_shader_t;
					using TransformShaderType = typename PixelShaderType::transform_shader_t;

				private:
					// Compile-time dispatch: skip per-pixel source sampling when the source is constant.
					using SkipSourceTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Shader::Source::ConstantColorFlag<ColorSourceType>::value>::type;

					using SkipShadeTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<ColorShaderType, Shader::Color::NoShader<dimension_t>>::value&&
						IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value
						>::type;

					using SkipColorTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<ColorShaderType, Shader::Color::NoShader<dimension_t>>::value
						>::type;

					// Compile-time dispatch: skip Transform(...) when the transform shader is NoTransform.
					using SkipTransformTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;


				public:
					RectangleShader() : Base() {}
					~RectangleShader() = default;

					/// <summary>
					/// Fills an axis-aligned rectangle defined by two corners (inclusive) in local space.
					/// Per-pixel shading, origin offset, and optional coordinate transforms are provided by the underlying pixel shader.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x1">Corner X coordinate (local space).</param>
					/// <param name="y1">Corner Y coordinate (local space).</param>
					/// <param name="x2">Opposing corner X coordinate (local space).</param>
					/// <param name="y2">Opposing corner Y coordinate (local space).</param>
					void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						RectangleFill(framebuffer, x1, y1, x2, y2, SkipSourceTag{}, SkipTransformTag{});
					}

				private:
					// SkipSource=False, SkipTransform=False: emit scanlines via full pixel shader path.
					inline void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						for (dimension_t y = startY; y <= endY; y++)
						{
							LineHorizontal(framebuffer, x1, x2, y, TypeTraits::TypeDispatch::FalseType{}, TypeTraits::TypeDispatch::FalseType{});
						}
					}

					// SkipSource=False, SkipTransform=True: shade per pixel, no transform.
					inline void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						for (dimension_t y = startY; y <= endY; y++)
						{
							LineHorizontal(framebuffer, x1, x2, y, TypeTraits::TypeDispatch::FalseType{}, TypeTraits::TypeDispatch::TrueType{});
						}
					}

					// SkipSource=True, SkipTransform=False: constant shade once, transform per pixel.
					inline void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						dimension_t fx, fy;
						for (dimension_t y = startY; y <= endY; y++)
						{
							for (dimension_t x = MinValue(x1, x2); x <= MaxValue(x1, x2); x++)
							{
								fx = x;
								fy = y;
								if (TransformShader.Transform(fx, fy))
								{
									PixelBlend(framebuffer, color, fx, fy);
								}
							}
						}
					}

					// SkipSource=True, SkipTransform=True: constant shade once, no transform; use the framebuffer optimized rectangle fill.
					inline void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						framebuffer->RectangleFill(ColorShader.Shade(ColorSource.Source(0, 0)),
							Base::Origin.x + x1, Base::Origin.y + y1,
							Base::Origin.x + x2, Base::Origin.y + y2);
					}
				};
			}
		}
	}
}
#endif