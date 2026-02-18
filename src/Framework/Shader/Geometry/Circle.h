#ifndef _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_h
#define _EGFX_FRAMEWORK_SHADER_GEOMETRY_CIRCLE_h

#include <EgfxFramework.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Geometry
			{
				/// <summary>
				/// Geometry shader stage that rasterizes circle primitives on top of a primitive shader pipeline.
				/// Uses midpoint circle stepping and emits pixels via Pixel(...) and filled spans via RectangleFill(...).
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Pixel(...), RectangleFill(...), and Prepare(...).</typeparam>
				template<typename dimension_t,
					typename PrimitiveShaderType = Framework::Shader::Primitive::TemplateShader<dimension_t>
				>
				class CircleShader : public PrimitiveShaderType
				{
				private:
					using Base = PrimitiveShaderType;

				public:
					CircleShader() : Base() {}
					~CircleShader() = default;

				public:
					/// <summary>
					/// Draws a circle outline centered at (centerX, centerY) in local space.
					/// Uses 8-way symmetry and emits pixels through the underlying primitive shader.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Circle radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleOutline(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Draw8(framebuffer, centerX, centerY, x, y);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Draw8(framebuffer, centerX, centerY, x, y);
						}
					}

					/// <summary>
					/// Draws a filled circle centered at (centerX, centerY) in local space.
					/// Emits horizontal spans via RectangleFill(...) to reduce per-pixel overhead.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Circle radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Fill4Spans(framebuffer, centerX, centerY, x, y);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Fill4Spans(framebuffer, centerX, centerY, x, y);
						}
					}

					/// <summary>
					/// Draws the top-left quarter of a circle outline centered at (centerX, centerY) in local space.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterTopLeft(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::Pixel(framebuffer, centerX - x, centerY - y);
						Base::Pixel(framebuffer, centerX - y, centerY - x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::Pixel(framebuffer, centerX - x, centerY - y);
							Base::Pixel(framebuffer, centerX - y, centerY - x);
						}
					}

					/// <summary>
					/// Draws a filled top-left quarter of a circle centered at (centerX, centerY) in local space.
					/// Emits horizontal spans via RectangleFill(...).
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterTopLeftFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::RectangleFill(framebuffer, centerX - x, centerY - y, centerX, centerY - y);
						Base::RectangleFill(framebuffer, centerX - y, centerY - x, centerX, centerY - x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::RectangleFill(framebuffer, centerX - x, centerY - y, centerX, centerY - y);
							Base::RectangleFill(framebuffer, centerX - y, centerY - x, centerX, centerY - x);
						}
					}

					/// <summary>
					/// Draws the top-right quarter of a circle outline centered at (centerX, centerY) in local space.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterTopRight(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::Pixel(framebuffer, centerX + x, centerY - y);
						Base::Pixel(framebuffer, centerX + y, centerY - x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::Pixel(framebuffer, centerX + x, centerY - y);
							Base::Pixel(framebuffer, centerX + y, centerY - x);
						}
					}

					/// <summary>
					/// Draws a filled top-right quarter of a circle centered at (centerX, centerY) in local space.
					/// Emits horizontal spans via RectangleFill(...).
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterTopRightFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::RectangleFill(framebuffer, centerX, centerY - y, centerX + x, centerY - y);
						Base::RectangleFill(framebuffer, centerX, centerY - x, centerX + y, centerY - x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::RectangleFill(framebuffer, centerX, centerY - y, centerX + x, centerY - y);
							Base::RectangleFill(framebuffer, centerX, centerY - x, centerX + y, centerY - x);
						}
					}

					/// <summary>
					/// Draws the bottom-left quarter of a circle outline centered at (centerX, centerY) in local space.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterBottomLeft(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::Pixel(framebuffer, centerX - x, centerY + y);
						Base::Pixel(framebuffer, centerX - y, centerY + x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::Pixel(framebuffer, centerX - x, centerY + y);
							Base::Pixel(framebuffer, centerX - y, centerY + x);
						}
					}

					/// <summary>
					/// Draws a filled bottom-left quarter of a circle centered at (centerX, centerY) in local space.
					/// Emits horizontal spans via RectangleFill(...).
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterBottomLeftFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::RectangleFill(framebuffer, centerX - x, centerY + y, centerX, centerY + y);
						Base::RectangleFill(framebuffer, centerX - y, centerY + x, centerX, centerY + x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::RectangleFill(framebuffer, centerX - x, centerY + y, centerX, centerY + y);
							Base::RectangleFill(framebuffer, centerX - y, centerY + x, centerX, centerY + x);
						}
					}

					/// <summary>
					/// Draws the bottom-right quarter of a circle outline centered at (centerX, centerY) in local space.
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterBottomRight(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::Pixel(framebuffer, centerX + x, centerY + y);
						Base::Pixel(framebuffer, centerX + y, centerY + x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::Pixel(framebuffer, centerX + x, centerY + y);
							Base::Pixel(framebuffer, centerX + y, centerY + x);
						}
					}

					/// <summary>
					/// Draws a filled bottom-right quarter of a circle centered at (centerX, centerY) in local space.
					/// Emits horizontal spans via RectangleFill(...).
					/// </summary>
					/// <param name="framebuffer">Target framebuffer to draw into.</param>
					/// <param name="centerX">Center X coordinate (local space).</param>
					/// <param name="centerY">Center Y coordinate (local space).</param>
					/// <param name="radius">Quarter radius (local space). Values less than or equal to 0 draw nothing.</param>
					void CircleQuarterBottomRightFill(IFrameBuffer* framebuffer,
						const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius)
					{
						if (radius <= 0)
						{
							return;
						}

						dimension_t x = radius;
						dimension_t y = 0;
						dimension_t d = 1 - radius;

						Base::RectangleFill(framebuffer, centerX, centerY + y, centerX + x, centerY + y);
						Base::RectangleFill(framebuffer, centerX, centerY + x, centerX + y, centerY + x);

						while (y < x)
						{
							y++;

							if (d <= 0)
							{
								d += (2 * y) + 1;
							}
							else
							{
								x--;
								d += (2 * (y - x)) + 1;
							}

							Base::RectangleFill(framebuffer, centerX, centerY + y, centerX + x, centerY + y);
							Base::RectangleFill(framebuffer, centerX, centerY + x, centerX + y, centerY + x);
						}
					}

				private:
					/// <summary>
					/// Emits the 8 symmetric outline points for a circle step at (px, py) relative to the center.
					/// </summary>
					inline void Draw8(IFrameBuffer* framebuffer,
						const pixel_t centerX, const pixel_t centerY,
						const pixel_t px, const pixel_t py)
					{
						Base::Pixel(framebuffer, centerX + px, centerY + py);
						Base::Pixel(framebuffer, centerX - px, centerY + py);
						Base::Pixel(framebuffer, centerX + px, centerY - py);
						Base::Pixel(framebuffer, centerX - px, centerY - py);

						Base::Pixel(framebuffer, centerX + py, centerY + px);
						Base::Pixel(framebuffer, centerX - py, centerY + px);
						Base::Pixel(framebuffer, centerX + py, centerY - px);
						Base::Pixel(framebuffer, centerX - py, centerY - px);
					}

					/// <summary>
					/// Emits the 4 symmetric filled spans for a circle step at (x, y) relative to the center.
					/// </summary>
					inline void Fill4Spans(IFrameBuffer* framebuffer, const pixel_t centerX, const pixel_t centerY, const dimension_t x, const dimension_t y)
					{
						Base::RectangleFill(framebuffer, centerX - x, centerY + y, centerX + x, centerY + y);

						if (y != 0)
						{
							Base::RectangleFill(framebuffer, centerX - x, centerY - y, centerX + x, centerY - y);
						}

						if (x != y)
						{
							Base::RectangleFill(framebuffer, centerX - y, centerY + x, centerX + y, centerY + x);
							Base::RectangleFill(framebuffer, centerX - y, centerY - x, centerX + y, centerY - x);
						}
					}
				};
			}
		}
	}
}

#endif