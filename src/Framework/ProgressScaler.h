#ifndef _INTEGERGLASS_PROGRESS_SCALER_h
#define _INTEGERGLASS_PROGRESS_SCALER_h

#include <stdint.h>

namespace IntegerGlass
{
	namespace Framework
	{
		namespace Animation
		{
			namespace TimeCurve
			{
				/// <summary>
				/// Turns a linear time value into a sawtooth wave value that oscillates between 0 and period.
				/// </summary>
				/// <typeparam name="period">The period of the sawtooth wave.</typeparam>
				/// <param name="elapsed">The elapsed time.</param>
				/// <returns>The sawtooth wave value.</returns>
				template<uint32_t period>
				inline constexpr uint32_t Sawtooth(const uint32_t elapsed)
				{
					return elapsed % period;
				}

				/// <summary>
				/// Turns a linear time value into a sawtooth wave value that oscillates between 0 and period.
				/// </summary>
				/// <param name="elapsed">The elapsed time.</param>
				/// <param name="period">The period of the sawtooth wave.</param>
				/// <returns>The sawtooth wave value.</returns>
				inline constexpr uint32_t Sawtooth(const uint32_t elapsed, const uint32_t period)
				{
					return elapsed % period;
				}

				namespace TriangleDetail
				{
					template<typename EvaluationTag>
					struct Policy;

					template<>
					struct Policy<IntegerSignal::TypeTraits::Evaluation::ConstantEvaluatedType>
					{
						static constexpr uint32_t Get(const uint32_t elapsed, const uint32_t period)
						{
							return (Sawtooth(elapsed, period) < period / 2)
								? (Sawtooth(elapsed, period) * 2)
								: (period - ((Sawtooth(elapsed, period) - (period / 2)) * 2));
						}
					};

					template<>
					struct Policy<IntegerSignal::TypeTraits::Evaluation::RuntimeEvaluatedType>
					{
						template<uint32_t period>
						static constexpr uint32_t Get(const uint32_t elapsed)
						{
							constexpr uint32_t half_period = period / 2;
							const uint32_t modded = Sawtooth<period>(elapsed);
							return (modded < half_period)
								? (modded * 2)
								: (period - ((modded - half_period) * 2));
						}

						static constexpr uint32_t Get(const uint32_t elapsed, const uint32_t period)
						{
							return ((Sawtooth(elapsed, period)) < (period / 2))
								? ((Sawtooth(elapsed, period)) * 2)
								: (period - (((Sawtooth(elapsed, period)) - (period / 2)) * 2));
						}
					};

					template<typename EvaluationTag>
					static constexpr uint32_t Get(const uint32_t elapsed, const uint32_t period, EvaluationTag)
					{
						return Policy<EvaluationTag>::Get(elapsed, period);
					}
				}


				/// <summary>
				/// Turns a linear time value into a square wave value that oscillates between 0 and period.
				/// </summary>
				/// <typeparam name="period">The period of the square wave.</typeparam>
				/// <param name="elapsed">The elapsed time.</param>
				/// <returns>The square wave value.</returns>
				template<uint32_t period>
				inline constexpr uint32_t Square(uint32_t elapsed)
				{
					return (Sawtooth(elapsed, period) < period / 2) ? 0 : period;
				}

				/// <summary>
				/// Inverts a value within a given period.
				/// </summary>
				/// <param name="value">The value to invert.</param>
				/// <param name="period">The period within which to invert the value.</param>
				/// <returns>The inverted value.</returns>
				inline constexpr uint32_t Invert(const uint32_t value, const uint32_t period)
				{
					return period - value;
				}

				/// <summary>
				/// Turns a linear time value into a triangle wave value that oscillates between 0 and period.
				/// </summary>
				/// <typeparam name="period">The period of the triangle wave.</typeparam>
				/// <param name="elapsed">The elapsed time.</param>
				/// <returns>The triangle wave value.</returns>
				template<uint32_t period>
				inline constexpr uint32_t Triangle(const uint32_t elapsed)
				{
					return TriangleDetail::Get(elapsed, period,
						IntegerSignal::TypeTraits::Evaluation::GetEvaluationTag());
				}

				/// <summary>
				/// Turns a linear time value into a triangle wave value that oscillates between 0 and period.
				/// </summary>
				/// <param name="elapsed">The elapsed time.</param>
				/// <param name="period">The period of the triangle wave.</param>
				/// <returns>The triangle wave value.</returns>
				inline constexpr uint32_t Triangle(const uint32_t elapsed, const uint32_t period)
				{
					return TriangleDetail::Get(elapsed, period,
						IntegerSignal::TypeTraits::Evaluation::GetEvaluationTag());
				}

				/// <summary>
				/// Performs an exponential decay step towards a target value.
				/// </summary>
				/// <param name="current">The current value.</param>
				/// <param name="target">The target value.</param>
				/// <returns>The updated value after the exponential decay step.</returns>
				inline constexpr uint32_t ExponentialDecayStep(uint32_t current, uint32_t target)
				{
					return current + ((target > current) ? ((target - current) >> 3) : -((current - target) >> 3));
				}
			}
		}
	}

	struct ProgressScaler
	{
		template<const uint32_t PeriodMicros>
		static constexpr uint16_t GetProgress(const uint32_t frameTime)
		{
			return ((uint64_t)(frameTime % PeriodMicros) * UINT16_MAX) / PeriodMicros;
		}

		static constexpr uint16_t GetProgress(const uint32_t periodMicros, const uint32_t frameTime)
		{
			return uint16_t(((uint64_t)(frameTime % periodMicros) * UINT16_MAX) / periodMicros);
		}

		static constexpr uint8_t ScaleProgress(const uint16_t progress, const uint8_t value)
		{
			return uint8_t(ScaleProgress(progress, (uint16_t)value));
		}

		static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint16_t value)
		{
			return uint16_t((((uint32_t)progress) * value) / UINT16_MAX);
		}

		static constexpr uint16_t ScaleProgress(const uint16_t progress, const uint32_t value)
		{
			return uint16_t((((uint64_t)progress) * value) / UINT16_MAX);
		}

		static uint16_t TriangleResponse(const uint16_t progress)
		{
			if (progress < ((uint16_t)INT16_MAX + 1))
			{
				return progress * 2;
			}
			else
			{
				return UINT16_MAX - (progress * 2);
			}
		}
	};
}
#endif