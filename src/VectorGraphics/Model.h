#ifndef _EGFX_VECTOR_GRAPHICS_MODEL_h
#define _EGFX_VECTOR_GRAPHICS_MODEL_h

#include "../Platform/Platform.h"
#include <IntegerSignal.h>

namespace Egfx
{
	/// <summary>
	/// Compact node encodings for vector graphics streams.
	/// Provides primitive opcodes plus packing/decoding/scaling helpers for multiple models.
	///
	/// Models:
	/// - 8-bit (1 byte/node): 2-bit primitive + 3-bit X + 3-bit Y on an 8×8 grid [0..7].
	/// - 16-bit (2 bytes/node):
	///   - VectorNode65x65: shift-optimized, limited coordinate range for fast right-shift scaling.
	///   - VectorNode128x128: full 7-bit X/Y on a 128×128 grid [0..127], scaled via division.
	///
	/// Primitives:
	/// - Start: begin/continue a segment; draws a line from previous to current when a previous point exists.
	/// - End: terminate a segment; draws a line when previous != current, otherwise a single pixel; then resets state.
	/// - TriangleFill: triangle strip (sliding window) using (prev2, prev1, current) -> filled triangle.
	/// - RectangleFill: filled axis-aligned rectangle using last and current points as opposite corners.
	/// </summary>
	namespace VectorGraphics
	{
		using namespace IntegerSignal::FixedPoint;

		/// <summary>
		/// Primitive commands (2-bit values).
		/// </summary>
		enum class NodeEnum
		{
			Start = 0b00,			// Begin/continue a segment; may draw a line from the previous point.
			End = 0b01,				// End a segment; draw line/pixel from the previous point, then reset state.
			TriangleFill = 0b10,	// Triangle strip: (prev2, prev1, current) -> filled triangle.
			RectangleFill = 0b11,	// Filled rectangle between the previous and current points.
			EnumCount
		};

		// 2 bits reserved for the primitive opcode.
		static constexpr uint8_t PrimitiveMask = uint8_t(NodeEnum::EnumCount) - 1;

		/// <summary>
		/// 8-bit model encoding: 2-bit primitive + 3-bit X + 3-bit Y (8×8 grid).
		/// </summary>
		struct VectorNode8x8
		{
			// 1 byte per node.
			using node_t = uint8_t;
			using axis_t = uint8_t;

			// 6 bits total for both coordinates.
			static constexpr uint8_t CoordinatesSize = GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

			// 3 bits per axis.
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
			/// Scale a local axis value [0..AxisMax] to [0..dimension] using integer math.
			/// </summary>
			template<typename dimension_t>
			static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
			{
				using high_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;
				return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) / AxisMax);
			}

			template<typename dimension_t>
			static constexpr dimension_t GetScaledAxisX(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisX(node));
			}
			template<typename dimension_t>
			static constexpr dimension_t GetScaledAxisY(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisY(node));
			}
		};

		/// <summary>
		/// 16-bit model (shift-optimized): 2-bit primitive + limited-range X/Y for fast scaling (right shift).
		/// Coordinates are constrained to favor speed over full 7-bit range.
		/// </summary>
		struct VectorNode65x65
		{
			// 2 bytes per node.
			using node_t = uint16_t;
			using axis_t = uint8_t;

			// 14 bits total for both coordinates.
			static constexpr uint8_t CoordinatesSize = GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

			// 7 bits per axis (with limited effective range for shift-based scaling).
			static constexpr uint8_t AxisSize = CoordinatesSize / 2;

			// Limited size enables faster scaling without division.
			// Note: AxisMax/AxisMask intentionally do not represent the full 7-bit mask.
			static constexpr uint16_t AxisMax = 1 << (AxisSize - 1);
			static constexpr uint16_t AxisMask = (1 << (AxisSize - 1));

			static constexpr uint16_t GetAxisX(const uint16_t node)
			{
				return (node >> AxisSize) & AxisMask;
			}
			static inline constexpr uint16_t GetAxisY(const uint16_t node)
			{
				return node & AxisMask;
			}

			/// <summary>
			/// Scale via right shift: (dimension * axis) >> AxisSize.
			/// </summary>
			template<typename dimension_t>
			static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
			{
				using large_t = typename TypeTraits::TypeConditional::larger_type<dimension_t, uint16_t>::type;
				using high_t = typename TypeTraits::TypeNext::next_uint_type<large_t>::type;
				return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) >> AxisSize);
			}

			template<typename dimension_t>
			static constexpr axis_t GetScaledAxisX(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisX(node));
			}
			template<typename dimension_t>
			static constexpr axis_t GetScaledAxisY(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisY(node));
			}

			static inline constexpr NodeEnum GetPrimitive(const uint16_t node)
			{
				return static_cast<NodeEnum>((node >> CoordinatesSize) & PrimitiveMask);
			}
		};

		/// <summary>
		/// 16-bit model (full range): 2-bit primitive + 7-bit X + 7-bit Y (128×128 grid).
		/// Uses division for scaling to preserve the full 0..127 range.
		/// </summary>
		struct VectorNode128x128
		{
			using node_t = uint16_t; // 2 bytes per node.
			using axis_t = uint8_t;  // Axis max (127) fits in uint8_t.

			// 14 bits total for both coordinates.
			static constexpr uint8_t CoordinatesSize = GetBitShifts(TypeTraits::TypeLimits::type_limits<node_t>::Max()) - GetBitShifts(PrimitiveMask);

			static constexpr uint8_t AxisSize = CoordinatesSize / 2; // 7 bits per axis.
			static constexpr axis_t AxisMax = (1 << AxisSize) - 1;   // 127
			static constexpr uint8_t AxisMask = AxisMax;             // 0b1111111

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

			template<typename dimension_t>
			static constexpr dimension_t GetScaled(const dimension_t dimension, const axis_t axis)
			{
				using high_t = typename TypeTraits::TypeNext::next_uint_type<dimension_t>::type;
				return static_cast<dimension_t>((static_cast<high_t>(dimension) * axis) / AxisMax);
			}

			template<typename dimension_t>
			static constexpr dimension_t GetScaledAxisX(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisX(node));
			}

			template<typename dimension_t>
			static constexpr dimension_t GetScaledAxisY(const dimension_t dimension, const node_t node)
			{
				return GetScaled(dimension, GetAxisY(node));
			}
		};

		/// <summary>
		/// Pack 8-bit primitive + 8×8 coordinates into a single byte.
		/// </summary>
		inline constexpr VectorNode8x8::node_t Node8x8(const NodeEnum primitive, const uint8_t x, const uint8_t y)
		{
			return (static_cast<uint8_t>(primitive) << VectorNode8x8::CoordinatesSize)
				| ((x & VectorNode8x8::AxisMask) << VectorNode8x8::AxisSize)
				| (y & VectorNode8x8::AxisMask);
		}

		/// <summary>
		/// Pack 16-bit primitive + shift-optimized coordinates into a single 16-bit value.
		/// </summary>
		inline constexpr VectorNode65x65::node_t Node65x65(const NodeEnum primitive, const uint8_t x, const uint8_t y)
		{
			return (static_cast<uint16_t>(primitive) << VectorNode65x65::CoordinatesSize)
				| ((x & VectorNode65x65::AxisMask) << VectorNode65x65::AxisSize)
				| (y & VectorNode65x65::AxisMask);
		}

		/// <summary>
		/// Pack 16-bit primitive + 128×128 coordinates into a single 16-bit value.
		/// </summary>
		inline constexpr VectorNode128x128::node_t Node128x128(const NodeEnum primitive, const uint8_t x, const uint8_t y)
		{
			return (static_cast<uint16_t>(primitive) << VectorNode128x128::CoordinatesSize)
				| ((x & VectorNode128x128::AxisMask) << VectorNode128x128::AxisSize)
				| (y & VectorNode128x128::AxisMask);
		}
	}
}
#endif