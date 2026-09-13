#ifndef _EGFX_FRAMEWORK_SHADER_PIXEL_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_PIXEL_SHADERS_h

#include "Model.h"

#include "../Color/Shaders.h"
#include "../Transform/Shaders.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Pixel
			{
				/// <summary>
				/// Mock pixel shader that does nothing, used as compile-time option for filling in for a pixel shader when none is needed.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct MockShader
				{
					void Prepare(const dimension_t /*originX*/, const dimension_t /*originY*/) {}
					void Pixel(IFrameBuffer* /*framebuffer*/, const dimension_t /*x*/, const dimension_t /*y*/) {}
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
					static constexpr bool IsReplaceBlendMode = BlendMode == BlendModeEnum::Replace;

					static constexpr dimension_t MaxDimension = IntegerSignal::TypeTraits::TypeLimits::type_limits<dimension_t>::Max();

				private:
					// Compile-time dispatch: skip Transform(...) when TransformShaderType is NoTransform.
					using SkipTransformTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;

					using viewport_t = Shader::viewport_t<dimension_t>;

				public:
					ColorSourceType ColorSource{};
					ColorShaderType ColorShader{};
					TransformShaderType TransformShader{};

				private:
					viewport_t Viewport;

				protected:
					// Pixel-space origin and translation added to local-space coordinates when writing to the framebuffer.
					pixel_point_t Origin{ 0, 0 };

				public:
					TemplateShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Viewport{ left, top, right, bottom, 0, 0 }
					{}

					~TemplateShader() = default;

					void SetBounds(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
					{
						Viewport.BoundsLeft = left;
						Viewport.BoundsTop = top;
						Viewport.BoundsRight = right;
						Viewport.BoundsBottom = bottom;
					}

					/// <summary>
					/// Sets the translation offset applied before writing to the framebuffer.
					/// </summary>
					/// <param name="x">Translation along the X axis.</param>
					/// <param name="y">Translation along the Y axis.</param>
					void SetTranslation(const pixel_t x, const pixel_t y)
					{
						Viewport.TranslationX = x;
						Viewport.TranslationY = y;
					}

					bool IsVisible() const
					{
						return Viewport.IsValid();
					}

					/// <summary>
					/// Prepares the shader for a draw cycle.
					/// Must be called at least once per draw cycle before any Pixel(...) calls to set the pixel-space origin for framebuffer writes.
					/// Sets the pixel-space origin used for framebuffer writes and forwards preparation to the transform shader.
					/// </summary>
					/// <param name="originX">Pixel-space origin X (pixels).</param>
					/// <param name="originY">Pixel-space origin Y (pixels).</param>
					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX + Viewport.TranslationX;
						Origin.y = originY + Viewport.TranslationY;
						TransformShader.Prepare(Origin.x, Origin.y);
					}

					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y, TypeTraits::TypeDispatch::FalseType)
					{
						dimension_t coordinatesX = x;
						dimension_t coordinatesY = y;

						// Transform may modify the local-space coordinates and can reject the pixel by returning false.
						if (TransformShader.Transform(coordinatesX, coordinatesY))
						{
							PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)),
								coordinatesX, coordinatesY);
						}
					}

					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y, TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
					}

					/// <summary>
					/// Shades and writes a single pixel at the given local-space coordinate.
					/// Forwards to the optimal drawing method based on the presence of the color and transform shaders to minimize overhead.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x">Local-space X coordinate.</param>
					/// <param name="y">Local-space Y coordinate.</param>
					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						Pixel(framebuffer, x, y, SkipTransformTag{});
					}

				protected:
					bool IsInsideBounds(const dimension_t x, const dimension_t y) const
					{
						return Viewport.IsInside(x, y);
					}

					dimension_t GetBoundsWidth() const
					{
						return Viewport.GetWidth();
					}

					dimension_t GetBoundsHeight() const
					{
						return Viewport.GetHeight();
					}

					dimension_t GetBoundsLeft() const
					{
						return Viewport.GetTopLeftX();
					}

					dimension_t GetBoundsTop() const
					{
						return Viewport.GetTopLeftY();
					}

					dimension_t GetBoundsRight() const
					{
						return Viewport.GetBottomRightX();
					}

					dimension_t GetBoundsBottom() const
					{
						return Viewport.GetBottomRightY();
					}

					/// <summary>
					/// Shades and writes a pixel without applying the layout clip.
					/// Higher-level geometry shaders may use this after performing primitive-level culling.
					/// </summary>
					void PixelUnclipped(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						PixelUnclipped(framebuffer, x, y, SkipTransformTag{});
					}

					void PixelUnclipped(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlendUnclipped(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
					}

					void PixelUnclipped(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType)
					{
						dimension_t coordinatesX = x;
						dimension_t coordinatesY = y;

						if (TransformShader.Transform(coordinatesX, coordinatesY))
						{
							PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)),
								coordinatesX, coordinatesY);
						}
					}

					void PixelBlendUnclipped(IFrameBuffer* framebuffer, const rgb_color_t color, const dimension_t x, const dimension_t y)
					{
						const pixel_t targetX = static_cast<pixel_t>(Origin.x) + x;
						const pixel_t targetY = static_cast<pixel_t>(Origin.y) + y;

						switch (BlendMode)
						{
						case BlendModeEnum::BlendHalfAlpha:
							framebuffer->PixelBlend(color, targetX, targetY);
							break;
						case BlendModeEnum::Add:
							framebuffer->PixelBlendAdd(color, targetX, targetY);
							break;
						case BlendModeEnum::Subtract:
							framebuffer->PixelBlendSubtract(color, targetX, targetY);
							break;
						case BlendModeEnum::Multiply:
							framebuffer->PixelBlendMultiply(color, targetX, targetY);
							break;
						case BlendModeEnum::Screen:
							framebuffer->PixelBlendScreen(color, targetX, targetY);
							break;
						case BlendModeEnum::Xor:
							framebuffer->PixelBlendXor(color, targetX, targetY);
							break;
						default:
						case BlendModeEnum::Replace:
							framebuffer->Pixel(color, targetX, targetY);
							break;
						}
					}

					/// <summary>
					/// Writes a pre-shaded color using the selected blend mode.
					/// Applies the prepared origin offset when addressing the framebuffer.
					/// </summary>
					void PixelBlend(IFrameBuffer* framebuffer, const rgb_color_t color, const dimension_t x, const dimension_t y)
					{
						if (!IsInsideBounds(x, y))
							return;

						const pixel_t targetX = static_cast<pixel_t>(Origin.x) + x;
						const pixel_t targetY = static_cast<pixel_t>(Origin.y) + y;

						switch (BlendMode)
						{
						case BlendModeEnum::BlendHalfAlpha:
							framebuffer->PixelBlend(color, targetX, targetY);
							break;
						case BlendModeEnum::Add:
							framebuffer->PixelBlendAdd(color, targetX, targetY);
							break;
						case BlendModeEnum::Subtract:
							framebuffer->PixelBlendSubtract(color, targetX, targetY);
							break;
						case BlendModeEnum::Multiply:
							framebuffer->PixelBlendMultiply(color, targetX, targetY);
							break;
						case BlendModeEnum::Screen:
							framebuffer->PixelBlendScreen(color, targetX, targetY);
							break;
						case BlendModeEnum::Xor:
							framebuffer->PixelBlendXor(color, targetX, targetY);
							break;
						default:
						case BlendModeEnum::Replace:
							framebuffer->Pixel(color, targetX, targetY);
							break;
						}
					}
				};
			}
		}
	}
}
#endif