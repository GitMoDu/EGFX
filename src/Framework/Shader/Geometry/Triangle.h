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
				/// Uses scanline filling with fixed-point edge stepping and applies a top-left rule to avoid overlaps between adjacent triangles.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader type providing Pixel(...), PixelBlend(...), and the ColorSource/ColorShader/TransformShader members.</typeparam>
				template<typename dimension_t,
					typename PixelShaderType = Shader::Pixel::TemplateShader<dimension_t>
				>
				class TriangleShader : public RectangleShader<dimension_t, PixelShaderType>
				{
				private:
					using Base = RectangleShader<dimension_t, PixelShaderType>;

				protected:
					/// <summary>Intermediate type used by Bresenham drawing.</summary>
					using bresenham_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

				private:
					// Extract shader types for compile-time optimizations.
					using ColorSourceType = typename PixelShaderType::color_source_t;
					using TransformShaderType = typename PixelShaderType::transform_shader_t;

				private:
					// Compile-time dispatch: skip per-pixel source sampling when the source is constant.
					using SkipSourceTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Shader::Source::ConstantColorFlag<ColorSourceType>::value>::type;

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
					TriangleShader() : Base() {}
					~TriangleShader() = default;

					/// <summary>
					/// Fills a triangle defined by three vertices in local space.
					/// Sorts vertices by Y, splits into a top and bottom segment, then emits horizontal scanlines using the underlying line shader.
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
						dimension_t ax = x1, ay = y1;
						dimension_t bx = x2, by = y2;
						dimension_t cx = x3, cy = y3;

						{
							dimension_t tmp;
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
							const dimension_t xStart = MinValue(ax, MinValue(bx, cx));
							const dimension_t xEnd = MaxValue(ax, MaxValue(bx, cx));
							Base::Line(framebuffer, xStart, ay, xEnd, ay);
							return;
						}

						const dimension_t hTop = by - ay;
						const dimension_t hBottom = cy - by;
						const dimension_t hTotal = cy - ay;

						const bresenham_t fxA = IntToFixed(ax);
						const bresenham_t fxB = IntToFixed(bx);

						const bresenham_t dxLong = (hTotal != 0) ? (IntToFixed(cx - ax) / hTotal) : 0;
						const bresenham_t dxTop = (hTop != 0) ? (IntToFixed(bx - ax) / hTop) : 0;
						const bresenham_t dxBottom = (hBottom != 0) ? (IntToFixed(cx - bx) / hBottom) : 0;

						// Determine if the long edge is left of the top edge at y = by.
						const bool longEdgeIsLeft = ((fxA + dxLong * hTop) <= fxB);

						// Top segment [ay, by)
						if (hTop > 0)
						{
							const bresenham_t stepLeft = longEdgeIsLeft ? dxLong : dxTop;
							const bresenham_t stepRight = longEdgeIsLeft ? dxTop : dxLong;

							FillTriangleSegment(framebuffer, ay, by, fxA, fxA, stepLeft, stepRight);
						}

						// Bottom segment [by, cy)
						if (hBottom > 0)
						{
							const bresenham_t stepLeft = longEdgeIsLeft ? dxLong : dxBottom;
							const bresenham_t stepRight = longEdgeIsLeft ? dxBottom : dxLong;

							const bresenham_t fxLongAtB = fxA + dxLong * hTop;

							const bresenham_t fxLeft = longEdgeIsLeft ? fxLongAtB : fxB;
							const bresenham_t fxRight = longEdgeIsLeft ? fxB : fxLongAtB;

							FillTriangleSegment(framebuffer, by, cy, fxLeft, fxRight, stepLeft, stepRight);
						}
					}

				private:
					/// <summary>
					/// Rounds a fixed-point value to the nearest integer using a signed right shift.
					/// </summary>
					inline constexpr dimension_t FixedRoundToInt(const bresenham_t fx)
					{
						return SignedRightShift(fx + FP_ROUND_HALF, BRESENHAM_SCALE);
					}

					/// <summary>
					/// Converts an integer pixel coordinate to fixed-point representation.
					/// </summary>
					inline constexpr bresenham_t IntToFixed(const dimension_t x)
					{
						return SignedLeftShift<bresenham_t>(x, BRESENHAM_SCALE);
					}

				private:
					/// <summary>
					/// Fills a horizontal segment of a triangle by drawing scanlines between interpolated left and right edges.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="yStart">Starting y-coordinate (inclusive) of the segment to fill.</param>
					/// <param name="yEnd">Ending y-coordinate (exclusive) of the segment to fill.</param>
					/// <param name="fxLeft">Fixed-point x-coordinate of the left edge at yStart.</param>
					/// <param name="fxRight">Fixed-point x-coordinate of the right edge at yStart.</param>
					/// <param name="stepLeft">Fixed-point increment of the left edge per scanline.</param>
					/// <param name="stepRight">Fixed-point increment of the right edge per scanline.</param>
					inline void FillTriangleSegment(IFrameBuffer* framebuffer,
						const dimension_t yStart,
						const dimension_t yEnd,
						bresenham_t fxLeft,
						bresenham_t fxRight,
						const bresenham_t stepLeft,
						const bresenham_t stepRight)
					{
						for (dimension_t y = yStart; y < yEnd; y++)
						{
							const dimension_t startX = FixedRoundToInt(fxLeft);
							const dimension_t endX = static_cast<dimension_t>(FixedRoundToInt(fxRight) - 1);

							if (startX <= endX)
							{
								Base::LineHorizontal(framebuffer, startX, endX, y, SkipSourceTag{}, SkipTransformTag{});
							}

							fxLeft += stepLeft;
							fxRight += stepRight;
						}
					}
				};
			}
		}
	}
}
#endif