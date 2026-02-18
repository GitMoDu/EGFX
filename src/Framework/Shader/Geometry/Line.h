#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_LINE_H
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_LINE_H

#include "../../../Model/IFrameDraw.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				/// <summary>
				/// Geometry shader stage that rasterizes a line primitive on top of a pixel shader.
				/// Uses horizontal/vertical fast paths and falls back to Bresenham for diagonal lines.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader type providing Pixel(...), PixelBlend(...), and the ColorSource/ColorShader/TransformShader members.</typeparam>
				template<typename dimension_t,
					typename PixelShaderType = Shader::Pixel::TemplateShader<dimension_t>
				>
				class LineShader : public PixelShaderType
				{
				private:
					using Base = PixelShaderType;

				protected:
					/// <summary>Intermediate type used by Bresenham drawing.</summary>
					using bresenham_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

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

				protected:
					/// <summary>
					/// Fixed-point scale used by line and triangle rendering.
					/// </summary>
					static constexpr uint8_t BRESENHAM_SCALE = 8;

					// Fixed-point rounding helper: add half-unit for the current Bresenham scale, then arithmetic right-shift.
					static constexpr int16_t FP_ROUND_HALF = SignedLeftShift<int16_t>(1, BRESENHAM_SCALE - 1);

				public:
					LineShader() : Base() {}
					~LineShader() = default;

					/// <summary>
					/// Draws a line between two local-space endpoints.
					/// Per-pixel shading, origin offset, and optional coordinate transforms are provided by the underlying pixel shader.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x1">Start X coordinate (local space).</param>
					/// <param name="y1">Start Y coordinate (local space).</param>
					/// <param name="x2">End X coordinate (local space).</param>
					/// <param name="y2">End Y coordinate (local space).</param>
					void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						if (y1 == y2)
						{
							LineHorizontal(framebuffer, x1, x2, y1, SkipSourceTag{}, SkipTransformTag{});
						}
						else if (x1 == x2)
						{
							LineVertical(framebuffer, x1, y1, y2, SkipSourceTag{}, SkipTransformTag{});
						}
						else
						{
							const bresenham_t dx = AbsValue(static_cast<bresenham_t>(x2) - x1);
							const bresenham_t dy = AbsValue(static_cast<bresenham_t>(y2) - y1);

							// X-major
							if (dx >= dy)
							{
								// Ensure startX <= endX
								if (x1 <= x2)
								{
									BresenhamRight(framebuffer, x1, y1, x2, y2, SkipSourceTag{}, SkipTransformTag{});
								}
								else
								{
									BresenhamRight(framebuffer, x2, y2, x1, y1, SkipSourceTag{}, SkipTransformTag{});
								}
							}
							// Y-major
							else
							{
								// Ensure startY <= endY
								if (y1 <= y2)
								{
									BresenhamUp(framebuffer, x1, y1, x2, y2, SkipSourceTag{}, SkipTransformTag{});
								}
								else
								{
									BresenhamUp(framebuffer, x2, y2, x1, y1, SkipSourceTag{}, SkipTransformTag{});
								}
							}
						}
					}

				protected:
					// Horizontal line dispatch is selected at compile time based on:
					// - whether the color source is constant (SkipSourceTag)
					// - whether the transform shader is NoTransform (SkipTransformTag)
					inline void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);

						for (dimension_t x = startX; x <= endX; x++)
						{
							Base::Pixel(framebuffer, x, y);
						}
					}

					inline void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						framebuffer->LineHorizontal(ColorShader.Shade(ColorSource.Source(0, 0)),
							Base::Origin.x + x1, Base::Origin.x + x2, Base::Origin.y + y);
					}

					inline void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						dimension_t fx, fy;
						for (dimension_t x = startX; x <= endX; x++)
						{
							fx = x;
							fy = y;
							if (TransformShader.Transform(fx, fy))
							{
								PixelBlend(framebuffer, color, fx, fy);
							}
						}
					}

					inline void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);

						for (dimension_t x = startX; x <= endX; x++)
						{
							PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
						}
					}

					// Vertical line dispatch is selected at compile time based on:
					// - whether the color source is constant (SkipSourceTag)
					// - whether the transform shader is NoTransform (SkipTransformTag)
					inline void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);

						for (dimension_t y = startY; y <= endY; y++)
						{
							Base::Pixel(framebuffer, x, y);
						}
					}

					inline void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						framebuffer->LineVertical(ColorShader.Shade(ColorSource.Source(0, 0)),
							Base::Origin.x + x, Base::Origin.y + y1, Base::Origin.y + y2);
					}

					inline void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						dimension_t fx, fy;
						for (dimension_t y = startY; y <= endY; y++)
						{
							fx = x;
							fy = y;
							if (TransformShader.Transform(fx, fy))
							{
								PixelBlend(framebuffer, color, fx, fy);
							}
						}
					}

					inline void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);

						for (dimension_t y = startY; y <= endY; y++)
						{
							PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
						}
					}

				private:
					/// <summary>
					/// X-major Bresenham line where startX is less than or equal to endX.
					/// </summary>
					template<typename SkipSourceT, typename SkipTransformT>
					void BresenhamRight(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						SkipSourceT skipSource, SkipTransformT skipTransform)
					{
						BresenhamRightImpl(framebuffer, startX, startY, endX, endY, skipSource, skipTransform);
					}

					template<typename SkipTransformT>
					void BresenhamRightImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::FalseType, SkipTransformT)
					{
						const bresenham_t scaledWidth = (endX - startX) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endY - startY) << 1;
						const int8_t slopeUnit = (endY >= startY) ? 1 : -1;
						const int8_t slopeSign = (endX >= startX) ? 1 : -1;

						bresenham_t slopeError = slopeMagnitude - (endX - startX);
						dimension_t y = startY;

						for (dimension_t x = startX; x != endX; x += slopeSign)
						{
							BresenhamEmit(framebuffer, x, y, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								y += slopeUnit;
								slopeError -= scaledWidth;
							}
						}

						BresenhamEmit(framebuffer, endX, endY, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});
					}

					template<typename SkipTransformT>
					void BresenhamRightImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::TrueType, SkipTransformT)
					{
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						const bresenham_t scaledWidth = (endX - startX) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endY - startY) << 1;
						const int8_t slopeUnit = (endY >= startY) ? 1 : -1;
						const int8_t slopeSign = (endX >= startX) ? 1 : -1;

						bresenham_t slopeError = slopeMagnitude - (endX - startX);
						dimension_t y = startY;

						for (dimension_t x = startX; x != endX; x += slopeSign)
						{
							BresenhamEmit(framebuffer, x, y, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								y += slopeUnit;
								slopeError -= scaledWidth;
							}
						}

						BresenhamEmit(framebuffer, endX, endY, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});
					}

					/// <summary>
					/// Y-major Bresenham line where startY is less than or equal to endY.
					/// </summary>
					template<typename SkipSourceT, typename SkipTransformT>
					void BresenhamUp(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						SkipSourceT skipSource, SkipTransformT skipTransform)
					{
						BresenhamUpImpl(framebuffer, startX, startY, endX, endY, skipSource, skipTransform);
					}

					template<typename SkipTransformT>
					void BresenhamUpImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::FalseType, SkipTransformT)
					{
						const bresenham_t scaledHeight = (endY - startY) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endX - startX) << 1;
						const int8_t slopeUnit = (endX >= startX) ? 1 : -1;
						const int8_t slopeSign = (endY >= startY) ? 1 : -1;

						bresenham_t slopeError = (bresenham_t)slopeMagnitude - (endY - startY);
						dimension_t x = startX;

						for (dimension_t y = startY; y != endY; y += slopeSign)
						{
							BresenhamEmit(framebuffer, x, y, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								x += slopeUnit;
								slopeError -= scaledHeight;
							}
						}

						BresenhamEmit(framebuffer, endX, endY, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});
					}

					template<typename SkipTransformT>
					void BresenhamUpImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::TrueType, SkipTransformT)
					{
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						const bresenham_t scaledHeight = (endY - startY) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endX - startX) << 1;
						const int8_t slopeUnit = (endX >= startX) ? 1 : -1;
						const int8_t slopeSign = (endY >= startY) ? 1 : -1;

						bresenham_t slopeError = (bresenham_t)slopeMagnitude - (endY - startY);
						dimension_t x = startX;

						for (dimension_t y = startY; y != endY; y += slopeSign)
						{
							BresenhamEmit(framebuffer, x, y, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								x += slopeUnit;
								slopeError -= scaledHeight;
							}
						}

						BresenhamEmit(framebuffer, endX, endY, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});
					}

				private:
					// SkipSource=False, SkipTransform=False: full pixel shader path.
					inline void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						Base::Pixel(framebuffer, x, y);
					}

					// SkipSource=False, SkipTransform=True: shade per pixel, no transform.
					inline void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
					}

					// SkipSource=True, SkipTransform=False: constant shade once, transform per pixel.
					inline void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						const rgb_color_t color,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						dimension_t fx = x;
						dimension_t fy = y;
						if (TransformShader.Transform(fx, fy))
						{
							PixelBlend(framebuffer, color, fx, fy);
						}
					}

					// SkipSource=True, SkipTransform=True: constant shade once, no transform.
					inline void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						const rgb_color_t color,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlend(framebuffer, color, x, y);
					}
				};
			}
		}
	}
}
#endif