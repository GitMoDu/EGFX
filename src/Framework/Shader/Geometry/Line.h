#ifndef _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_LINE_H
#define _INTEGERGLASS_FRAMEWORK_SHADER_GEOMETRY_LINE_H

#include "Model.h"

namespace IntegerGlass
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
					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
					using bresenham_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

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
					// Compile-time dispatch: skip per-pixel source sampling only when the source is constant
					// and the selected blend mode is compatible with replacement-only bulk operations.
					using SkipSourceTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Shader::Source::ConstantColorFlag<ColorSourceType>::value&& PixelShaderType::IsReplaceBlendMode>::type;

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
					static constexpr signed_t FP_ROUND_HALF = SignedLeftShift<signed_t>(1, BRESENHAM_SCALE - 1);

				public:
					LineShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
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
					/// <param name="boundaryMode">Controls inclusion of the start and end endpoints.</param>
					void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const LineBoundaryModeEnum boundaryMode = LineBoundaryModeEnum::Inclusive)
					{
						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;

						if (y1 == y2)
						{
							if (x1 == x2)
							{
								if (includeStart && includeEnd)
									LineHorizontal(framebuffer, x1, x2, y1, SkipSourceTag{}, SkipTransformTag{});
							}
							else
							{
								const dimension_t start = includeStart ? x1 : (x2 > x1 ? static_cast<dimension_t>(x1 + 1) : static_cast<dimension_t>(x1 - 1));
								const dimension_t end = includeEnd ? x2 : (x2 > x1 ? static_cast<dimension_t>(x2 - 1) : static_cast<dimension_t>(x2 + 1));
								if (x2 > x1 ? start <= end : start >= end)
									LineHorizontal(framebuffer, start, end, y1, SkipSourceTag{}, SkipTransformTag{});
							}
						}

						else if (x1 == x2)
						{
							if (y1 == y2)
							{
								if (includeStart && includeEnd)
									LineVertical(framebuffer, x1, y1, y2, SkipSourceTag{}, SkipTransformTag{});
							}
							else
							{
								const dimension_t start = includeStart ? y1 : (y2 > y1 ? static_cast<dimension_t>(y1 + 1) : static_cast<dimension_t>(y1 - 1));
								const dimension_t end = includeEnd ? y2 : (y2 > y1 ? static_cast<dimension_t>(y2 - 1) : static_cast<dimension_t>(y2 + 1));
								if (y2 > y1 ? start <= end : start >= end)
									LineVertical(framebuffer, x1, start, end, SkipSourceTag{}, SkipTransformTag{});
							}
						}
						else
						{
							const signed_t dx = AbsValue(static_cast<signed_t>(x2) - static_cast<signed_t>(x1));
							const signed_t dy = AbsValue(static_cast<signed_t>(y2) - static_cast<signed_t>(y1));

							// X-major
							if (dx >= dy)
							{
								// Ensure startX <= endX
								if (x1 <= x2)
								{
									BresenhamRight(framebuffer, x1, y1, x2, y2, SkipSourceTag{}, SkipTransformTag{}, boundaryMode);
								}
								else
								{
									BresenhamRight(framebuffer, x2, y2, x1, y1, SkipSourceTag{}, SkipTransformTag{}, ReverseBoundaryMode(boundaryMode));
								}
							}
							// Y-major
							else
							{
								// Ensure startY <= endY
								if (y1 <= y2)
								{
									BresenhamUp(framebuffer, x1, y1, x2, y2, SkipSourceTag{}, SkipTransformTag{}, boundaryMode);
								}
								else
								{
									BresenhamUp(framebuffer, x2, y2, x1, y1, SkipSourceTag{}, SkipTransformTag{}, ReverseBoundaryMode(boundaryMode));
								}
							}
						}
					}

					/// <summary>
					/// Draws a centered runtime-width stroke between two local-space endpoints.
					/// Width one uses the regular line rasterizer; wider strokes reuse it for parallel lines.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer.</param>
					/// <param name="x1">Start X coordinate (local space).</param>
					/// <param name="y1">Start Y coordinate (local space).</param>
					/// <param name="x2">End X coordinate (local space).</param>
					/// <param name="y2">End Y coordinate (local space).</param>
					/// <param name="strokeWidth">Runtime stroke width in pixels.</param>
					/// <param name="boundaryMode">Controls inclusion of the stroke centerline endpoints.</param>
					void Stroke(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2, const dimension_t strokeWidth,
						const LineBoundaryModeEnum boundaryMode = LineBoundaryModeEnum::Inclusive)
					{
						if (strokeWidth == 0)
							return;

						if (strokeWidth == 1)
						{
							Line(framebuffer, x1, y1, x2, y2, boundaryMode);
							return;
						}

						const bresenham_t dx = AbsValue(static_cast<bresenham_t>(x2) - static_cast<bresenham_t>(x1));
						const bresenham_t dy = AbsValue(static_cast<bresenham_t>(y2) - static_cast<bresenham_t>(y1));
						const bresenham_t width = static_cast<bresenham_t>(strokeWidth);
						const bresenham_t halfWidth = width / 2;

						for (bresenham_t index = 0; index < width; ++index)
						{
							const bresenham_t offset = index - halfWidth;
							StrokeOffset(framebuffer, x1, y1, x2, y2, offset, dx >= dy, boundaryMode);
						}
					}

					/// <summary>
					/// Draws a line without applying the pixel shader clip.
					/// The caller is responsible for ensuring that the complete primitive is already clipped.
					/// </summary>
					void LineUnclipped(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const LineBoundaryModeEnum boundaryMode = LineBoundaryModeEnum::Inclusive)
					{
						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;

						const signed_t dx = AbsValue(static_cast<signed_t>(x2) - static_cast<signed_t>(x1));
						const signed_t dy = AbsValue(static_cast<signed_t>(y2) - static_cast<signed_t>(y1));

						if (dx >= dy)
						{
							const dimension_t startX = x1 <= x2 ? x1 : x2;
							const dimension_t startY = x1 <= x2 ? y1 : y2;
							const dimension_t endX = x1 <= x2 ? x2 : x1;
							const dimension_t endY = x1 <= x2 ? y2 : y1;
							const signed_t slopeUnit = endY >= startY ? 1 : -1;
							signed_t y = static_cast<signed_t>(startY);
							signed_t error = (dy << 1) - dx;

							for (dimension_t x = startX; x < endX; ++x)
							{
								if (includeStart || x != startX)
									Base::PixelUnclipped(framebuffer, x, static_cast<dimension_t>(y));
								error += dy << 1;
								if (error >= 0)
								{
									y += slopeUnit;
									error -= dx << 1;
								}
							}

							if (includeEnd)
								Base::PixelUnclipped(framebuffer, endX, endY);
						}
						else
						{
							const dimension_t startX = y1 <= y2 ? x1 : x2;
							const dimension_t startY = y1 <= y2 ? y1 : y2;
							const dimension_t endX = y1 <= y2 ? x2 : x1;
							const dimension_t endY = y1 <= y2 ? y2 : y1;
							const signed_t slopeUnit = endX >= startX ? 1 : -1;
							signed_t x = static_cast<signed_t>(startX);
							signed_t error = (dx << 1) - dy;

							for (dimension_t y = startY; y < endY; ++y)
							{
								if (includeStart || y != startY)
									Base::PixelUnclipped(framebuffer, static_cast<dimension_t>(x), y);
								error += dx << 1;
								if (error >= 0)
								{
									x += slopeUnit;
									error -= dy << 1;
								}
							}

							if (includeEnd)
								Base::PixelUnclipped(framebuffer, endX, endY);
						}
					}

					void LineHorizontalUnclipped(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);

						for (dimension_t x = startX; x <= endX; ++x)
							Base::PixelUnclipped(framebuffer, x, y);
					}

					void LineVerticalUnclipped(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);

						for (dimension_t y = startY; y <= endY; ++y)
							Base::PixelUnclipped(framebuffer, x, y);
					}

				protected:
					void StrokeOffset(IFrameBuffer* framebuffer,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const bresenham_t offset, const bool horizontalOffset,
						const LineBoundaryModeEnum boundaryMode)
					{
						const bresenham_t maximum = static_cast<bresenham_t>(
							IntegerSignal::TypeTraits::TypeLimits::type_limits<dimension_t>::Max());

						if (horizontalOffset)
						{
							const bresenham_t offsetY1 = static_cast<bresenham_t>(y1) + offset;
							const bresenham_t offsetY2 = static_cast<bresenham_t>(y2) + offset;
							if (offsetY1 < 0 || offsetY1 > maximum || offsetY2 < 0 || offsetY2 > maximum)
								return;

							Line(framebuffer, x1, static_cast<dimension_t>(offsetY1),
								x2, static_cast<dimension_t>(offsetY2), boundaryMode);
							return;
						}

						const bresenham_t offsetX1 = static_cast<bresenham_t>(x1) + offset;
						const bresenham_t offsetX2 = static_cast<bresenham_t>(x2) + offset;
						if (offsetX1 < 0 || offsetX1 > maximum || offsetX2 < 0 || offsetX2 > maximum)
							return;

						Line(framebuffer, static_cast<dimension_t>(offsetX1), y1,
							static_cast<dimension_t>(offsetX2), y2, boundaryMode);
					}

					static LineBoundaryModeEnum ReverseBoundaryMode(const LineBoundaryModeEnum boundaryMode)
					{
						switch (boundaryMode)
						{
						case LineBoundaryModeEnum::ExcludeStart:
							return LineBoundaryModeEnum::ExcludeEnd;
						case LineBoundaryModeEnum::ExcludeEnd:
							return LineBoundaryModeEnum::ExcludeStart;
						default:
							return boundaryMode;
						}
					}

					// Horizontal line dispatch is selected at compile time based on:
					// - whether the color source is constant (SkipSourceTag)
					// - whether the transform shader is NoTransform (SkipTransformTag)
					void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);

						dimension_t x = startX;
						while (true)
						{
							Base::Pixel(framebuffer, x, y);

							if (x == endX)
							{
								break;
							}
							x++;
						}
					}

					void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						if (y < Base::GetBoundsTop() || y >= Base::GetBoundsBottom())
							return;

						const dimension_t startX = MaxValue(MinValue(x1, x2), Base::GetBoundsLeft());
						const dimension_t endX = MinValue(MaxValue(x1, x2), static_cast<dimension_t>(Base::GetBoundsRight() - 1));
						if (startX > endX)
							return;

						framebuffer->LineHorizontal(ColorShader.Shade(ColorSource.Source(0, 0)),
							static_cast<pixel_t>(Base::Origin.x + static_cast<pixel_t>(startX)),
							static_cast<pixel_t>(Base::Origin.x + static_cast<pixel_t>(endX)),
							static_cast<pixel_t>(Base::Origin.y + static_cast<pixel_t>(y)));
					}

					void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						dimension_t x = startX;
						dimension_t fx, fy;
						while (true)
						{
							fx = x;
							fy = y;
							if (TransformShader.Transform(fx, fy))
							{
								PixelBlend(framebuffer, color, fx, fy);
							}

							if (x == endX)
							{
								break;
							}
							x++;
						}
					}

					void LineHorizontal(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t x2, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						if (y < Base::GetBoundsTop() || y >= Base::GetBoundsBottom())
							return;

						const dimension_t startX = MaxValue(MinValue(x1, x2), Base::GetBoundsLeft());
						const dimension_t endX = MinValue(MaxValue(x1, x2), static_cast<dimension_t>(Base::GetBoundsRight() - 1));
						if (startX > endX)
							return;

						dimension_t x = startX;
						while (true)
						{
							Base::PixelUnclipped(framebuffer, x, y);

							if (x == endX)
							{
								break;
							}
							x++;
						}
					}

					// Vertical line dispatch is selected at compile time based on:
					// - whether the color source is constant (SkipSourceTag)
					// - whether the transform shader is NoTransform (SkipTransformTag)
					void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);

						dimension_t y = startY;
						while (true)
						{
							Base::Pixel(framebuffer, x, y);

							if (y == endY)
							{
								break;
							}
							y++;
						}
					}

					void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::TrueType)
					{
						if (x < Base::GetBoundsLeft() || x >= Base::GetBoundsRight())
							return;

						const dimension_t startY = MaxValue(MinValue(y1, y2), Base::GetBoundsTop());
						const dimension_t endY = MinValue(MaxValue(y1, y2), static_cast<dimension_t>(Base::GetBoundsBottom() - 1));
						if (startY > endY)
							return;

						framebuffer->LineVertical(ColorShader.Shade(ColorSource.Source(0, 0)),
							static_cast<pixel_t>(Base::Origin.x + static_cast<pixel_t>(x)),
							static_cast<pixel_t>(Base::Origin.y + static_cast<pixel_t>(startY)),
							static_cast<pixel_t>(Base::Origin.y + static_cast<pixel_t>(endY)));
					}

					void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::TrueType, TypeTraits::TypeDispatch::FalseType)
					{
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						dimension_t y = startY;
						dimension_t fx, fy;
						while (true)
						{
							fx = x;
							fy = y;
							if (TransformShader.Transform(fx, fy))
							{
								PixelBlend(framebuffer, color, fx, fy);
							}

							if (y == endY)
							{
								break;
							}
							y++;
						}
					}

					void LineVertical(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y1, const dimension_t y2,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						if (x < Base::GetBoundsLeft() || x >= Base::GetBoundsRight())
							return;

						const dimension_t startY = MaxValue(MinValue(y1, y2), Base::GetBoundsTop());
						const dimension_t endY = MinValue(MaxValue(y1, y2), static_cast<dimension_t>(Base::GetBoundsBottom() - 1));
						if (startY > endY)
							return;

						dimension_t y = startY;
						while (true)
						{
							Base::PixelUnclipped(framebuffer, x, y);

							if (y == endY)
							{
								break;
							}
							y++;
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
						SkipSourceT skipSource, SkipTransformT skipTransform,
						const LineBoundaryModeEnum boundaryMode)
					{
						BresenhamRightImpl(framebuffer, startX, startY, endX, endY, skipSource, skipTransform, boundaryMode);
					}

					template<typename SkipTransformT>
					void BresenhamRightImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::FalseType, SkipTransformT,
						const LineBoundaryModeEnum boundaryMode)
					{
						const bresenham_t dx = static_cast<bresenham_t>(endX) - static_cast<bresenham_t>(startX);
						const bresenham_t dy = AbsValue(static_cast<bresenham_t>(endY) - static_cast<bresenham_t>(startY));

						const int8_t slopeUnit = (endY >= startY) ? 1 : -1;

						bresenham_t slopeError = SignedRightShift(dx, 1);
						signed_t y = static_cast<signed_t>(startY);

						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						for (dimension_t x = startX; x < endX; x++)
						{
							if (includeStart || x != startX)
								BresenhamEmit(framebuffer, x, static_cast<dimension_t>(y), TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});

							slopeError -= dy;
							if (slopeError < 0)
							{
								y += slopeUnit;
								slopeError += dx;
							}
						}

						if (boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd)
						{
							BresenhamEmit(framebuffer, endX, endY, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});
						}
					}

					template<typename SkipTransformT>
					void BresenhamRightImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::TrueType, SkipTransformT,
						const LineBoundaryModeEnum boundaryMode)
					{
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						const bresenham_t dx = static_cast<bresenham_t>(endX) - static_cast<bresenham_t>(startX);
						const bresenham_t dy = AbsValue(static_cast<bresenham_t>(endY) - static_cast<bresenham_t>(startY));

						const int8_t slopeUnit = (endY >= startY) ? 1 : -1;

						bresenham_t slopeError = SignedRightShift(dx, 1);
						signed_t y = static_cast<signed_t>(startY);

						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						for (dimension_t x = startX; x < endX; x++)
						{
							if (includeStart || x != startX)
								BresenhamEmit(framebuffer, x, static_cast<dimension_t>(y), color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});

							slopeError -= dy;
							if (slopeError < 0)
							{
								y += slopeUnit;
								slopeError += dx;
							}
						}

						if (boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd)
						{
							BresenhamEmit(framebuffer, endX, endY, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});
						}
					}

					/// <summary>
					/// Y-major Bresenham line where startY is less than or equal to endY.
					/// </summary>
					template<typename SkipSourceT, typename SkipTransformT>
					void BresenhamUp(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						SkipSourceT skipSource, SkipTransformT skipTransform,
						const LineBoundaryModeEnum boundaryMode)
					{
						BresenhamUpImpl(framebuffer, startX, startY, endX, endY, skipSource, skipTransform, boundaryMode);
					}

					template<typename SkipTransformT>
					void BresenhamUpImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::FalseType, SkipTransformT,
						const LineBoundaryModeEnum boundaryMode)
					{
						const bresenham_t dy = static_cast<bresenham_t>(endY) - static_cast<bresenham_t>(startY);
						const bresenham_t dx = AbsValue(static_cast<bresenham_t>(endX) - static_cast<bresenham_t>(startX));

						const int8_t slopeUnit = (endX >= startX) ? 1 : -1;

						bresenham_t slopeError = SignedRightShift(dy, 1);
						signed_t x = static_cast<signed_t>(startX);

						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						for (dimension_t y = startY; y < endY; y++)
						{
							if (includeStart || y != startY)
								BresenhamEmit(framebuffer, static_cast<dimension_t>(x), y, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});

							slopeError -= dx;
							if (slopeError < 0)
							{
								x += slopeUnit;
								slopeError += dy;
							}
						}

						if (boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd)
						{
							BresenhamEmit(framebuffer, endX, endY, TypeTraits::TypeDispatch::FalseType{}, SkipTransformT{});
						}
					}

					template<typename SkipTransformT>
					void BresenhamUpImpl(IFrameBuffer* framebuffer,
						const dimension_t startX, const dimension_t startY,
						const dimension_t endX, const dimension_t endY,
						TypeTraits::TypeDispatch::TrueType, SkipTransformT,
						const LineBoundaryModeEnum boundaryMode)
					{
						const rgb_color_t color = ColorShader.Shade(ColorSource.Source(0, 0));

						const bresenham_t dy = static_cast<bresenham_t>(endY) - static_cast<bresenham_t>(startY);
						const bresenham_t dx = AbsValue(static_cast<bresenham_t>(endX) - static_cast<bresenham_t>(startX));

						const int8_t slopeUnit = (endX >= startX) ? 1 : -1;

						bresenham_t slopeError = SignedRightShift(dy, 1);
						signed_t x = static_cast<signed_t>(startX);

						const bool includeStart = boundaryMode != LineBoundaryModeEnum::ExcludeStart &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						const bool includeEnd = boundaryMode != LineBoundaryModeEnum::ExcludeEnd &&
							boundaryMode != LineBoundaryModeEnum::ExcludeStartAndEnd;
						for (dimension_t y = startY; y < endY; y++)
						{
							if (includeStart || y != startY)
								BresenhamEmit(framebuffer, static_cast<dimension_t>(x), y, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});

							slopeError -= dx;
							if (slopeError < 0)
							{
								x += slopeUnit;
								slopeError += dy;
							}
						}

						if (includeEnd)
						{
							BresenhamEmit(framebuffer, endX, endY, color, TypeTraits::TypeDispatch::TrueType{}, SkipTransformT{});
						}
					}

				private:
					// SkipSource=False, SkipTransform=False: full pixel shader path.
					void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::FalseType)
					{
						Base::Pixel(framebuffer, x, y);
					}

					// SkipSource=False, SkipTransform=True: shade per pixel, no transform.
					void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
						TypeTraits::TypeDispatch::FalseType, TypeTraits::TypeDispatch::TrueType)
					{
						PixelBlend(framebuffer, ColorShader.Shade(ColorSource.Source(x, y)), x, y);
					}

					// SkipSource=True, SkipTransform=False: constant shade once, transform per pixel.
					void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
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
					void BresenhamEmit(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y,
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