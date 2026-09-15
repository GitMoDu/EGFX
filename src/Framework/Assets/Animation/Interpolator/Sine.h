#ifndef _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_SINE_h
#define _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_SINE_h

#include "../../../Animation/Interpolator.h"
#include <IntegerTrigonometry16.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace Animation
			{
				namespace Interpolator
				{
					using namespace Framework::Animation;
					using namespace Framework::Animation::Interpolator;

					struct Sine
					{
						static progress_wide_t Get(const progress_t value)
						{
							if (value >= CURVE_SCALAR_UNIT)
								return 0;

							const angle_t angle = FixedPoint::ScalarFraction::UFraction16::Fraction<angle_t>(
								value, ANGLE_RANGE);
							const auto sine = IntegerSignal::Trigonometry::Sine16(angle + ANGLE_270);
							const auto centered = static_cast<UFraction16::scalar_t>(
								sine + Fraction16::FRACTION_1X);

							return FixedPoint::ScalarFraction::UFraction16::Fraction<progress_wide_t>(
								centered, CURVE_SCALAR_UNIT);
						}
					};
				}
			}
		}
	}
}

#endif
