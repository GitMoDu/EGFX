#ifndef _EGFX_MODULES_BATTERY_INDICATOR_VIEW_h
#define _EGFX_MODULES_BATTERY_INDICATOR_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Drawable.h"

namespace Egfx
{
	namespace Modules
	{
		namespace BatteryIndicator
		{
			namespace View
			{
				template<typename dimension_t,
					typename ParentLayout,
					typename BatteryStyle = TemplateBatteryStyle<>
				>
				class BatteryView : public Framework::View::DrawablesView<
					Drawable::Outside<dimension_t, ParentLayout, BatteryStyle>,
					Drawable::ProgressBars<dimension_t, ParentLayout, BatteryStyle>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<
						Drawable::Outside<dimension_t, ParentLayout, BatteryStyle>,
						Drawable::ProgressBars<dimension_t, ParentLayout, BatteryStyle>
					>;

					using MonochromeTag = typename TypeTraits::TypeConditional::conditional_type<
						TypeTraits::TypeDispatch::TrueType,
						TypeTraits::TypeDispatch::FalseType,
						BatteryStyle::Monochrome
					>::type;

				private:
					uint32_t AnimationStart = 0;
					bool Charging = false;
					bool AnimationInvalidated = false;

				public:
					bool Visible = true;

				public:
					BatteryView() : Base() {}
					~BatteryView() = default;

					void SetCharging(const bool charging)
					{
						AnimationInvalidated |= Charging != charging;
						Charging = charging;
					}

					void SetBarsColor(const rgb_color_t color)
					{
						SetBarsColor(color, MonochromeTag{});
					}

					/// <summary>
					/// Sets battery level to display on the view.
					/// </summary>
					/// <param name="level">Battery level normalized to uint8 (0-255)</param>
					void SetLevel(const uint8_t level)
					{
						auto& progressBars = Base::template drawable<1>();

						const uint16_t levelBiased = static_cast<uint16_t>(level) + 1u;
						const uint8_t lastIndex = static_cast<uint8_t>(
							(levelBiased * BatteryStyle::BarCount) / (static_cast<uint16_t>(UINT8_MAX) + 1u));

						const uint8_t currentIndex = progressBars.GetFullCount();
						progressBars.SetFullCount(lastIndex);

						AnimationInvalidated |= lastIndex != currentIndex;
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (AnimationInvalidated)
						{
							AnimationInvalidated = false;
							AnimationStart = frameTime;
						}

						InnerStep(frameTime, frameCounter, MonochromeTag{});

						return Visible;
					}

				private:
					void SetBarsColor(const rgb_color_t color, TypeTraits::TypeDispatch::TrueType)
					{
						// No-op for monochrome.
					}

					void SetBarsColor(const rgb_color_t color, TypeTraits::TypeDispatch::FalseType)
					{
						auto& progressBars = Base::template drawable<1>();
						progressBars.WholeShader.ColorSource.Color = color;
						progressBars.LastShader.ColorSource.Color = color;
					}

					void SetOutsideColor(const rgb_color_t color, TypeTraits::TypeDispatch::TrueType)
					{
						// No-op for monochrome.
					}

					void SetOutsideColor(const rgb_color_t color, TypeTraits::TypeDispatch::FalseType)
					{
						auto& outside = Base::template drawable<0>();
						outside.ColorSource.Color = color;
					}

					void InnerStep(const uint32_t frameTime, const uint16_t frameCounter, TypeTraits::TypeDispatch::TrueType)
					{
						auto& progressBars = Base::template drawable<1>();
						if (Charging)
						{
							progressBars.LastShader.TransformShader.SetMaskPhase((frameCounter >> 0) & 1, (frameCounter >> 0) & 1);

							const ufraction16_t progress = GetAnimationProgress(frameTime);
							const uint8_t modulus = 2 + Fraction<uint8_t>(progress, Dimensions::MaxDitherModulus + 1);
							progressBars.LastShader.TransformShader.SetModulus(modulus);
						}
						else
						{
							progressBars.LastShader.TransformShader.SetMaskPhase(0, 0);
							progressBars.LastShader.TransformShader.SetModulus(1);
						}
					}

					void InnerStep(const uint32_t frameTime, const uint16_t /*frameCounter*/, TypeTraits::TypeDispatch::FalseType)
					{
						auto& progressBars = Base::template drawable<1>();
						if (Charging)
						{
							const ufraction16_t progress = GetAnimationProgress(frameTime);
							const fraction16_t brightness = -(FRACTION16_1X / 2) - fraction16_t(SignedRightShift(progress, 2));
							progressBars.LastShader.ColorShader.SetBrightness(brightness);
						}
						else
						{
							progressBars.LastShader.ColorShader.SetBrightness(0);
						}
					}

					ufraction16_t GetAnimationProgress(const uint32_t frameTime) const
					{
						const uint32_t elapsed = (frameTime - AnimationStart) % (BatteryStyle::ChargeAnimationPeriod * 2);
						if (elapsed < BatteryStyle::ChargeAnimationPeriod)
						{
							return UFraction16::GetScalar<uint32_t>(elapsed, BatteryStyle::ChargeAnimationPeriod - 1);
						}
						else
						{
							return UFraction16::GetScalar<uint32_t>((BatteryStyle::ChargeAnimationPeriod * 2) - elapsed,
								BatteryStyle::ChargeAnimationPeriod - 1);
						}
					}
				};

				template<typename ParentLayout,
					bool Monochrome = true
				>
				class BatteryDemoView : public BatteryView<pixel_t, ParentLayout, TemplateBatteryStyle<Monochrome>>
				{
				private:
					using Base = BatteryView<pixel_t, ParentLayout, TemplateBatteryStyle<Monochrome>>;

				private:
					static constexpr uint32_t CyclePeriod = 5000000;

					static constexpr uint32_t ProgressiveDuration = CyclePeriod / 2;
					static constexpr uint32_t HoldDuration = CyclePeriod / 4;
					static constexpr uint32_t FullDuration = CyclePeriod / 4;
					static constexpr uint32_t ProgressiveStart = 0;
					static constexpr uint32_t ProgressiveEnd = ProgressiveStart + ProgressiveDuration;
					static constexpr uint32_t HoldStart = ProgressiveEnd;
					static constexpr uint32_t HoldEnd = HoldStart + HoldDuration;
					static constexpr uint32_t FullStart = HoldEnd;
					static constexpr uint32_t FullEnd = FullStart + FullDuration;

				public:
					BatteryDemoView() : Base() {}
					~BatteryDemoView() = default;

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (Base::ViewStep(frameTime, frameCounter))
						{
							const uint32_t section = frameTime % CyclePeriod;

							if (section <= ProgressiveEnd)
							{
								Base::SetCharging(true);
								Base::SetLevel((section * UINT8_MAX) / ProgressiveDuration);
							}
							else if (section <= HoldEnd)
							{
								Base::SetCharging(true);
								Base::SetLevel(UINT8_MAX);
							}
							else
							{
								Base::SetCharging(false);
								Base::SetLevel(UINT8_MAX);
							}

							return true;
						}
						else
						{
							return false;
						}
					}
				};
			}
		}
	}
}
#endif