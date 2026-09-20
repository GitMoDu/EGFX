#ifndef _INTEGERGLASS_MODULES_KIT_SHADER_SOURCE_GRADIENT_h
#define _INTEGERGLASS_MODULES_KIT_SHADER_SOURCE_GRADIENT_h

#include <IntegerGlassFramework.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Shader
			{
				namespace Source
				{
					/// <summary>
					/// Horizontal gradient color source (left->right) between ColorLeft and ColorRight.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Gradient width in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						bool LinearInterpolation = false>
					class GradientX
					{
					private:
						using InterpolationTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType,
							LinearInterpolation>::type;

						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to)
						{
							return Interpolate(fraction, from, to, InterpolationTag{});
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to,
							IntegerSignal::TypeTraits::TypeDispatch::TrueType) {
							return Rgb::InterpolateLinear(fraction, from, to);
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType) {
							return Rgb::Interpolate(fraction, from, to);
						}

					public:
						rgb_color_t ColorLeft = RGB_COLOR_WHITE;
						rgb_color_t ColorRight = RGB_COLOR_BLACK;

					public:
						GradientX() = default;
						~GradientX() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t /*y*/)
						{
							if (Width <= 1)
							{
								return ColorLeft;
							}

							const ufraction16_t fraction = UFraction16::GetScalar<dimension_t>(x, static_cast<dimension_t>(Width - 1));
							return Interpolate(fraction, ColorLeft, ColorRight);
						}
					};

					/// <summary>
					/// Vertical gradient color source (top->bottom) between ColorTop and ColorBottom.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Height">Gradient height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Height,
						bool LinearInterpolation = false>
					class GradientY
					{
					private:
						using InterpolationTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<IntegerSignal::TypeTraits::TypeDispatch::TrueType, IntegerSignal::TypeTraits::TypeDispatch::FalseType, LinearInterpolation>::type;
						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to)
						{
							return Interpolate(fraction, from, to, InterpolationTag{});
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::TrueType) { return Rgb::InterpolateLinear(fraction, from, to); }
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::FalseType) { return Rgb::Interpolate(fraction, from, to); }

					public:
						rgb_color_t ColorTop = RGB_COLOR_WHITE;
						rgb_color_t ColorBottom = RGB_COLOR_BLACK;

					public:
						GradientY() = default;
						~GradientY() = default;

						rgb_color_t Source(const dimension_t /*x*/, const dimension_t y)
						{
							if (Height <= 1)
							{
								return ColorTop;
							}

							const ufraction16_t fraction = UFraction16::GetScalar<dimension_t>(y, static_cast<dimension_t>(Height - 1));
							return Interpolate(fraction, ColorTop, ColorBottom);
						}
					};

					/// <summary>
					/// Combined 2D gradient source (bilinear blend).
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Gradient width in pixels.</typeparam>
					/// <typeparam name="Height">Gradient height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool LinearInterpolation = false>
					class GradientXY
					{
					private:
						using InterpolationTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<IntegerSignal::TypeTraits::TypeDispatch::TrueType, IntegerSignal::TypeTraits::TypeDispatch::FalseType, LinearInterpolation>::type;
						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to)
						{
							return Interpolate(fraction, from, to, InterpolationTag{});
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::TrueType) { return Rgb::InterpolateLinear(fraction, from, to); }
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::FalseType) { return Rgb::Interpolate(fraction, from, to); }

					public:
						rgb_color_t ColorTopLeft = RGB_COLOR_BLACK;
						rgb_color_t ColorTopRight = RGB_COLOR_WHITE;
						rgb_color_t ColorBottomLeft = RGB_COLOR_WHITE;
						rgb_color_t ColorBottomRight = RGB_COLOR_BLACK;

					public:
						GradientXY() = default;
						~GradientXY() = default;

						rgb_color_t Source(const dimension_t x, const dimension_t y)
						{
							const ufraction16_t fx = (Width <= 1)
								? static_cast<ufraction16_t>(0)
								: UFraction16::GetScalar<dimension_t>(x, static_cast<dimension_t>(Width - 1));

							const ufraction16_t fy = (Height <= 1)
								? static_cast<ufraction16_t>(0)
								: UFraction16::GetScalar<dimension_t>(y, static_cast<dimension_t>(Height - 1));

							const rgb_color_t top = Interpolate(fx, ColorTopLeft, ColorTopRight);
							const rgb_color_t bot = Interpolate(fx, ColorBottomLeft, ColorBottomRight);
							return Interpolate(fy, top, bot);
						}
					};

					/// <summary>
					/// Radial gradient source (center -> edge) between ColorCenter and ColorEdge.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels.</typeparam>
					/// <typeparam name="Height">Source height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool LinearInterpolation = false>
					class GradientRadial
					{
					private:
						using InterpolationTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<IntegerSignal::TypeTraits::TypeDispatch::TrueType, IntegerSignal::TypeTraits::TypeDispatch::FalseType, LinearInterpolation>::type;
						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to)
						{
							return Interpolate(fraction, from, to, InterpolationTag{});
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::TrueType) { return Rgb::InterpolateLinear(fraction, from, to); }
						static rgb_color_t Interpolate(const ufraction16_t fraction, const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::FalseType) { return Rgb::Interpolate(fraction, from, to); }

						using Radial = Framework::Shader::Radial::Calculator<dimension_t>;
						using wide_t = typename Radial::wide_t;

					public:
						rgb_color_t ColorCenter = RGB_COLOR_WHITE;
						rgb_color_t ColorEdge = RGB_COLOR_BLACK;

					private:
						// Center offset (local coordinate space).
						dimension_t OffsetX = Width / 2;
						dimension_t OffsetY = Height / 2;

						// Radius in pixels (local coordinate space).
						dimension_t Radius = 1;

					public:
						GradientRadial() {}

						~GradientRadial() = default;

						/// <summary>
						/// Set the gradient center offset in pixels (local coords).
						/// </summary>
						void SetCenter(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = LimitValue<dimension_t, 0, static_cast<dimension_t>(Width - 1)>(offsetX);
							OffsetY = LimitValue<dimension_t, 0, static_cast<dimension_t>(Height - 1)>(offsetY);
						}

						/// <summary>
						/// Set the gradient radius in pixels.
						/// Radius 0 collapses to ColorCenter.
						/// </summary>
						void SetRadius(const dimension_t radius)
						{
							Radius = radius;
						}

						/// <summary>
						/// Get the gradient radius in pixels.
						/// </summary>
						/// <returns>The gradient radius in pixels.</returns>
						dimension_t GetRadius() const
						{
							return Radius;
						}

						rgb_color_t Source(const dimension_t x, const dimension_t y) const
						{
							const wide_t distanceSquared = Radial::GetDistanceSquared(x, y, OffsetX, OffsetY);
							const dimension_t distance = Radial::GetDistance(distanceSquared);
							const ufraction16_t fraction = (Radius == 0) ? 0 : UFraction16::GetScalar<dimension_t>(distance, Radius);

							return Interpolate(fraction, ColorCenter, ColorEdge);
						}
					};

					/// <summary>
					/// Angular gradient source rotating clockwise around a runtime center.
					/// </summary>
					/// <typeparam name="dimension_t">The shader's intrinsic dimension type.</typeparam>
					/// <typeparam name="Width">Source width in pixels.</typeparam>
					/// <typeparam name="Height">Source height in pixels.</typeparam>
					template<typename dimension_t,
						dimension_t Width,
						dimension_t Height,
						bool LinearInterpolation = false>
					class GradientAngular
					{
					private:
						using Radial = Framework::Shader::Radial::Calculator<dimension_t>;
						using coordinate_t = typename Radial::coordinate_t;

						using InterpolationTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
							IntegerSignal::TypeTraits::TypeDispatch::TrueType,
							IntegerSignal::TypeTraits::TypeDispatch::FalseType, LinearInterpolation>::type;

					private:
						dimension_t OffsetX = Width / 2;
						dimension_t OffsetY = Height / 2;
						angle_t StartAngle = 0;
						angle_t EndAngle = ANGLE_RANGE;
						angle_t NormalizedStartAngle = 0;
						angle_t SweepAngle = ANGLE_RANGE;


					private:
						rgb_color_t ColorStart = RGB_COLOR_BLACK;
						rgb_color_t ColorEnd = RGB_COLOR_WHITE;

					public:
						GradientAngular() {}
						~GradientAngular() = default;

						void SetColors(const rgb_color_t colorStart, const rgb_color_t colorEnd)
						{
							ColorStart = colorStart;
							ColorEnd = colorEnd;
						}

						void SetCenter(const dimension_t offsetX, const dimension_t offsetY)
						{
							OffsetX = LimitValue<dimension_t, 0, static_cast<dimension_t>(Width - 1)>(offsetX);
							OffsetY = LimitValue<dimension_t, 0, static_cast<dimension_t>(Height - 1)>(offsetY);
						}

						void SetStartAngle(const angle_t startAngle)
						{
							StartAngle = startAngle;
							UpdateAngles();
						}

						angle_t GetStartAngle() const
						{
							return StartAngle;
						}

						void SetEndAngle(const angle_t endAngle)
						{
							EndAngle = endAngle;
							UpdateAngles();
						}

						angle_t GetEndAngle() const
						{
							return EndAngle;
						}

						rgb_color_t Source(const dimension_t x, const dimension_t y) const
						{
							if (SweepAngle == 0)
							{
								return ColorStart;
							}
							else
							{
								const angle_t angle = Radial::GetAngle(x, y, OffsetX, OffsetY);
								const angle_t relativeAngle = static_cast<angle_t>((angle + ANGLE_RANGE - NormalizedStartAngle) % ANGLE_RANGE);
								const angle_t limitedAngle = MinValue(relativeAngle, SweepAngle);
								const ufraction16_t fraction = UFraction16::GetScalar<angle_t>(limitedAngle, SweepAngle);

								return Interpolate(fraction, ColorStart, ColorEnd);
							}
						}

					private:
						void UpdateAngles()
						{
							const angle_t range = static_cast<angle_t>(ANGLE_RANGE);
							NormalizedStartAngle = static_cast<angle_t>(StartAngle % range);
							const angle_t normalizedEndAngle = static_cast<angle_t>(EndAngle % range);
							const bool fullRange = EndAngle > StartAngle &&
								static_cast<angle_t>(EndAngle - StartAngle) >= range;
							SweepAngle = fullRange ? range : static_cast<angle_t>((normalizedEndAngle + range - NormalizedStartAngle) % range);
						}

						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to)
						{
							return Interpolate(fraction, from, to, InterpolationTag{});
						}
						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
						{
							return Rgb::InterpolateLinear(fraction, from, to);
						}

						static rgb_color_t Interpolate(const ufraction16_t fraction,
							const rgb_color_t from, const rgb_color_t to, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
						{
							return Rgb::Interpolate(fraction, from, to);
						}
					};
				}
			}
		}
	}
}
#endif