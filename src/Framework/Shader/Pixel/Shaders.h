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
				/// Pixel shader that draws a color source directly (no color shading and no transform shader).
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorSourceType">Color source type providing Source(x, y).</typeparam>
				template<typename dimension_t,
					typename ColorSourceType = Shader::Source::SingleColor<dimension_t>
				>
				class NoShader
				{
				public:
					/// <summary>Color source used to produce the pixel color.</summary>
					ColorSourceType ColorSource{};

				private:
					pixel_point_t Origin{ 0, 0 };

				public:
					NoShader() = default;
					~NoShader() = default;

					/// <summary>
					/// Prepares the shader for a draw cycle by setting the pixel-space origin.
					/// Call once before Pixel().
					/// </summary>
					/// <param name="originX">Pixel-space origin X (pixels).</param>
					/// <param name="originY">Pixel-space origin Y (pixels).</param>
					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
					}

					/// <summary>
					/// Shades and writes a single pixel at the given local-space coordinate.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Local-space X coordinate.</param>
					/// <param name="y">Local-space Y coordinate.</param>
					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						framebuffer->Pixel(ColorSource.Source(x, y),
							static_cast<pixel_t>(Origin.x + x),
							static_cast<pixel_t>(Origin.y + y));
					}
				};

				/// <summary>
				/// Pixel shader that combines a color source, an optional color shader, and a transform shader.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorSourceType">Color source type providing Source(x, y).</typeparam>
				/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
				/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
				template<typename dimension_t,
					typename ColorSourceType,
					typename ColorShaderType,
					typename TransformShaderType
				>
				class TemplateColorAndTransform
				{
				public:
					ColorSourceType ColorSource{};
					ColorShaderType ColorShader{};
					TransformShaderType TransformShader{};

				private:
					pixel_point_t Origin{ 0, 0 };

				public:
					TemplateColorAndTransform() = default;
					~TemplateColorAndTransform() = default;

					/// <summary>
					/// Prepares the shader for a draw cycle by setting the pixel-space origin and preparing the transform shader.
					/// Call once before Pixel().
					/// </summary>
					/// <param name="originX">Pixel-space origin X (pixels).</param>
					/// <param name="originY">Pixel-space origin Y (pixels).</param>
					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
						TransformShader.Prepare(originX, originY);
					}

					/// <summary>
					/// Shades and writes a single pixel at the given local-space coordinate.
					/// The transform shader may discard the pixel by returning false.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Local-space X coordinate.</param>
					/// <param name="y">Local-space Y coordinate.</param>
					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						dimension_t coordinatesX = x;
						dimension_t coordinatesY = y;
						if (TransformShader.Transform(coordinatesX, coordinatesY))
						{
							framebuffer->Pixel(ColorShader.Shade(ColorSource.Source(x, y)),
								static_cast<pixel_t>(Origin.x + coordinatesX),
								static_cast<pixel_t>(Origin.y + coordinatesY));
						}
					}
				};

				/// <summary>
				/// Pixel shader alias that selects the optimized path when both shader types are no-ops,
				/// otherwise falls back to the full implementation.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorSourceType">Color source type providing Source(x, y).</typeparam>
				/// <typeparam name="ColorShaderType">Color shader type providing Shade(color).</typeparam>
				/// <typeparam name="TransformShaderType">Transform shader type providing Prepare(...) and Transform(x, y).</typeparam>
				template<typename dimension_t,
					typename ColorSourceType = Shader::Source::SingleColor<dimension_t>,
					typename ColorShaderType = Shader::Color::NoShader<dimension_t>,
					typename TransformShaderType = Shader::Transform::NoTransform<dimension_t>
				>
				using ColorAndTransform = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
					NoShader<dimension_t, ColorSourceType>,
					TemplateColorAndTransform<dimension_t, ColorSourceType, ColorShaderType, TransformShaderType>,
					IntegerSignal::TypeTraits::TypeDispatch::is_same<ColorShaderType, Shader::Color::NoShader<dimension_t>>::value&&
					IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value
				>::type;
			}
		}
	}
}
#endif