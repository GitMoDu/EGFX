#ifndef _EGFX_FRAMEWORK_VECTOR_TEMPLATE_DRAWER_h
#define _EGFX_FRAMEWORK_VECTOR_TEMPLATE_DRAWER_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Vector
		{
			/// <summary>
			/// Vector stream drawer.
			///
			/// Decodes a packed vector node stream (ModelType) and renders it using a primitive shader.
			/// Nodes provide a primitive opcode plus packed local-space coordinates which are scaled into
			/// the configured Width/Height before emitting primitives.
			/// </summary>
			/// <typeparam name="ModelType">Vector node model type providing node_t, axis_t, decode helpers, and scaling helpers.</typeparam>
			/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
			/// <typeparam name="vector_count_t">Vector node index type.</typeparam>
			/// <typeparam name="PrimitiveShaderType">Primitive shader type providing Prepare(...) and primitive draw calls.</typeparam>
			template<
				typename ModelType,
				typename dimension_t,
				typename vector_count_t = uint_fast8_t,
				typename PrimitiveShaderType = Shader::Primitive::NoShader<dimension_t>
			>
			class TemplateDrawer : public PrimitiveShaderType
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
				/// <summary>Target width used when scaling local-space X coordinates (pixels).</summary>
				dimension_t Width;

				/// <summary>Target height used when scaling local-space Y coordinates (pixels).</summary>
				dimension_t Height;

			public:
				/// <summary>
				/// Constructs the drawer with target scaling dimensions.
				/// </summary>
				/// <param name="width">Target width used when scaling local-space X coordinates (pixels).</param>
				/// <param name="height">Target height used when scaling local-space Y coordinates (pixels).</param>
				TemplateDrawer(const dimension_t width = 16, const dimension_t height = 16)
					: PrimitiveShaderType()
					, Width(width), Height(height)
				{
				}

				~TemplateDrawer() = default;

				/// <summary>
				/// Decodes a vector stream and renders primitives.
				///
				/// Primitive behavior:
				/// - Start: continues a polyline segment; terminates any active triangle strip without emitting a triangle.
				/// - End: emits the final line/pixel or final strip triangle, then resets segment state.
				/// - TriangleFill: triangle strip (sliding window); requires two prior points; Start/End terminate the strip.
				/// - RectangleFill: fills an axis-aligned rectangle using the previous and current points as opposite corners.
				///
				/// Coordinates are decoded from nodes and scaled into the configured Width/Height before drawing.
				/// </summary>
				/// <param name="framebuffer">Target framebuffer to draw into.</param>
				/// <param name="x">Pixel-space origin X (pixels).</param>
				/// <param name="y">Pixel-space origin Y (pixels).</param>
				/// <param name="vectors">Pointer to packed vector nodes (RAM or flash depending on node type reader).</param>
				/// <param name="vectorCount">Number of nodes in the vector stream.</param>
				void Draw(IFrameBuffer* framebuffer, const pixel_t x, const pixel_t y, const node_t* vectors, const uint16_t vectorCount)
				{
					PrimitiveShaderType::Prepare(x, y);

					coordinates_t lastPoints[3]{}; // [prev2, prev1, current]
					uint8_t pointIndex = 0;        // 0: none, 1: prev1 valid, 2: prev2 + prev1 valid
					bool polygonActive = false;    // True while emitting TriangleFill strip

					for (vector_count_t i = 0; i < vectorCount; i++)
					{
						const node_t raw = ReadNode(vectors, i);
						const NodeEnum primitive = ModelType::GetPrimitive(raw);

						lastPoints[2].x = Width == ModelType::AxisMax ? ModelType::GetAxisX(raw) : ModelType::GetScaledAxisX(Width, raw);
						lastPoints[2].y = Height == ModelType::AxisMax ? ModelType::GetAxisY(raw) : ModelType::GetScaledAxisY(Height, raw);

						switch (primitive)
						{
						case NodeEnum::Start:
							if (polygonActive)
							{
								polygonActive = false;
								pointIndex = 1;
							}

							if (pointIndex > 0)
							{
								if (lastPoints[1].x != lastPoints[2].x || lastPoints[1].y != lastPoints[2].y)
								{
									PrimitiveShaderType::Line(framebuffer, lastPoints[1].x, lastPoints[1].y,
										lastPoints[2].x, lastPoints[2].y);
								}
								else
								{
									PrimitiveShaderType::Pixel(framebuffer, lastPoints[2].x, lastPoints[2].y);
								}
							}

							pointIndex = 1;
							break;

						case NodeEnum::End:
							if (polygonActive)
							{
								polygonActive = false;
								if (pointIndex >= 2)
								{
									PrimitiveShaderType::TriangleFill(framebuffer,
										lastPoints[0].x, lastPoints[0].y,
										lastPoints[1].x, lastPoints[1].y,
										lastPoints[2].x, lastPoints[2].y);
								}
							}
							else if (pointIndex > 0)
							{
								if (lastPoints[1].x != lastPoints[2].x || lastPoints[1].y != lastPoints[2].y)
								{
									PrimitiveShaderType::Line(framebuffer, lastPoints[1].x, lastPoints[1].y,
										lastPoints[2].x, lastPoints[2].y);
								}
								else
								{
									PrimitiveShaderType::Pixel(framebuffer, lastPoints[2].x, lastPoints[2].y);
								}
							}

							pointIndex = 0;
							break;

						case NodeEnum::TriangleFill:
							if (!polygonActive)
							{
								polygonActive = true;
								if (pointIndex < 2)
								{
									pointIndex++;
								}
							}

							if (polygonActive && pointIndex >= 2)
							{
								PrimitiveShaderType::TriangleFill(framebuffer,
									lastPoints[0].x, lastPoints[0].y,
									lastPoints[1].x, lastPoints[1].y,
									lastPoints[2].x, lastPoints[2].y);
							}
							else if (polygonActive && pointIndex < 2)
							{
								pointIndex = 2;
							}
							break;

						case NodeEnum::RectangleFill:
							// Emit rectangle using previous and current as opposite corners.
							if (pointIndex >= 1)
							{
								PrimitiveShaderType::RectangleFill(framebuffer,
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
				/// <summary>
				/// Reads a packed node from memory.
				/// On AVR this reads from flash/PROGMEM; on other targets it reads from RAM.
				/// </summary>
				static uint8_t ReadNode(const uint8_t* base, const vector_count_t i)
				{
#if defined(ARDUINO_ARCH_AVR)
					return pgm_read_byte_near(base + i);
#else
					return base[i];
#endif
				}

				/// <summary>
				/// Reads a packed node from memory.
				/// On AVR this reads from flash/PROGMEM; on other targets it reads from RAM.
				/// </summary>
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
}
#endif