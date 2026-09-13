#ifndef _EGFX_FRAMEWORK_SHADER_RADIAL_MODEL_h
#define _EGFX_FRAMEWORK_SHADER_RADIAL_MODEL_h

#include "../../EgfxCore.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Shader
		{
			namespace Radial
			{
				template<typename dimension_t>
				struct PolarCoordinate
				{
					using wide_t = typename IntegerSignal::TypeTraits::TypeNext::next_uint_type<dimension_t>::type;

					wide_t DistanceSquared = 0;
					angle_t AngleValue = 0;

					dimension_t GetDistance() const
					{
						if (DistanceSquared == 0)
							return 0;

						if (IntegerSignal::TypeTraits::TypeLimits::type_limits<wide_t>::Max() <= UINT16_MAX)
						{
							return IntegerSignal::SquareRoot16(DistanceSquared);
						}
						else
						{
							return IntegerSignal::SquareRoot32(DistanceSquared);
						}
					}
				};

				template<typename dimension_t>
				struct Calculator
				{
					using wide_t = typename IntegerSignal::TypeTraits::TypeNext::next_uint_type<dimension_t>::type;
					using signed_wide_t = typename IntegerSignal::TypeTraits::TypeNext::next_int_type<dimension_t>::type;

				public:
					using coordinate_t = PolarCoordinate<dimension_t>;

					static coordinate_t GetPolarCoordinate(const dimension_t x, const dimension_t y, const dimension_t offsetX, const dimension_t offsetY)
					{
						const signed_wide_t dx = static_cast<signed_wide_t>(x) - static_cast<signed_wide_t>(offsetX);
						const signed_wide_t dy = static_cast<signed_wide_t>(y) - static_cast<signed_wide_t>(offsetY);
						const wide_t ax = Absolute(dx);
						const wide_t ay = Absolute(dy);
						const wide_t radiusSquared = static_cast<wide_t>((ax * ax) + (ay * ay));

						return coordinate_t{
							radiusSquared,
							GetAngleInternal(dx, dy, ax, ay)
						};
					}

					static constexpr wide_t GetDistance(const wide_t distanceSquared)
					{
						using Use16BitSquareRoot = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeLimits::type_limits<wide_t>::Max() <= UINT16_MAX>::type;

						return GetDistance(distanceSquared, Use16BitSquareRoot{});
					}

					static wide_t GetDistanceSquared(const dimension_t x, const dimension_t y, const dimension_t offsetX, const dimension_t offsetY)
					{
						const signed_wide_t dx = static_cast<signed_wide_t>(x) - static_cast<signed_wide_t>(offsetX);
						const signed_wide_t dy = static_cast<signed_wide_t>(y) - static_cast<signed_wide_t>(offsetY);
						const wide_t ax = Absolute(dx);
						const wide_t ay = Absolute(dy);

						return static_cast<wide_t>((ax * ax) + (ay * ay));
					}

					static angle_t GetAngle(const dimension_t x, const dimension_t y,
						const dimension_t offsetX, const dimension_t offsetY)
					{
						const signed_wide_t dx = static_cast<signed_wide_t>(x) - static_cast<signed_wide_t>(offsetX);
						const signed_wide_t dy = static_cast<signed_wide_t>(y) - static_cast<signed_wide_t>(offsetY);
						const wide_t ax = Absolute(dx);
						const wide_t ay = Absolute(dy);

						return GetAngleInternal(dx, dy, ax, ay);
					}

				private:
					static constexpr wide_t GetDistance(const wide_t distanceSquared,
						IntegerSignal::TypeTraits::TypeDispatch::TrueType)
					{
						return IntegerSignal::SquareRoot16(distanceSquared);
					}

					static constexpr wide_t GetDistance(const wide_t distanceSquared,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType)
					{
						return IntegerSignal::SquareRoot32(distanceSquared);
					}

					static constexpr wide_t Absolute(const signed_wide_t value)
					{
						return value < 0 ? static_cast<wide_t>(-value) : static_cast<wide_t>(value);
					}

					static angle_t CalculateQuadrantAngle(const wide_t primary, const wide_t secondary)
					{
						using Use8BitScaling = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							IntegerSignal::TypeTraits::TypeLimits::type_limits<dimension_t>::Max() <= UINT8_MAX>::type;

						return CalculateQuadrantAngle(primary, secondary, Use8BitScaling{});
					}

					// Fast equivalent of primary / sum, scaled to ANGLE_QUARTER_CIRCLE, using 8-bit scaling for small types.
					static constexpr angle_t CalculateQuadrantAngle(const wide_t primary, const wide_t secondary,
						IntegerSignal::TypeTraits::TypeDispatch::TrueType)
					{
						return UFraction8::Fraction(
							UFraction8::GetScalar<wide_t>(primary, primary + secondary), ANGLE_QUARTER_CIRCLE);
					}

					// Fast equivalent of primary / sum, scaled to ANGLE_QUARTER_CIRCLE, using 16-bit scaling for larger types.
					static constexpr angle_t CalculateQuadrantAngle(const wide_t primary, const wide_t secondary,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType)
					{
						return UFraction16::Fraction(
							UFraction16::GetScalar<wide_t>(primary, primary + secondary), ANGLE_QUARTER_CIRCLE);
					}

					static angle_t GetAngleInternal(const signed_wide_t dx, const signed_wide_t dy,
						const wide_t ax, const wide_t ay)
					{
						const angle_t half = ANGLE_RANGE / 2;
						const angle_t threeQuarter = (static_cast<uint32_t>(ANGLE_RANGE) * 3) / 4;

						if (ax == 0 && ay == 0)
							return 0;

						// Quadrant 1 (e.g., dx >= 0, dy < 0)
						if (dx >= 0 && dy < 0)
						{
							return CalculateQuadrantAngle(ax, ay);
						}
						// Quadrant 2 (e.g., dx >= 0, dy >= 0)
						else if (dx >= 0 && dy >= 0)
						{
							return static_cast<angle_t>(ANGLE_QUARTER_CIRCLE + CalculateQuadrantAngle(ay, ax));
						}
						// Quadrant 3 (e.g., dx < 0, dy >= 0)
						else if (dx < 0 && dy >= 0)
						{
							return static_cast<angle_t>(half + CalculateQuadrantAngle(ax, ay));
						}
						// Quadrant 4 (e.g., dx < 0, dy < 0)
						else
						{
							return static_cast<angle_t>(threeQuarter + CalculateQuadrantAngle(ay, ax)) % ANGLE_RANGE;
						}
					}
				};
			}
		}
	}
}
#endif