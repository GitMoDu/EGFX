#ifndef _INTEGERGLASS_MODULES_PLOT_VIEW_h
#define _INTEGERGLASS_MODULES_PLOT_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Shader.h"
#include "Drawable.h"
#include <IntegerTrigonometry8.h>

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Plot
		{
			template<typename dimension_t,
				typename ParentLayout,
				typename SourceType,
				typename PlotStyle = TemplatePlotStyle<>,
				typename ColorShaderType = Framework::Shader::Color::NoShader<dimension_t>,
				typename TransformShaderType = Framework::Shader::Transform::NoTransform<dimension_t>,
				Framework::Shader::Pixel::BlendModeEnum BlendMode = Framework::Shader::Pixel::BlendModeEnum::Replace
			>
			class View : public Framework::View::DrawablesView<ParentLayout,
				Drawable::TemplatePlot<dimension_t, ParentLayout, PlotStyle,
				ColorShaderType, TransformShaderType, BlendMode, SourceType>>
			{
			private:
				using DrawableType = Drawable::TemplatePlot<dimension_t, ParentLayout, PlotStyle,
					ColorShaderType, TransformShaderType, BlendMode, SourceType>;
				using Base = Framework::View::DrawablesView<ParentLayout, DrawableType>;

			public:
				View() : Base() {}
				View(SourceType& source) : Base(DrawableType(source)) {}
				~View() = default;

				void Bind(SourceType& source)
				{
					Base::template drawable<0>().Bind(source);
				}

			};

			namespace Demo
			{
				using sample_t = uint8_t;

				template<typename ParentLayout>
				static constexpr pixel_t SampleCount()
				{
					return ParentLayout::Width() / 4;
				}

				template<typename ParentLayout>
				using LinearStyle = TemplatePlotStyle<Demo::SampleCount<ParentLayout>(), Demo::sample_t,
					PlotOrientationEnum::HorizontalLeftToRight, PlotTypeEnum::BarLow, 0, 2>;

				template<typename ParentLayout>
				using CircularStyle = TemplatePlotStyle<Demo::SampleCount<ParentLayout>(), Demo::sample_t,
					PlotOrientationEnum::HorizontalLeftToRight, PlotTypeEnum::FillLow>;

				template<typename ParentLayout>
				class DemoCircularBufferView : public View<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					ParentLayout,
					Data::CircularBuffer<Demo::sample_t, Demo::SampleCount<ParentLayout>()>,
					Demo::CircularStyle<ParentLayout>>
				{
				private:
					using Style = Demo::CircularStyle<ParentLayout>;
					using Source = Data::CircularBuffer<typename Style::value_t, Style::SampleCount>;
				using Base = View<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout, Source, Style>;

				private:
					static constexpr uint32_t FillDuration = 700000;
					static constexpr uint8_t MID = static_cast<uint8_t>((static_cast<uint16_t>(UINT8_MAX) + 1u) / 2u);

				private:
					Source Samples{};

					uint32_t LastPushTime = 0;

				public:
					DemoCircularBufferView() : Base()
					{
						Base::Bind(Samples);
						auto& fillLow = Base::template drawable<0>();
						fillLow.SetRange(0, UINT8_MAX);
					}
					~DemoCircularBufferView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (!Base::ViewStep(frameTime, frameCounter))
							return false;

						// Calculate a fill value based on a sine wave over time.
						const UFraction16::scalar_t fillFraction = UFraction16::GetScalar<uint32_t>(frameTime % FillDuration, FillDuration);
						const angle_t fillAngle = UFraction16::Fraction(fillFraction, static_cast<angle_t>(ANGLE_RANGE));
						const int16_t fillDelta = Fraction<int16_t>(Trigonometry::Sine8(fillAngle), static_cast<int16_t>(MID - 1));

						// Push the value into the circular buffer.
						auto& fillLow = Base::template drawable<0>();
						fillLow.Push(LimitValue<int16_t, 0, UINT8_MAX>(static_cast<int16_t>(MID) + fillDelta));

						return true;
					}
				};

				template<typename ParentLayout>
				class DemoLinearBufferView : public View<
					typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
					ParentLayout,
					Data::LinearBuffer<Demo::sample_t, Demo::SampleCount<ParentLayout>()>,
					Demo::LinearStyle<ParentLayout>>
				{
				private:
					using Style = Demo::LinearStyle<ParentLayout>;
					using Source = Data::LinearBuffer<typename Style::value_t, Style::SampleCount>;
				using Base = View<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, ParentLayout, Source, Style>;

				private:
					static constexpr uint32_t DecayTime = 150000;
					static constexpr uint16_t InjectionEnergy = INT8_MAX * 3;
					static constexpr uint8_t DecayStrength = 16;

					static constexpr uint16_t SampleSpan = static_cast<uint16_t>(Source::SampleCount - 1);
					static constexpr uint16_t SampleCountSquared = static_cast<uint16_t>(Source::SampleCount) * static_cast<uint16_t>(Source::SampleCount);

				private:
					Source Samples{};

					uint32_t LastUpdateTime = 0;
					uint16_t RandomState = 0x1337;

				public:
					DemoLinearBufferView() : Base()
					{
						Base::Bind(Samples);
						auto& fillLow = Base::template drawable<0>();
						fillLow.SetRange(0, UINT8_MAX);

						Samples.SetAll(UINT8_MAX - 1);
					}
					~DemoLinearBufferView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (!Base::ViewStep(frameTime, frameCounter))
							return false;

						// Calculate the time and decay to apply.
						const uint32_t elapsedTime = frameTime - LastUpdateTime;
						const UFraction16::scalar_t decayFraction = UFraction16::GetScalar<uint32_t>(elapsedTime, DecayTime);
						const UFraction16::scalar_t retainFraction = UFraction16::SCALAR_UNIT - decayFraction;

						// Apply decay and random energy injection to each sample.
						for (size_t i = 0; i < Source::SampleCount; i++)
						{
							const uint8_t currentValue = Samples.Get(i);

							const uint16_t sampleCenter = static_cast<uint16_t>(Source::SampleCount);
							const uint16_t samplePosition = static_cast<uint16_t>(static_cast<uint16_t>(i) * 2u + 1u);
							const uint16_t distance = static_cast<uint16_t>(
								(samplePosition <= sampleCenter) ? sampleCenter - samplePosition : samplePosition - sampleCenter);
							const uint16_t distanceSquared = distance * distance;
							const UFraction16::scalar_t edgeFraction = UFraction16::GetScalar<uint16_t>(distanceSquared, SampleCountSquared);
							const UFraction16::scalar_t shapeFraction = UFraction16::SCALAR_UNIT - edgeFraction;

							uint8_t newValue = UFraction16::Fraction(retainFraction, currentValue);

							const uint16_t energy = UFraction16::Fraction(shapeFraction, UFraction16::Fraction<uint16_t>(decayFraction, NextRandom()));
							newValue = LimitValue<int16_t, 0, UINT8_MAX>(static_cast<int16_t>(newValue) + energy);

							Samples.Set(i, newValue);
						}

						LastUpdateTime = frameTime;

						return true;
					}

				private:
					uint16_t NextRandom()
					{
						RandomState ^= static_cast<uint16_t>(RandomState << 7);
						RandomState ^= static_cast<uint16_t>(RandomState >> 9);
						RandomState ^= static_cast<uint16_t>(RandomState << 8);

						return RandomState % InjectionEnergy;
					}
				};

			}
		}
	}
}
#endif
