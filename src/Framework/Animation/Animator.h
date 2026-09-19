#ifndef _EGFX_FRAMEWORK_ANIMATION_ANIMATOR_h
#define _EGFX_FRAMEWORK_ANIMATION_ANIMATOR_h

#include "Model.h"
#include "Interpolator.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Animation
		{
			template<typename value_t>
			class Animator
			{
			public:
				using animation_state_t = Animation::animation_state_t<value_t>;

			private:
				using calculation_t = typename IntegerSignal::TypeTraits::TypeNext::next_int_type<value_t>::type;

				static constexpr calculation_t ValueMinimum = static_cast<calculation_t>(
					IntegerSignal::TypeTraits::TypeLimits::type_limits<value_t>::Min());

				static constexpr calculation_t ValueMaximum = static_cast<calculation_t>(
					IntegerSignal::TypeTraits::TypeLimits::type_limits<value_t>::Max());

				const Interpolator::IInterpolator* Interpolator = &Interpolator::NoInterpolator;

				uint32_t StartTime = 0;
				uint32_t Duration = 0;

				value_t StartValue{};
				value_t TargetValue{};

				bool IsActive = false;
				bool IsStarted = false;
				bool Loop = false;

			public:
				void Start(const value_t startValue, const value_t targetValue,
					const uint32_t duration)
				{
					StartValue = startValue;
					TargetValue = targetValue;
					Duration = duration;
					IsActive = true;
					IsStarted = false;
				}

				void Start(const value_t startValue, const value_t targetValue)
				{
					StartValue = startValue;
					TargetValue = targetValue;
					IsActive = true;
					IsStarted = false;
				}

				void SetDuration(const uint32_t duration)
				{
					Duration = duration;
				}

				void SetLoop(const bool loop)
				{
					Loop = loop;
				}

				bool IsLooping() const
				{
					return Loop;
				}

				uint32_t GetDuration() const
				{
					return Duration;
				}

				void Cancel()
				{
					IsActive = false;
					IsStarted = false;
				}

				bool IsAnimating() const
				{
					return IsActive;
				}

				void SetInterpolator(const Interpolator::IInterpolator& interpolator)
				{
					Interpolator = &interpolator;
				}

				void SetInterpolator(const Interpolator::IInterpolator* interpolator)
				{
					Interpolator = interpolator != nullptr ? interpolator : &Interpolator::NoInterpolator;
				}

				animation_state_t Step(const uint32_t currentTime)
				{
					if (!IsActive || Interpolator == nullptr)
						return { UFraction16::SCALAR_UNIT, UFraction16::SCALAR_UNIT, TargetValue };

					if (!IsStarted)
					{
						StartTime = currentTime;
						IsStarted = true;
					}

					const uint32_t elapsed = currentTime - StartTime;
					if (elapsed >= Duration)
					{
						if (Loop)
						{
							StartTime = currentTime;
							return { 0, Interpolator->Get(0), StartValue };
						}
						else
						{
							IsActive = false;
						}
						return { UFraction16::SCALAR_UNIT, UFraction16::SCALAR_UNIT, TargetValue };
					}

					const progress_t progress = UFraction16::GetScalar<uint32_t>(elapsed, Duration);
					const progress_wide_t curveProgress = Interpolator->Get(progress);


					const value_t value = InterpolateValue(curveProgress, StartValue, TargetValue);

					return { progress, curveProgress, value };
				}

			private:
				static value_t InterpolateValue(const progress_wide_t curveProgress,
					const value_t startValue, const value_t targetValue)
				{
					const calculation_t scalarUnit = UFraction16::SCALAR_UNIT;
					calculation_t whole = curveProgress / scalarUnit;
					calculation_t remainder = curveProgress % scalarUnit;

					if (remainder < 0)
					{
						--whole;
						remainder += scalarUnit;
					}

					const calculation_t start = static_cast<calculation_t>(startValue);
					const calculation_t delta = static_cast<calculation_t>(targetValue) - start;
					const calculation_t interpolated = start + delta * whole +
						Fraction<calculation_t>(static_cast<progress_t>(remainder), delta);

					return static_cast<value_t>(LimitValue<calculation_t, ValueMinimum, ValueMaximum>(interpolated));
				}
			};
		}
	}
}
#endif