#ifndef _EGFX_VECTOR_GRAPHICS_SINGLE_COLOR_VECTOR_DRAWER_h
#define _EGFX_VECTOR_GRAPHICS_SINGLE_COLOR_VECTOR_DRAWER_h

#include "TemplateShadeDecoder.h"

namespace Egfx
{
	namespace VectorGraphics
	{
		namespace VectorShaders
		{
			/// <summary>
			/// Draws vector primitives to IFrameBuffer with origin translation and single color per draw call.
			/// Default implementation of a vector shader.
			/// </summary>
			/// <typeparam name="dimension_t">Vector coordinate dimension type.</typeparam>
			template<typename dimension_t>
			class SingleColorShader
			{
			private:
				IFrameBuffer* Framebuffer = nullptr;
				pixel_point_t Origin{ 0,0 };
				rgb_color_t Color = RGB_COLOR_WHITE;

			public:
				SingleColorShader() = default;

				~SingleColorShader() = default;

			protected:
				void PrimitivePoint(const dimension_t x, const dimension_t y)
				{
					Framebuffer->Pixel(Color,
						static_cast<pixel_t>(Origin.x + x),
						static_cast<pixel_t>(Origin.y + y));
				}

				void PrimitiveLine(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2)
				{
					Framebuffer->Line(Color,
						static_cast<pixel_t>(Origin.x + x1),
						static_cast<pixel_t>(Origin.y + y1),
						static_cast<pixel_t>(Origin.x + x2),
						static_cast<pixel_t>(Origin.y + y2));
				}

				void PrimitiveRectangleFill(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2)
				{
					Framebuffer->RectangleFill(Color,
						static_cast<pixel_t>(Origin.x + x1),
						static_cast<pixel_t>(Origin.y + y1),
						static_cast<pixel_t>(Origin.x + x2),
						static_cast<pixel_t>(Origin.y + y2));
				}

				void PrimitiveTriangleFill(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2,
					const dimension_t x3, const dimension_t y3)
				{
					Framebuffer->TriangleFill(Color,
						static_cast<pixel_t>(Origin.x + x1),
						static_cast<pixel_t>(Origin.y + y1),
						static_cast<pixel_t>(Origin.x + x2),
						static_cast<pixel_t>(Origin.y + y2),
						static_cast<pixel_t>(Origin.x + x3),
						static_cast<pixel_t>(Origin.y + y3));
				}

				void PrepareDraw(IFrameBuffer* framebuffer, const rgb_color_t color, const pixel_t originX, const pixel_t originY)
				{
					Framebuffer = framebuffer;
					Color = color;
					Origin.x = originX;
					Origin.y = originY;
				}
			};
		}


		/// <summary>
		/// Simple vector drawer that renders to an IFrameBuffer.
		/// Decodes and renders vectors to an IFrameBuffer.
		/// Configurable Vector Model, dimensions, and shader type.
		/// </summary>
		/// <typeparam name="ModelType">Vector Model that determines format and node structure.</typeparam>
		/// <typeparam name="dimension_t">Configurable Vector output dimensions type.</typeparam>
		/// <typeparam name="vector_count_t">The type used to count vectors. Defaults to uint_fast8_t.</typeparam>
		/// <typeparam name="VectorShaderType">The shader type used for rendering vectors. Defaults to VectorShaders::SingleColorShader.</typeparam>
		template<typename ModelType,
			typename dimension_t,
			typename vector_count_t = uint_fast8_t,
			typename VectorShaderType = VectorShaders::SingleColorShader<dimension_t>>
			class SingleColorVectorDrawer : public TemplateShadeDecoder<VectorShaderType, ModelType, dimension_t, vector_count_t>
		{
		private:
			using Base = TemplateShadeDecoder<VectorShaderType, ModelType, dimension_t, vector_count_t>;

		protected:
			using node_t = typename ModelType::node_t;

		public:
			SingleColorVectorDrawer(const dimension_t width = 16, const dimension_t height = 16)
				: Base(width, height)
			{
			}

			~SingleColorVectorDrawer() = default;

		public:
			void DrawVectors(IFrameBuffer* framebuffer, const node_t* vectors, const uint16_t vectorCount, const pixel_t x, const pixel_t y, const rgb_color_t color = RGB_COLOR_WHITE)
			{
				// Setup shader for this draw call.
				VectorShaderType::PrepareDraw(framebuffer, color, x, y);

				// Decode and render vectors. Primitive draw calls will be called on the shader.
				Base::DecodeVectors(vectors, vectorCount);
			}
		};
	}
}
#endif