#ifndef _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_ANTICIPATE_OVERSHOOT_h
#define _EGFX_FRAMEWORK_ASSETS_ANIMATION_INTERPOLATOR_ANTICIPATE_OVERSHOOT_h

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

					template<uint8_t Strength = STRENGTH_DEFAULT>
					struct Antecipate
					{
						static constexpr int32_t StrengthValue = static_cast<int32_t>(CURVE_SCALAR_UNIT) +
							(static_cast<int32_t>(Strength) * CURVE_SCALAR_UNIT) / UINT8_MAX;

						static constexpr progress_wide_t Get(const progress_t value)
						{
							if (value <= 0)
								return 0;
							if (value >= CURVE_SCALAR_UNIT)
								return CURVE_SCALAR_UNIT;

							const int64_t square =
								(static_cast<int64_t>(value) * value) / CURVE_SCALAR_UNIT;
							const int64_t shape =
								(((static_cast<int64_t>(CURVE_SCALAR_UNIT) + StrengthValue) * value) -
									(static_cast<int64_t>(StrengthValue) * CURVE_SCALAR_UNIT)) /
								CURVE_SCALAR_UNIT;

							return static_cast<progress_wide_t>((square * shape) / CURVE_SCALAR_UNIT);
						}
					};

					template<uint8_t Strength = STRENGTH_DEFAULT>
					struct Overshoot
					{
						static constexpr int32_t StrengthValue = static_cast<int32_t>(CURVE_SCALAR_UNIT) +
							(static_cast<int32_t>(Strength) * CURVE_SCALAR_UNIT) / UINT8_MAX;


						static constexpr progress_wide_t Get(const progress_t value)
						{
							if (value <= 0)
								return 0;
							if (value >= CURVE_SCALAR_UNIT)
								return CURVE_SCALAR_UNIT;

							const int64_t offset = static_cast<int64_t>(value) - CURVE_SCALAR_UNIT;
							const int64_t offsetMagnitude = -offset;
							const int64_t square =
								(offsetMagnitude * offsetMagnitude) / CURVE_SCALAR_UNIT;
							const int64_t shape =
								(((static_cast<int64_t>(CURVE_SCALAR_UNIT) + StrengthValue) * offset) +
									(static_cast<int64_t>(StrengthValue) * CURVE_SCALAR_UNIT)) /
								CURVE_SCALAR_UNIT;

							return static_cast<progress_wide_t>(CURVE_SCALAR_UNIT +
								((square * shape) / CURVE_SCALAR_UNIT));
						}
					};

					template<uint8_t AntecipateStrength = STRENGTH_DEFAULT, uint8_t OvershootStrength = STRENGTH_DEFAULT>
					using AntecipateOvershoot = TemplateInOut<Antecipate<AntecipateStrength>, Overshoot<OvershootStrength>>;

				}
			}
		}
	}
}

#endif
