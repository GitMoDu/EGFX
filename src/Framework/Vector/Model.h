#ifndef _INTEGERGLASS_FRAMEWORK_VECTOR_MODEL_h
#define _INTEGERGLASS_FRAMEWORK_VECTOR_MODEL_h

#include "../../IntegerGlassCore.h"
#include "../Shader/Geometry/Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		/// <summary>
		/// Compact node encodings for vector graphics streams.
		///
		/// A vector stream is made of nodes. Each node packs:
		/// - a primitive opcode (Start/End/TriangleFill/RectangleFill)
		/// - local-space coordinates on a small fixed grid (for example 8x8 or 128x128)
		/// 
		/// Nodes are decoded to (primitive, x, y) and then scaled into the target draw dimensions using
		/// integer math.
		/// 
		/// Models:
		/// - 8-bit (1 byte/node): 2-bit primitive + 3-bit X + 3-bit Y on an 8x8 grid [0..7].
		/// - 16-bit (2 bytes/node):
		///   - VectorNode65x65: shift-optimized, limited coordinate range for fast right-shift scaling.
		///   - VectorNode128x128: full 7-bit X/Y on a 128x128 grid [0..127], scaled via division.
		/// 
		/// Primitives:
		/// - Start: begin/continue a segment; draws a line from previous to current when a previous point exists.
		/// - End: terminate a segment; draws a line when previous != current, otherwise a single pixel; then resets state.
		/// - TriangleFill: triangle strip (sliding window) using (prev2, prev1, current) and outputs a filled triangle.
		/// - RectangleFill: filled axis-aligned rectangle using the last and current points as opposite corners.
		///
		/// </summary>
		namespace Vector
		{
			/// <summary>
			///	Thickness scale factor type. Fast fixed-point unsigned fraction.
			/// </summary>
			using thickness_scale_t = IntegerSignal::FixedPoint::ScalarFraction::UFraction8::scalar_t;

			/// <summary>
			///	Thickness scale factor for 1x thickness (no scaling).
			/// </summary>
			static constexpr thickness_scale_t THICKNESS_SCALE_1X = IntegerSignal::FixedPoint::ScalarFraction::UFraction8::SCALAR_UNIT;

			/// <summary>
			/// Canvas coordinate type. 8-bit unsigned integer.
			/// All canvases are at most 255x255, so we can use a single byte for each coordinate.
			///	</summary>	
			using canvas_t = uint8_t;

			/// <summary>
			/// Sequential packed vector streams.
			///
			/// Normal records are points and continue the current line. Records using the
			/// reserved maximum coordinate row or column are commands. Vector shapes should
			/// be top-left aligned so a caller can canvas each vector into a larger target
			/// without requiring shape-specific placement compensation.
			/// </summary>
			enum class PackedVectorEnum : uint8_t
			{
				Image15x15,
				Image255x255
			};

			enum class PrimitiveEnum : uint8_t
			{
				// Set the line-thickness parameter for subsequent primitives; consumes one operand.
				SetWeight,

				// Set the color index for subsequent primitives; consumes one operand.
				SetColor,

				// End the current line segment and clear the decoder's previous-point state.
				Break,

				// Draw an outlined rectangle from two operand points used as opposite corners.
				Rectangle,
				// Draw a filled rectangle from two operand points used as opposite corners.
				RectangleFill,

				// Draw an outlined triangle from three operand points used as vertices.
				Triangle,
				// Draw a filled triangle from three operand points used as vertices.
				TriangleFill,

				// Draw an outlined circle from center and radius-point operands.
				Circle,
				// Draw a filled circle from center and radius-point operands.
				CircleFill,

				// Draw an outlined ring from center and radii operands; X is outer radius and Y is inner radius.
				Ring,
				// Draw a filled ring from center and radii operands; X is outer radius and Y is inner radius.
				RingFill,

				// Draw an outlined arc from center, radius/properties, and angle operands; angles use X/Y.
				Arc,
				// Draw a filled arc from center, outer/inner radii, and angle operands; angles use X/Y.
				ArcFill,

				// Draw an outlined circle quadrant from center and radius/properties operands; Y bits 0..1 select the quadrant.
				CircleQuadrant,
				// Draw a filled circle quadrant from center and radius/properties operands; Y bits 0..1 select the quadrant.
				CircleQuadrantFill,

				EnumCount
			};

			using QuadrantEnum = Shader::Geometry::QuadrantEnum;


			namespace Contract
			{
				template<typename dimension_t>
				struct DecoderBase
				{
					void OnColorSet(const uint8_t colorIndex) {}

					void OnDrawPoint(IFrameBuffer* frame, const dimension_t x, const dimension_t y, const dimension_t thickness) {}
					void OnDrawLine(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1, const dimension_t x2, const dimension_t y2, const dimension_t thickness) {}
					void OnDrawRectangle(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const bool fill, const dimension_t thickness) {}
					void OnDrawTriangle(IFrameBuffer* frame, const dimension_t x1, const dimension_t y1,
						const dimension_t x2, const dimension_t y2,
						const dimension_t x3, const dimension_t y3,
						const bool fill, const dimension_t thickness) {}
					void OnDrawCircle(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius, const bool fill, const dimension_t thickness) {}
					void OnDrawCircleQuadrant(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
						const dimension_t radius, const QuadrantEnum quadrant,
						const bool fill, const dimension_t thickness) {}
					void OnDrawCircleArc(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
						const angle_t startAngle, const angle_t endAngle,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const bool fill, const dimension_t thickness) {}
					void OnDrawCircleRing(IFrameBuffer* frame, const dimension_t centerX, const dimension_t centerY,
						const dimension_t outerRadius, const dimension_t innerRadius,
						const bool fill, const dimension_t thickness) {}
				};
			}


			namespace Detail
			{
				template<typename packed_t, typename axis_t>
				struct template_source_t
				{
					PackedVectorEnum type; // Type of the source vector model
					const packed_t* data; // Pointer to the packed source vector model data
					size_t count; // Number of nodes in the source vector model
					axis_t canvasWidth; // Width of the source vector model in local-space coordinate count
					axis_t canvasHeight; // Height of the source vector model in local-space coordinate count
					ufraction8_t thicknessScale;
					uint8_t inset;
					// CircleQuadrant operands are center and radius/properties; radius is X, quadrant is Y bits 0..1.
					// Ring operands are center and radii; outer radius is X and inner radius is Y.
					// Arc operands are center, radius/properties, and start/end angles; radius is X and angles are X/Y.
					// ArcFill operands are center, outer/inner radii, and start/end angles.

					bool Validate() const
					{
						return data != nullptr
							&& count > 0
							&& canvasWidth <= static_cast<axis_t>((1u << (sizeof(axis_t) * 8)) - 1u)
							&& canvasHeight <= static_cast<axis_t>((1u << (sizeof(axis_t) * 8)) - 1u);
					}
				};

				template<typename node_type, typename opcode_type, uint8_t axis_size, uint16_t axis_max>
				struct template_model_t
				{
					using axis_t = uint8_t;
					using opcode_t = opcode_type;

					static constexpr uint8_t AxisSize = axis_size;
					static constexpr axis_t AxisMask = static_cast<axis_t>((1u << AxisSize) - 1u);
					static constexpr axis_t AxisMax = static_cast<axis_t>(axis_max);
					static constexpr axis_t OpCodeAxis = AxisMask;
					static constexpr opcode_t OpCodeCount = static_cast<opcode_t>(AxisMask) * 2 + 1;

					struct Node
					{
						using packed_t = node_type;

						opcode_t opcode{};
						axis_t x{};
						axis_t y{};

						constexpr Node(const opcode_t opcodeValue, const axis_t xValue, const axis_t yValue)
							: opcode(opcodeValue), x(xValue), y(yValue) {}

						static constexpr Node FromPacked(const packed_t packed)
						{
							return Node(
								((packed >> AxisSize) & AxisMask) == OpCodeAxis
								? static_cast<opcode_t>(packed & AxisMask)
								: static_cast<opcode_t>(OpCodeAxis + 1 + ((packed >> AxisSize) & AxisMask)),
								static_cast<axis_t>((packed >> AxisSize) & AxisMask),
								static_cast<axis_t>(packed & AxisMask));
						}

						constexpr bool IsOpCode() const
						{
							return x == OpCodeAxis || y == OpCodeAxis;
						}
					};

					using source_t = template_source_t<node_type, axis_t>;

				};
			}

			namespace Image15x15
			{
				using model_t = Detail::template_model_t<uint8_t, uint8_t, 4, 14>;
				using axis_t = typename model_t::axis_t;
				using opcode_t = typename model_t::opcode_t;
				using source_t = typename model_t::source_t;
					using packed_t = typename model_t::Node::packed_t;

				static constexpr uint8_t AxisSize = model_t::AxisSize;
				static constexpr axis_t AxisMask = model_t::AxisMask;
				static constexpr axis_t AxisMax = model_t::AxisMax;
				static constexpr axis_t OpCodeAxis = model_t::OpCodeAxis;
				static constexpr opcode_t OpCodeCount = model_t::OpCodeCount;
			}

			namespace Image255x255
			{
				using model_t = Detail::template_model_t<uint16_t, uint16_t, 8, 254>;
				using axis_t = typename model_t::axis_t;
				using opcode_t = typename model_t::opcode_t;
				using source_t = typename model_t::source_t;
					using packed_t = typename model_t::Node::packed_t;

				static constexpr uint8_t AxisSize = model_t::AxisSize;
				static constexpr axis_t AxisMask = model_t::AxisMask;
				static constexpr axis_t AxisMax = model_t::AxisMax;
				static constexpr axis_t OpCodeAxis = model_t::OpCodeAxis;
				static constexpr opcode_t OpCodeCount = model_t::OpCodeCount;
			}

			template<PackedVectorEnum vectorType>
			struct Format;

			template<>
			struct Format<PackedVectorEnum::Image15x15>
			{
				using model_t = Image15x15::model_t;
				using node_t = Image15x15::model_t::Node;
				using packed_t = Image15x15::packed_t;
			};

			template<>
			struct Format<PackedVectorEnum::Image255x255>
			{
				using model_t = Image255x255::model_t;
				using node_t = Image255x255::model_t::Node;
				using packed_t = Image255x255::packed_t;
			};
		}
	}
}
#endif