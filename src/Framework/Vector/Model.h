#ifndef _EGFX_FRAMEWORK_VECTOR_MODEL_h
#define _EGFX_FRAMEWORK_VECTOR_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
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
			using namespace IntegerSignal::FixedPoint;

			/// <summary>
			/// Primitive commands (2-bit values).
			/// </summary>
			enum class NodeEnum
			{
				Start = 0b00,         // Begin/continue a segment; may draw a line from the previous point.
				End = 0b01,           // End a segment; draw line/pixel from the previous point, then reset state.
				TriangleFill = 0b10,  // Triangle strip: (prev2, prev1, current) -> filled triangle.
				RectangleFill = 0b11, // Filled rectangle between the previous and current points.
				EnumCount
			};

			/// <summary>Bit mask covering the primitive opcode field.</summary>
			static constexpr uint8_t PrimitiveMask = uint8_t(NodeEnum::EnumCount) - 1;

			/// <summary>
			/// 8-bit node model: 2-bit primitive + 3-bit X + 3-bit Y (8x8 grid).
			/// </summary>
			struct VectorNode8x8
			{
				using node_t = uint8_t; // 1 byte per node.
				using axis_t = uint8_t;

				static constexpr uint8_t CoordinatesSize =
					GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

				static constexpr uint8_t AxisSize = CoordinatesSize / 2;
				static constexpr axis_t AxisMax = (1 << AxisSize) - 1; // 7
				static constexpr uint8_t AxisMask = AxisMax;           // 0b111

				static constexpr NodeEnum GetPrimitive(const node_t node)
				{
					return static_cast<NodeEnum>((node >> CoordinatesSize) & PrimitiveMask);
				}

				static constexpr axis_t GetAxisX(const node_t node)
				{
					return (node >> AxisSize) & AxisMask;
				}

				static constexpr axis_t GetAxisY(const node_t node)
				{
					return node & AxisMask;
				}

				/// <summary>
				/// Scales a local axis value from [0..AxisMax] into [0..dimension] using integer division.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
				{
					using high_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;
					return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) / AxisMax);
				}

				/// <summary>
				/// Scales the packed X coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisX(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, GetAxisX(node));
				}

				/// <summary>
				/// Scales the packed Y coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisY(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, GetAxisY(node));
				}
			};

			/// <summary>
			/// 16-bit node model (shift-optimized): 2-bit primitive + limited-range X/Y for fast scaling (right shift).
			/// </summary>
			struct VectorNode65x65
			{
				using node_t = uint16_t; // 2 bytes per node.
				using axis_t = uint8_t;

				static constexpr uint8_t CoordinatesSize =
					GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

				static constexpr uint8_t AxisSize = CoordinatesSize / 2;

				// Note: AxisMax/AxisMask intentionally do not represent the full 7-bit mask.
				static constexpr uint16_t AxisMax = 1 << (AxisSize - 1);
				static constexpr uint16_t AxisMask = (1 << (AxisSize - 1));

				static constexpr NodeEnum GetPrimitive(const node_t node)
				{
					return static_cast<NodeEnum>((node >> CoordinatesSize) & PrimitiveMask);
				}

				static constexpr uint16_t GetAxisX(const node_t node)
				{
					return (node >> AxisSize) & AxisMask;
				}

				static constexpr uint16_t GetAxisY(const node_t node)
				{
					return node & AxisMask;
				}

				/// <summary>
				/// Scales via right shift: (dimension * axis) >> AxisSize.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
				{
					using large_t = typename TypeTraits::TypeConditional::larger_type<dimension_t, uint16_t>::type;
					using high_t = typename TypeTraits::TypeNext::next_uint_type<large_t>::type;
					return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) >> AxisSize);
				}

				/// <summary>
				/// Scales the packed X coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisX(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, static_cast<axis_t>(GetAxisX(node)));
				}

				/// <summary>
				/// Scales the packed Y coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisY(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, static_cast<axis_t>(GetAxisY(node)));
				}
			};

			/// <summary>
			/// 16-bit node model (full range): 2-bit primitive + 7-bit X + 7-bit Y (128x128 grid).
			/// Uses integer division for scaling.
			/// </summary>
			struct VectorNode128x128
			{
				using node_t = uint16_t; // 2 bytes per node.
				using axis_t = uint8_t;  // Axis max fits in uint8_t.

				static constexpr uint8_t CoordinatesSize =
					GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

				static constexpr uint8_t AxisSize = CoordinatesSize / 2;
				static constexpr axis_t AxisMax = (1 << AxisSize) - 1; // 127
				static constexpr uint8_t AxisMask = AxisMax;

				static constexpr NodeEnum GetPrimitive(const node_t node)
				{
					return static_cast<NodeEnum>((node >> CoordinatesSize) & PrimitiveMask);
				}

				static constexpr axis_t GetAxisX(const node_t node)
				{
					return (node >> AxisSize) & AxisMask;
				}

				static constexpr axis_t GetAxisY(const node_t node)
				{
					return node & AxisMask;
				}

				/// <summary>
				/// Scales a local axis value from [0..AxisMax] into [0..dimension] using integer division.
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
				{
					using high_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;
					return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) / AxisMax);
				}

				/// <summary>
				/// Scales the packed X coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisX(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, GetAxisX(node));
				}

				/// <summary>
				/// Scales the packed Y coordinate into [0..dimension].
				/// </summary>
				/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
				template<typename dimension_t>
				static constexpr dimension_t GetScaledAxisY(const dimension_t dimension, const node_t node)
				{
					return GetScaled(dimension, GetAxisY(node));
				}
			};

			/// <summary>
			/// Packs a primitive opcode and 8x8 coordinates into an 8-bit node value.
			/// </summary>
			/// <param name="primitive">Primitive opcode.</param>
			/// <param name="x">Local X coordinate on an 8x8 grid.</param>
			/// <param name="y">Local Y coordinate on an 8x8 grid.</param>
			inline constexpr VectorNode8x8::node_t Node8x8(const NodeEnum primitive, const uint8_t x, const uint8_t y)
			{
				return (static_cast<uint8_t>(primitive) << VectorNode8x8::CoordinatesSize)
					| ((x & VectorNode8x8::AxisMask) << VectorNode8x8::AxisSize)
					| (y & VectorNode8x8::AxisMask);
			}

			/// <summary>
			/// Packs a primitive opcode and 65x65 (shift-optimized) coordinates into a 16-bit node value.
			/// </summary>
			/// <param name="primitive">Primitive opcode.</param>
			/// <param name="x">Local X coordinate on a limited-range grid.</param>
			/// <param name="y">Local Y coordinate on a limited-range grid.</param>
			inline constexpr VectorNode65x65::node_t Node65x65(const NodeEnum primitive, const uint8_t x, const uint8_t y)
			{
				return (static_cast<uint16_t>(primitive) << VectorNode65x65::CoordinatesSize)
					| ((x & VectorNode65x65::AxisMask) << VectorNode65x65::AxisSize)
					| (y & VectorNode65x65::AxisMask);
			}

			/// <summary>
			/// Packs a primitive opcode and 128x128 coordinates into a 16-bit node value.
			/// </summary>
			/// <param name="primitive">Primitive opcode.</param>
			/// <param name="x">Local X coordinate on a 128x128 grid.</param>
			/// <param name="y">Local Y coordinate on a 128x128 grid.</param>
			inline constexpr VectorNode128x128::node_t Node128x128(const NodeEnum primitive, const uint8_t x, const uint8_t y)
			{
				return (static_cast<uint16_t>(primitive) << VectorNode128x128::CoordinatesSize)
					| ((x & VectorNode128x128::AxisMask) << VectorNode128x128::AxisSize)
					| (y & VectorNode128x128::AxisMask);
			}
		}
	}
}
#endif