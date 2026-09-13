#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_TRIANGLE_H
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_TRIANGLE_H

#include "Rectangle.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				/// <summary>
				/// Geometry shader stage that rasterizes a filled triangle on top of a rectangle/line/pixel shader pipeline.
				/// Uses scanline filling with fixed-point edge stepping and supports optional overdraw control.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader type providing Pixel(...), PixelBlend(...), and the ColorSource/ColorShader/TransformShader members.</typeparam>
				/// <typeparam name="UseOverdraw">Enables inclusive edge coverage when true; applies the top-left rule when false.</typeparam>
				template<typename dimension_t,
					typename PixelShaderType = Shader::Pixel::TemplateShader<dimension_t>,
					bool UseOverdraw = false
				>
				class TriangleShader : public RectangleShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = RectangleShader<dimension_t, PixelShaderType>;

				private:
					using signed_t = typename AutoDimension::ByDimension<dimension_t>::signed_t;
					using bresenham_t = typename AutoDimension::ByDimension<dimension_t>::signed_wide_t;

				private:
					// Extract shader types for compile-time optimizations.
					using ColorSourceType = typename PixelShaderType::color_source_t;
					using TransformShaderType = typename PixelShaderType::transform_shader_t;

				private:
					// Compile-time dispatch: skip per-pixel source sampling only when the source is constant
					// and the selected blend mode is compatible with replacement-only bulk operations.
					using SkipSourceTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Shader::Source::ConstantColorFlag<ColorSourceType>::value && PixelShaderType::IsReplaceBlendMode>::type;

					// Compile-time dispatch: skip Transform(...) when the transform shader is NoTransform.
					using SkipTransformTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						IntegerSignal::TypeTraits::TypeDispatch::is_same<TransformShaderType, Shader::Transform::NoTransform<dimension_t>>::value>::type;

					// Compile-time dispatch: include shared edges when overdraw is enabled.
					using OverdrawTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						UseOverdraw>::type;

				protected:
					/// <summary>
					/// Fixed-point scale used by line and triangle rendering.
					/// </summary>
					static constexpr uint8_t BRESENHAM_SCALE = 8;

					// Fixed-point rounding helper: add half-unit for the current Bresenham scale, then arithmetic right-shift.
					static constexpr bresenham_t FP_ROUND_HALF = SignedLeftShift<bresenham_t>(1, BRESENHAM_SCALE - 1);

				public:
					TriangleShader(const dimension_t left, const dimension_t top,
						const dimension_t right, const dimension_t bottom)
						: Base(left, top, right, bottom) {}
					~TriangleShader() = default;

					/// <summary>
					/// Fills a triangle defined by three vertices in local space.
					/// Sorts vertices by Y, splits vertices into top and bottom segments, then emits horizontal scanlines using the underlying line shader.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="x1">Vertex 1 X coordinate (local space).</param>
					/// <param name="y1">Vertex 1 Y coordinate (local space).</param>
					/// <param name="x2">Vertex 2 X coordinate (local space).</param>
					/// <param name="y2">Vertex 2 Y coordinate (local space).</param>
					/// <param name="x3">Vertex 3 X coordinate (local space).</param>
					/// <param name="y3">Vertex 3 Y coordinate (local space).</param>
					void TriangleFill(IFrameBuffer* framebuffer,
						const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const dimension_t x3, const dimension_t y3)
					{
						// Sort vertices by Y (and reorder X accordingly).
						signed_t ax = static_cast<signed_t>(x1);
						signed_t ay = static_cast<signed_t>(y1);
						signed_t bx = static_cast<signed_t>(x2);
						signed_t by = static_cast<signed_t>(y2);
						signed_t cx = static_cast<signed_t>(x3);
						signed_t cy = static_cast<signed_t>(y3);

						{
							signed_t tmp;
							if (ay > by)
							{
								tmp = ax; ax = bx; bx = tmp;
								tmp = ay; ay = by; by = tmp;
							}
							if (by > cy)
							{
								tmp = bx; bx = cx; cx = tmp;
								tmp = by; by = cy; cy = tmp;
							}
							if (ay > by)
							{
								tmp = ax; ax = bx; bx = tmp;
								tmp = ay; ay = by; by = tmp;
							}
						}

						// Degenerate (single scanline).
						if (ay == cy)
						{
							const dimension_t xStart = static_cast<dimension_t>(MinValue(ax, MinValue(bx, cx)));
							const dimension_t xEnd = static_cast<dimension_t>(MaxValue(ax, MaxValue(bx, cx)));
							Base::Line(framebuffer, xStart, static_cast<dimension_t>(ay), xEnd, static_cast<dimension_t>(ay));
							return;
						}

						const signed_t hTop = by - ay;
						const signed_t hBottom = cy - by;
						const signed_t hTotal = cy - ay;

						const bresenham_t dxLong = (hTotal != 0) ? (IntToFixed((cx - ax)) / hTotal) : 0;
						const bresenham_t dxTop = (hTop != 0) ? (IntToFixed((bx - ax)) / hTop) : 0;
						const bresenham_t dxBottom = (hBottom != 0) ? (IntToFixed((cx - bx)) / hBottom) : 0;

						const bresenham_t fxA = IntToFixed(ax);
						const bresenham_t fxB = IntToFixed(bx);

						// Determine if the long edge is left of the top edge at y = by.
						const bool longEdgeIsLeft = ((fxA + dxLong * hTop) <= fxB);

						// Top segment [ay, by) unless it is the only segment and overdraw is enabled.
						if (hTop > 0)
						{
							const bresenham_t stepLeft = longEdgeIsLeft ? dxLong : dxTop;
							const bresenham_t stepRight = longEdgeIsLeft ? dxTop : dxLong;

							if (hBottom > 0)
							{
								FillTriangleSegment(framebuffer,
									ay, by,
									fxA, fxA,
									stepLeft, stepRight,
									OverdrawTag{},
									TypeTraits::TypeDispatch::FalseType{});
							}
							else
							{
								FillTriangleSegment(framebuffer,
									ay, by,
									fxA, fxA,
									stepLeft, stepRight,
									OverdrawTag{},
									TypeTraits::TypeDispatch::TrueType{});
							}
						}

						// Bottom segment [by, cy) or [by, cy] when overdraw is enabled.
						if (hBottom > 0)
						{
							const bresenham_t stepLeft = longEdgeIsLeft ? dxLong : dxBottom;
							const bresenham_t stepRight = longEdgeIsLeft ? dxBottom : dxLong;

							const bresenham_t fxLongAtB = fxA + dxLong * hTop;

							const bresenham_t fxLeft = longEdgeIsLeft ? fxLongAtB : fxB;
							const bresenham_t fxRight = longEdgeIsLeft ? fxB : fxLongAtB;

							FillTriangleSegment(framebuffer,
								by, cy,
								fxLeft, fxRight,
								stepLeft, stepRight,
								OverdrawTag{},
								TypeTraits::TypeDispatch::TrueType{});
						}
					}

				private:
					/// <summary>
					/// Fills a horizontal segment of a triangle by drawing scanlines between interpolated left and right edges.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="yStart">Starting y-coordinate (inclusive) of the segment to fill.</param>
					/// <param name="yEnd">Ending y-coordinate of the segment to fill.</param>
					/// <param name="fxLeft">Fixed-point x-coordinate of the left edge at yStart.</param>
					/// <param name="fxRight">Fixed-point x-coordinate of the right edge at yStart.</param>
					/// <param name="stepLeft">Fixed-point increment of the left edge per scanline.</param>
					/// <param name="stepRight">Fixed-point increment of the right edge per scanline.</param>
					void FillTriangleSegment(IFrameBuffer* framebuffer,
						signed_t yStart,
						signed_t yEnd,
						bresenham_t fxLeft,
						bresenham_t fxRight,
						const bresenham_t stepLeft,
						const bresenham_t stepRight,
						TypeTraits::TypeDispatch::FalseType,
						TypeTraits::TypeDispatch::FalseType)
					{
						yEnd = MaxValue(yStart, yEnd);

						for (signed_t y = yStart; y < yEnd; y++)
						{
							const signed_t startX = FixedRoundToInt(fxLeft);
							const signed_t endX = FixedRoundToInt(fxRight) - 1;

							if (startX <= endX)
							{
								Base::LineHorizontal(framebuffer,
									startX,
									endX,
									y, SkipSourceTag{}, SkipTransformTag{});
							}

							fxLeft += stepLeft;
							fxRight += stepRight;
						}
					}

					void FillTriangleSegment(IFrameBuffer* framebuffer,
						signed_t yStart,
						signed_t yEnd,
						bresenham_t fxLeft,
						bresenham_t fxRight,
						const bresenham_t stepLeft,
						const bresenham_t stepRight,
						TypeTraits::TypeDispatch::FalseType,
						TypeTraits::TypeDispatch::TrueType)
					{
						FillTriangleSegment(framebuffer,
							yStart, yEnd,
							fxLeft, fxRight,
							stepLeft, stepRight,
							TypeTraits::TypeDispatch::FalseType{},
							TypeTraits::TypeDispatch::FalseType{});
					}

					void FillTriangleSegment(IFrameBuffer* framebuffer,
						signed_t yStart,
						signed_t yEnd,
						bresenham_t fxLeft,
						bresenham_t fxRight,
						const bresenham_t stepLeft,
						const bresenham_t stepRight,
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType)
					{
						yEnd = MaxValue(yStart, yEnd);

						for (signed_t y = yStart; y < yEnd; y++)
						{
							const signed_t startX = FixedRoundToInt(fxLeft);
							const signed_t endX = FixedRoundToInt(fxRight);

							if (startX <= endX)
							{
								Base::LineHorizontal(framebuffer,
									startX,
									endX,
									y, SkipSourceTag{}, SkipTransformTag{});
							}

							fxLeft += stepLeft;
							fxRight += stepRight;
						}
					}

					void FillTriangleSegment(IFrameBuffer* framebuffer,
						signed_t yStart,
						signed_t yEnd,
						bresenham_t fxLeft,
						bresenham_t fxRight,
						const signed_t stepLeft,
						const signed_t stepRight,
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::TrueType)
					{
						if (yStart > yEnd)
						{
							return;
						}

						for (signed_t y = yStart; y <= yEnd; y++)
						{
							const signed_t startX = FixedRoundToInt(fxLeft);
							const signed_t endX = FixedRoundToInt(fxRight);

							if (startX <= endX)
							{
								Base::LineHorizontal(framebuffer,
									startX,
									endX,
									y, SkipSourceTag{}, SkipTransformTag{});
							}

							fxLeft += stepLeft;
							fxRight += stepRight;
						}
					}

				private:
					/// <summary>
					/// Rounds a fixed-point value to the nearest integer using a signed right shift.
					/// </summary>
					static constexpr signed_t FixedRoundToInt(const bresenham_t fx)
					{
						return SignedRightShift(fx + FP_ROUND_HALF, BRESENHAM_SCALE);
					}

					/// <summary>
					/// Converts an integer pixel coordinate to fixed-point representation.
					/// </summary>
					static constexpr bresenham_t IntToFixed(const signed_t x)
					{
						return SignedLeftShift<bresenham_t>(static_cast<bresenham_t>(x), BRESENHAM_SCALE);
					}
				};
			}
		}
	}
}
#endif