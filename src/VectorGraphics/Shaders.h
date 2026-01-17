#ifndef _EGFX_VECTOR_GRAPHICS_SHADERS_h
#define _EGFX_VECTOR_GRAPHICS_SHADERS_h

#include "Model.h"

namespace Egfx
{
	namespace VectorGraphics
	{
		namespace Shaders
		{
			namespace Contract
			{
				/// <summary>
				/// Defines the contract for primitive shading (rasterization).
				/// Optional prepare step is called once per primitive batch to set up any necessary state.
				/// All draw methods correspond to primitive drawing calls, which can be custom or not implemented.
				/// </summary>
				/// <typeparam name="dimension_t">The vector's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct PrimitiveShader
				{
					void PrimitivePrepare(const pixel_t originX, const pixel_t originY) {}
					void PrimitivePoint(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y) {}
					void PrimitiveLine(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2) {}
					void PrimitiveRectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2) {}
					void PrimitiveTriangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2, const dimension_t x3, const dimension_t y3) {}
				};

				/// <summary>
				/// Defines the contract for color shading, uses only coordinate inputs to determine the color output.
				/// </summary>
				/// <typeparam name="dimension_t">The type used to represent coordinate dimensions.</typeparam>
				template<typename dimension_t>
				struct ColorShader
				{
					rgb_color_t ShadeColor(const dimension_t /*x*/, const dimension_t /*y*/)
					{
						return RGB_COLOR_WHITE;
					}
				};
			}

			namespace Color
			{
				/// <summary>
				/// Color shader that always returns the same color, independent of position.
				/// </summary>
				/// <typeparam name="dimension_t">The vector's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct Single
				{
					rgb_color_t Color = RGB_COLOR_WHITE;

					rgb_color_t ShadeColor(const dimension_t /*x*/, const dimension_t /*y*/) const
					{
						return Color;
					}
				};

				/// <summary>
				/// Stateless color shader that always returns white.
				/// Intended as a lightweight/default shader (no per-instance storage).
				/// </summary>
				/// <typeparam name="dimension_t">The vector's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				struct Full
				{
					static constexpr rgb_color_t ShadeColor(const dimension_t /*x*/, const dimension_t /*y*/)
					{
						return RGB_COLOR_WHITE;
					}
				};
			}

			namespace Primitive
			{
				/// <summary>
				/// Default primitive shader that uses a single color shader to render all primitives.
				/// Forwards all primitive calls to the frame buffer with the color determined by the color shader.
				/// </summary>
				/// <typeparam name="dimension_t">The vector's intrinsic dimension type.</typeparam>
				/// <typeparam name="ColorShaderType">ColorShader type, should follow VectorGraphics::Shaders::Contract::ColorShader contract.</typeparam>
				template<typename dimension_t, typename ColorShaderType = Color::Single<dimension_t>>
				struct SingleColor
				{
					ColorShaderType ColorShader{};

					pixel_point_t Origin{ 0, 0 };

					void PrimitivePrepare(const pixel_t originX, const pixel_t originY)
					{
						Origin.x = originX;
						Origin.y = originY;
					}

					void PrimitivePixel(IFrameBuffer* framebuffer, const dimension_t x, const dimension_t y)
					{
						framebuffer->Pixel(ColorShader.ShadeColor(x, y),
							static_cast<pixel_t>(Origin.x + x),
							static_cast<pixel_t>(Origin.y + y));
					}

					void PrimitiveLine(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						framebuffer->Line(ColorShader.ShadeColor(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2));
					}

					void PrimitiveRectangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2)
					{
						framebuffer->RectangleFill(ColorShader.ShadeColor(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2));
					}

					void PrimitiveTriangleFill(IFrameBuffer* framebuffer, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const dimension_t x3, const dimension_t y3)
					{
						framebuffer->TriangleFill(ColorShader.ShadeColor(x1, y1),
							static_cast<pixel_t>(Origin.x + x1),
							static_cast<pixel_t>(Origin.y + y1),
							static_cast<pixel_t>(Origin.x + x2),
							static_cast<pixel_t>(Origin.y + y2),
							static_cast<pixel_t>(Origin.x + x3),
							static_cast<pixel_t>(Origin.y + y3));
					}
				};
			}
		}
	}
}
#endif