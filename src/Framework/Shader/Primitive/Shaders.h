#ifndef _EGFX_FRAMEWORK_SHADER_PRIMITIVE_SHADERS_h
#define _EGFX_FRAMEWORK_SHADER_PRIMITIVE_SHADERS_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Primitive
			{
				/// <summary>
				/// Primitive shader pass-through.
				/// Applies an origin offset (Prepare) and forwards primitives to the framebuffer using a color source.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorSourceType">Color source providing Source(x,y) for per-pixel color selection.</typeparam>
				template<typename dimension_t,
					typename ColorSourceType = Shader::Source::SingleColor<dimension_t>
				>
				class NoShader
				{
				public:
					/// <summary>Color source used to produce the draw color (may be constant or position-dependent).</summary>
					ColorSourceType ColorSource{};

				private:
					/// <summary>Origin offset applied to all emitted primitives (pixel-space).</summary>
					pixel_point_t Origin{ 0, 0 };

				public:
					NoShader() = default;
					~NoShader() = default;

					/// <summary>
					/// Prepares the shader for a draw pass by setting the pixel-space origin offset.
					/// </summary>
					void Prepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
					}

					/// <summary>Draws a single pixel at local-space (x,y) offset by the prepared origin.</summary>
					void Pixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						framebuffer->Pixel(ColorSource.Source(x, y),
							static_cast<pixel_t>(Origin.x + x),
							static_cast<pixel_t>(Origin.y + y));
					}

					/// <summary>Draws a line between local-space endpoints offset by the prepared origin.</summary>
					void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2)
					{
						framebuffer->Line(ColorSource.Source(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2));
					}

					/// <summary>Fills an axis-aligned rectangle defined by two local-space corners offset by the prepared origin.</summary>
					void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2)
					{
						framebuffer->RectangleFill(ColorSource.Source(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2));
					}

					/// <summary>Fills a triangle defined by local-space vertices offset by the prepared origin.</summary>
					void TriangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2, const dimension_t x3, const dimension_t y3)
					{
						framebuffer->TriangleFill(ColorSource.Source(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2),
							static_cast<pixel_t>(Origin.x + x3),
							static_cast<pixel_t>(Origin.y + y3));
					}
				};

				/// <summary>
				/// Primitive shader wrapper that implements higher-level primitives (Line/RectangleFill/TriangleFill)
				/// in terms of the underlying pixel shader's Pixel(...) call.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PixelShaderType">Pixel shader providing Pixel(...) and typically Prepare(...).</typeparam>
				template<typename dimension_t,
					typename PixelShaderType>
				class TemplateShader : public PixelShaderType
				{
				protected:
					/// <summary>Intermediate type used by Bresenham helpers to avoid overflow.</summary>
					using bresenham_t = typename TypeTraits::TypeNext::next_int_type<dimension_t>::type;

				public:
					TemplateShader() : PixelShaderType() {}
					~TemplateShader() = default;

					/// <summary>
					/// Draws a line using the underlying Pixel(...) implementation.
					/// Uses horizontal/vertical fast paths and falls back to Bresenham for diagonals.
					/// </summary>
					void Line(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						if (y1 == y2)
						{
							// Horizontal line optimization.
							const dimension_t startX = MinValue(x1, x2);
							const dimension_t endX = MaxValue(x1, x2);
							for (dimension_t x = startX; x <= endX; x++)
							{
								PixelShaderType::Pixel(framebuffer, x, y1);
							}
						}
						else if (x1 == x2)
						{
							// Vertical line optimization.
							const dimension_t startY = MinValue(y1, y2);
							const dimension_t endY = MaxValue(y1, y2);
							for (dimension_t y = startY; y <= endY; y++)
							{
								PixelShaderType::Pixel(framebuffer, x1, y);
							}
						}
						else
						{
							BresenhamDiagonal(framebuffer, x1, y1, x2, y2);
						}
					}

					/// <summary>
					/// Fills an axis-aligned rectangle using the underlying Pixel(...) implementation.
					/// </summary>
					void RectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						const dimension_t startX = MinValue(x1, x2);
						const dimension_t endX = MaxValue(x1, x2);
						const dimension_t startY = MinValue(y1, y2);
						const dimension_t endY = MaxValue(y1, y2);
						for (dimension_t x = startX; x <= endX; x++)
						{
							for (dimension_t y = startY; y <= endY; y++)
							{
								PixelShaderType::Pixel(framebuffer, x, y);
							}
						}
					}

					/// <summary>
					/// Fills a triangle using the underlying Pixel(...) implementation.
					/// </summary>
					void TriangleFill(IFrameBuffer* /*framebuffer*/, const dimension_t /*x1*/, const dimension_t /*y1*/,
						const dimension_t /*x2*/, const dimension_t /*y2*/,
						const dimension_t /*x3*/, const dimension_t /*y3*/)
					{
						//TODO: implement pixel-shaded triangle breseham triangle helper.
					}

				protected:
					/// <summary>
					/// Dispatches to a Bresenham helper based on dominant axis (X-major vs Y-major) and direction.
					/// </summary>
					void BresenhamDiagonal(IFrameBuffer* framebuffer,
						const dimension_t x0, const dimension_t y0,
						const dimension_t x1, const dimension_t y1)
					{
						const bresenham_t dx = AbsValue(static_cast<bresenham_t>(x1) - x0);
						const bresenham_t dy = AbsValue(static_cast<bresenham_t>(y1) - y0);

						// X-major
						if (dx >= dy)
						{
							// Ensure startX <= endX
							if (x0 <= x1)
							{
								BresenhamRight(framebuffer, x0, y0, x1, y1);
							}
							else
							{
								BresenhamRight(framebuffer, x1, y1, x0, y0);
							}
						}
						// Y-major
						else
						{
							// Ensure startY <= endY
							if (y0 <= y1)
							{
								BresenhamUp(framebuffer, x0, y0, x1, y1);
							}
							else
							{
								BresenhamUp(framebuffer, x1, y1, x0, y0);
							}
						}
					}

					/// <summary>
					/// Bresenham line helper with monotonic X (iterates from startX toward endX).
					/// </summary>
					void BresenhamRight(IFrameBuffer* framebuffer, const dimension_t startX, const dimension_t startY, const dimension_t endX, const dimension_t endY)
					{
						const bresenham_t scaledWidth = (endX - startX) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endY - startY) << 1;
						const int8_t slopeUnit = (endY >= startY) ? 1 : -1;
						const int8_t slopeSign = (endX >= startX) ? 1 : -1;

						bresenham_t slopeError = slopeMagnitude - (endX - startX);
						dimension_t y = startY;

						for (dimension_t x = startX; x != endX; x += slopeSign)
						{
							PixelShaderType::Pixel(framebuffer, x, y);

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								y += slopeUnit;
								slopeError -= scaledWidth;
							}
						}

						// Ensure the final endpoint is drawn.
						PixelShaderType::Pixel(framebuffer, endX, endY);
					}

					/// <summary>
					/// Bresenham line helper with monotonic Y (iterates from startY toward endY).
					/// </summary>
					void BresenhamUp(IFrameBuffer* framebuffer, const dimension_t startX, const dimension_t startY, const dimension_t endX, const dimension_t endY)
					{
						const bresenham_t scaledHeight = (endY - startY) << 1;
						const bresenham_t slopeMagnitude = AbsValue(endX - startX) << 1;
						const int8_t slopeUnit = (endX >= startX) ? 1 : -1;
						const int8_t slopeSign = (endY >= startY) ? 1 : -1;

						bresenham_t slopeError = (bresenham_t)slopeMagnitude - (endY - startY);
						dimension_t x = startX;

						for (dimension_t y = startY; y != endY; y += slopeSign)
						{
							PixelShaderType::Pixel(framebuffer, x, y);

							slopeError += slopeMagnitude;
							if (slopeError >= 0)
							{
								x += slopeUnit;
								slopeError -= scaledHeight;
							}
						}

						// Ensure the final endpoint is drawn.
						PixelShaderType::Pixel(framebuffer, endX, endY);
					}
				};
			}
		}
	}
}
#endif