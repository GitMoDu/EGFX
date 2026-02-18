#ifndef _EGFX_FRAMEWORK_SHADER_PIXEL_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_PIXEL_SHADERS_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Pixel
			{
				/// <summary>
				/// Blend modes for pixel shaders.
				/// Selects which framebuffer write/blend operation is used when emitting the final shaded color.
				/// </summary>
				enum class BlendModeEnum : uint8_t
				{
					Replace,
					Add,
					Subtract,
					Multiply,
					Screen,
					BlendHalfAlpha
				};

				/// <summary>
				/// Lowest-level shader stage that shades a single pixel.
				/// Combines a color source, an optional color shader, and an optional transform shader, then writes to the framebuffer using the selected blend mode.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorSourceType">Color source type providing Source(x, y).</typeparam>
				/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
				/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
				/// <typeparam name="BlendMode">Blend mode for combining the shader's output color with the existing framebuffer pixel color.</typeparam>
				template<typename dimension_t,
					typename ColorSourceType = Shader::Source::StaticColor<dimension_t>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>,
					BlendModeEnum BlendMode = BlendModeEnum::Replace
				>
				class TemplateShader
				{
				public:
					using color_source_t = ColorSourceType;
					using color_shader_t = ColorShaderType;
					using transform_shader_t = TransformShaderType;

				private:
					// Compile-time dispatch: skip Transform(...) when TransformShaderType is NoTransform.
					using SkipTransformTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;

				public:
					ColorSourceType ColorSource{};
					ColorShaderType ColorShader{};
					TransformShaderType TransformShader{};

				protected:
					// Pixel-space origin added to local-space coordinates when writing to the framebuffer.
					pixel_point_t Origin{ 0, 0 };

					uint8_t BlendAlpha = INT8_MAX;

				public:
					TemplateShader() = default;
					~TemplateShader() = default;

					/// <summary>
					/// Prepares the shader for a draw cycle.
					/// Sets the pixel-space origin used for framebuffer writes and forwards preparation to the transform shader.
					/// </summary>
					/// <param name="originX">Pixel-space origin X (pixels).</param>
					/// <param name="originY">Pixel-space origin Y (pixels).</param>
					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
						TransformShader.Prepare(originX, originY);
					}

					inline void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y, TypeTraits::TypeDispatch::FalseType)
					{
						dimension_t coordinatesX = x;
						dimension_t coordinatesY = y;

						// Transform may modify the local-space coordinates and can reject the pixel by returning false.
						if (TransformShader.Transform(coordinatesX, coordinatesY))
						{
							PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)),
								static_cast<pixel_t>(coordinatesX),
								static_cast<pixel_t>(coordinatesY));
						}
					}

					inline void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y, TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)),
							static_cast<pixel_t>(x),
							static_cast<pixel_t>(y));
					}

					/// <summary>
					/// Shades and writes a single pixel at the given local-space coordinate.
					/// Forwards to the optimal drawing method based on the presence of the color and transform shaders to minimize overhead.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Local-space X coordinate.</param>
					/// <param name="y">Local-space Y coordinate.</param>
					inline void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						Pixel(framebuffer, x, y, SkipTransformTag{});
					}

				protected:
					/// <summary>
					/// Writes a pre-shaded color using the selected blend mode.
					/// Applies the prepared origin offset when addressing the framebuffer.
					/// </summary>
					inline void PixelBlend(IFrameBuffer* framebuffer, const rgb_color_t color, const dimension_t x, const dimension_t y)
					{
						switch (BlendMode)
						{
						case BlendModeEnum::BlendHalfAlpha:
							framebuffer->PixelBlend(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						case BlendModeEnum::Add:
							framebuffer->PixelBlendAdd(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						case BlendModeEnum::Subtract:
							framebuffer->PixelBlendSubtract(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						case BlendModeEnum::Multiply:
							framebuffer->PixelBlendMultiply(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						case BlendModeEnum::Screen:
							framebuffer->PixelBlendScreen(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						default:
						case BlendModeEnum::Replace:
							framebuffer->Pixel(color,
								static_cast<pixel_t>(Origin.x + x),
								static_cast<pixel_t>(Origin.y + y));
							break;
						}
					}
				};
			}
		}
	}
}
#endif