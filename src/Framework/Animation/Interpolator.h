#ifndef _INTEGERGLASS_FRAMEWORK_PROJECTION_ANIMATION_INTERPOLATOR_h
#define _INTEGERGLASS_FRAMEWORK_PROJECTION_ANIMATION_INTERPOLATOR_h

#include "Model.h"

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Animation
		{
			namespace Interpolator
			{
				using strength_t = uint8_t;

				static constexpr progress_t CURVE_SCALAR_UNIT = UFraction16::SCALAR_UNIT;
				static constexpr progress_t CURVE_SCALAR_HALF_UNIT = CURVE_SCALAR_UNIT / 2;

				static constexpr strength_t STRENGTH_DEFAULT = UINT8_MAX / 2;
				static constexpr strength_t STRENGTH_MAX = UINT8_MAX;

				struct IInterpolator
				{
					IInterpolator() = default;
					virtual ~IInterpolator() = default;

					virtual progress_wide_t Get(const progress_t value) const = 0;
				};

				template<typename InterpolatorType>
				struct InterpolatorAdapter : public IInterpolator
				{
					InterpolatorAdapter() : IInterpolator()
					{}
					virtual ~InterpolatorAdapter() = default;

					progress_wide_t Get(const progress_t value) const override
					{
						return InterpolatorType::Get(value);
					}
				};

				struct Linear
				{
					static constexpr progress_t Get(const progress_t value)
					{
						return value;
					}
				};


				static InterpolatorAdapter<Linear> NoInterpolator{};

				template<typename InInterpolator,
					typename OutInterpolator>
				struct TemplateInOut
				{
					static constexpr progress_t SecondHalfUnit = CURVE_SCALAR_UNIT - CURVE_SCALAR_HALF_UNIT;

					static progress_wide_t Get(const progress_t value)
					{
						if (value < CURVE_SCALAR_HALF_UNIT)
						{
							const progress_t input = value * 2;
							return static_cast<progress_wide_t>(
								(static_cast<int64_t>(InInterpolator::Get(input)) * CURVE_SCALAR_HALF_UNIT) /
								CURVE_SCALAR_UNIT);
						}

						const progress_t input = MinValue<progress_t>(
							(value - CURVE_SCALAR_HALF_UNIT) * 2, CURVE_SCALAR_UNIT);

						return static_cast<progress_wide_t>(CURVE_SCALAR_HALF_UNIT +
							(static_cast<int64_t>(OutInterpolator::Get(input)) * SecondHalfUnit) /
							CURVE_SCALAR_UNIT);
					}
				};
			}
		}
	}
}
#endif