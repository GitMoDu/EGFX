#ifndef _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_TRIANGLE_h
#define _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_TRIANGLE_h

#include "../../../Animation/Interpolator.h"

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
