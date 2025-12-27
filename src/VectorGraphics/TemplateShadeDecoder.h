#ifndef _EGFX_VECTOR_GRAPHICS_TEMPLATE_SHADE_DECODER_h
#define _EGFX_VECTOR_GRAPHICS_TEMPLATE_SHADE_DECODER_h

#include "Model.h"

namespace Egfx
{
	namespace VectorGraphics
	{
		namespace Contract
		{
			template<typename dimension_t>
			class VectorShader
			{
			public:
				VectorShader() {}

			protected:
				void PrimitivePoint(const dimension_t x, const dimension_t y) {}

				void PrimitiveLine(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2)
				{
				}

				void PrimitiveRectangleFill(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2)
				{
				}

				void PrimitiveTriangleFill(const dimension_t x1, const dimension_t y1,
					const dimension_t x2, const dimension_t y2,
					const dimension_t x3, const dimension_t y3)
				{
				}

				void PrepareDraw(IFrameBuffer* framebuffer, const rgb_color_t color, const pixel_t originX, const pixel_t originY)
				{
				}
			};
		}

		template<typename VectorShaderType,
			typename ModelType,
			typename dimension_t,
			typename vector_count_t = uint_fast8_t>
		class TemplateShadeDecoder : public VectorShaderType
		{
		protected:
			using axis_t = typename ModelType::axis_t;
			using node_t = typename ModelType::node_t;

			struct coordinates_t
			{
				axis_t x;
				axis_t y;
			};

		public:
			dimension_t Width;
			dimension_t Height;

		public:
			TemplateShadeDecoder(const dimension_t width = 16, const dimension_t height = 16)
				: VectorShaderType()
				, Width(width), Height(height)
			{
			}

		protected:
			/// <summary>
			/// Decode a vector stream and emit primitives to the provided output shader.
			/// Matches the FramebufferDraw behavior:
			/// - Start: continue polyline; terminates any active strip without emitting a triangle.
			/// - End: emits final line/pixel or final strip triangle, then resets segment state.
			/// - TriangleFill: sliding window strip; needs two prior points; Start/End stop the strip.
			/// - RectangleFill: uses previous and current as opposite corners (corner ordering handled downstream).
			/// Coordinates are scaled to [0..Width]/[0..Height] for the active ModelType.
			/// </summary>
			/// <param name="vector_shader_t"> The output shader type; must implement Point, Line, TriangleFill, and RectangleFill methods.</param>
			void DecodeVectors(const node_t* vectors, const uint16_t vectorCount)
			{
				coordinates_t lastPoints[3]{}; // [prev2, prev1, current]
				uint8_t pointIndex = 0;        // 0: none, 1: prev1 valid, 2: prev2+prev1 valid (strip-ready).
				bool polygonActive = false;    // True while emitting TriangleFill strip.

				for (vector_count_t i = 0; i < vectorCount; i++)
				{
					const node_t raw = ReadNode(vectors, i);
					const NodeEnum primitive = ModelType::GetPrimitive(raw);

					// Decode + scale coordinates (bypass scaling if target matches AxisMax).
					lastPoints[2].x = Width == ModelType::AxisMax ? ModelType::GetAxisX(raw) : ModelType::GetScaledAxisX(Width, raw);
					lastPoints[2].y = Height == ModelType::AxisMax ? ModelType::GetAxisY(raw) : ModelType::GetScaledAxisY(Height, raw);

					switch (primitive)
					{
					case NodeEnum::Start:
						// Starting a new segment terminates any active strip without emitting an extra triangle.
						if (polygonActive)
						{
							polygonActive = false;
							pointIndex = 1;
						}
						// If we already had a previous point, connect it as a line (or pixel if degenerate).
						if (pointIndex > 0)
						{
							if (lastPoints[1].x != lastPoints[2].x || lastPoints[1].y != lastPoints[2].y)
							{
								VectorShaderType::PrimitiveLine(lastPoints[1].x, lastPoints[1].y,
									lastPoints[2].x, lastPoints[2].y);
							}
							else
							{
								VectorShaderType::PrimitivePoint(lastPoints[2].x, lastPoints[2].y);
							}
						}
						// Arm for next point.
						pointIndex = 1;
						break;
					case NodeEnum::End:
						// End terminates strip and emits the final segment if present.
						if (polygonActive)
						{
							polygonActive = false;
							if (pointIndex >= 2)
							{
								VectorShaderType::PrimitiveTriangleFill(
									lastPoints[0].x, lastPoints[0].y,
									lastPoints[1].x, lastPoints[1].y,
									lastPoints[2].x, lastPoints[2].y);
							}
						}
						else if (pointIndex > 0)
						{
							if (lastPoints[1].x != lastPoints[2].x || lastPoints[1].y != lastPoints[2].y)
							{
								VectorShaderType::PrimitiveLine(lastPoints[1].x, lastPoints[1].y,
									lastPoints[2].x, lastPoints[2].y);
							}
							else
							{
								VectorShaderType::PrimitivePoint(lastPoints[2].x, lastPoints[2].y);
							}
						}
						pointIndex = 0; // Reset segment state.
						break;
					case NodeEnum::TriangleFill:
						// Arm strip on first TriangleFill; emit once two prior points are valid.
						if (!polygonActive)
						{
							polygonActive = true;
							if (pointIndex < 2)
							{
								pointIndex++; // Grow toward having prev2+prev1.
							}
						}
						if (polygonActive && pointIndex >= 2)
						{
							VectorShaderType::PrimitiveTriangleFill(
								lastPoints[0].x, lastPoints[0].y,
								lastPoints[1].x, lastPoints[1].y,
								lastPoints[2].x, lastPoints[2].y);
						}
						else if (polygonActive && pointIndex < 2)
						{
							// After consuming current, we will have prev1 ready; cap at 2 later via slide.
							pointIndex = 2;
						}
						break;

					case NodeEnum::RectangleFill:
						// Emit rectangle using previous and current as opposite corners.
						if (pointIndex >= 1)
						{
							VectorShaderType::PrimitiveRectangleFill(
								lastPoints[1].x, lastPoints[1].y,
								lastPoints[2].x, lastPoints[2].y);
						}
						// Keep last point as the first corner for potential chaining.
						pointIndex = 1;
						polygonActive = false;
						break;
					default:
						break;
					}

					// Slide window: prev2 <- prev1, prev1 <- current.
					lastPoints[0] = lastPoints[1];
					lastPoints[1] = lastPoints[2];
				}
			}

		private:
			// PROGMEM-safe node readers for AVR (flash) vs others (RAM).
			static uint8_t ReadNode(const uint8_t* base, const vector_count_t i)
			{
#if defined(ARDUINO_ARCH_AVR)
				return pgm_read_byte_near(base + i);
#else
				return base[i];
#endif
			}
			static uint16_t ReadNode(const uint16_t* base, const vector_count_t i)
			{
#if defined(ARDUINO_ARCH_AVR)
				return pgm_read_word_near(base + i);
#else
				return base[i];
#endif
			}
		};		
	}
}
#endif