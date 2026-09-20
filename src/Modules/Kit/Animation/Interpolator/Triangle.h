#ifndef _INTEGERGLASS_MODULES_KIT_ANIMATION_INTERPOLATOR_TRIANGLE_h
#define _INTEGERGLASS_MODULES_KIT_ANIMATION_INTERPOLATOR_TRIANGLE_h

#include <IntegerGlassFramework.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Kit
		{
			namespace Animation
			{
				namespace Interpolator
				{
					using namespace Framework::Animation;
					using namespace Framework::Animation::Interpolator;

					template<typename InInterpolator = Linear,
						typename OutInterpolator = Linear>
					struct Triangle
					{
						static progress_wide_t FirstHalf(const progress_t value)
						{
							return InInterpolator::Get(static_cast<progress_t>(value) * 2);
						}

						static progress_wide_t SecondHalf(const progress_t value)
						{
							const progress_t input = static_cast<progress_t>(
								(value - CURVE_SCALAR_HALF_UNIT) * 2);

							return static_cast<progress_wide_t>(CURVE_SCALAR_UNIT) -
								OutInterpolator::Get(input);
						}

						static progress_wide_t Get(const progress_t value)
						{
							return value < CURVE_SCALAR_HALF_UNIT
								? FirstHalf(value)
								: SecondHalf(value);
						}
					};
				}
			}
		}
	}
}

#endif
