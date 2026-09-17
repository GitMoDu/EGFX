#ifndef _EGFX_MODULES_KIT_ANIMATION_INTERPOLATOR_EASE_IN_OUT_h
#define _EGFX_MODULES_KIT_ANIMATION_INTERPOLATOR_EASE_IN_OUT_h

#include <EgfxFramework.h>

namespace Egfx
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

					template<uint8_t Power>
					struct PowerValue
					{
						static constexpr int64_t Get(const int64_t value, const int64_t unit)
						{
							return (value * PowerValue<Power - 1>::Get(value, unit)) / unit;
						}
					};

					template<>
					struct PowerValue<0>
					{
						static constexpr int64_t Get(const int64_t, const int64_t unit)
						{
							return unit;
						}
					};

					template<uint8_t Power>
					struct PowerIn
					{
						static constexpr progress_wide_t Get(const progress_t value)
						{
							return static_cast<progress_wide_t>(PowerValue<Power>::Get(value, CURVE_SCALAR_UNIT));
						}
					};

					template<uint8_t Power>
					struct PowerOut
					{
						static constexpr progress_wide_t Get(const progress_t value)
						{
							return static_cast<progress_wide_t>(CURVE_SCALAR_UNIT -
								PowerValue<Power>::Get(CURVE_SCALAR_UNIT - value, CURVE_SCALAR_UNIT));
						}
					};


					using EaseIn = PowerIn<2>;
					using EaseOut = PowerOut<2>;
					using CubicIn = PowerIn<3>;
					using CubicOut = PowerOut<3>;

					using EaseInOut = TemplateInOut<EaseIn, EaseOut>;

					using CubicInOut = TemplateInOut<CubicIn, CubicOut>;


				}
			}
		}
	}
}

#endif
