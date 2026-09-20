#ifndef _INTEGERGLASS_MODULES_LOGO_VIEW_h
#define _INTEGERGLASS_MODULES_LOGO_VIEW_h

#include "Constant.h"
#include "Layout.h"
#include "Drawable.h"

namespace IntegerGlass
{
	namespace Modules
	{
		namespace Logo
		{
			namespace View
			{
				template<typename ParentLayout, bool Monochrome, bool AutoScroll = true>
				class Logo : public Framework::View::DrawablesView<ParentLayout,
					Drawable::Label<Layout::Label<ParentLayout>, Monochrome>,
					Drawable::Glass<Layout::Glass<ParentLayout>, Monochrome>,
					Drawable::GlassText<Layout::Text<ParentLayout>, Monochrome>,
					Drawable::Reflection<Layout::Reflection<ParentLayout>, Monochrome>
				>
				{
				private:
					using Base = Framework::View::DrawablesView<ParentLayout,
						Drawable::Label<Layout::Label<ParentLayout>, Monochrome>,
						Drawable::Glass<Layout::Glass<ParentLayout>, Monochrome>,
						Drawable::GlassText<Layout::Text<ParentLayout>, Monochrome>,
						Drawable::Reflection<Layout::Reflection<ParentLayout>, Monochrome>
					>;

					using MonochromeTag =
						typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
						IntegerSignal::TypeTraits::TypeDispatch::TrueType,
						IntegerSignal::TypeTraits::TypeDispatch::FalseType,
						Monochrome
						>::type;

					using label_dimension_t = typename Framework::AutoDimension::ByLayout<Layout::Label<ParentLayout>>::dimension_t;
					using LabelLayout = Layout::Label<ParentLayout>;

				private:
					static constexpr pixel_t ReflectionWidth = Layout::Calc::ReflectionLayout<Layout::Reflection<ParentLayout>>::Width();
					static constexpr pixel_t GlassWidth = Layout::Glass<ParentLayout>::Width();
					static constexpr pixel_t ReflectionTranslation = GlassWidth + ReflectionWidth;
					static constexpr pixel_t ReflectionOffset = -ReflectionWidth;
					static constexpr uint32_t ColorCycle = ::IntegerGlass::Modules::Logo::Durations::ColorDuration * 3u;

				public:
					Logo() : Base() {}
					~Logo() = default;

					void SetReflectionScroll(const ufraction16_t scrollFraction)
					{
						auto& diagonalReflection = Base::template drawable<3>();
						const pixel_t horizontalScroll = UFraction16::Fraction(scrollFraction, ReflectionTranslation);
						diagonalReflection.OffsetX = horizontalScroll - ReflectionWidth;
					}

					void SetTranslationY(const pixel_t y)
					{
						auto& label = Base::template drawable<0>();
						label.SetTranslation(0, y);
						auto& glass = Base::template drawable<1>();
						glass.SetTranslation(0, y);
						auto& glassText = Base::template drawable<2>();
						glassText.SetTranslation(0, y);
						auto& reflection = Base::template drawable<3>();
						reflection.SetTranslation(0, y);
					}

					void SetAlpha(const uint8_t alpha)
					{
						auto& label = Base::template drawable<0>();
						auto& glass = Base::template drawable<1>();
						auto& glassText = Base::template drawable<2>();
						auto& reflection = Base::template drawable<3>();

						reflection.SetAlpha(alpha);
						glass.SetAlpha(alpha);
						glassText.SetAlpha(alpha);
						label.SetAlpha(alpha);
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
					{
						if (AutoScroll)
						{
							static constexpr uint32_t AutoScrollDuration = 3000000;
							const ufraction16_t scrollFraction = UFraction16::GetScalar<uint32_t>(frameTime % AutoScrollDuration, AutoScrollDuration);
							SetReflectionScroll(UFRACTION16_1X - scrollFraction);
						}

						UpdateColors(frameTime, MonochromeTag{});

						return true;
					}

				private:
					void UpdateColors(const uint32_t /*frameTime*/, TypeTraits::TypeDispatch::TrueType)
					{
						// No color updates for monochrome mode.
					}

					void UpdateColors(const uint32_t frameTime, TypeTraits::TypeDispatch::FalseType)
					{
						auto& glassText = Base::template drawable<2>();
						const uint32_t cycleTime = frameTime % ColorCycle;
						const ufraction16_t colorProgress = UFraction16::GetScalar<uint32_t>(
							cycleTime % ::IntegerGlass::Modules::Logo::Durations::ColorDuration,
							::IntegerGlass::Modules::Logo::Durations::ColorDuration);
						const uint8_t colorPhase = static_cast<uint8_t>(cycleTime / ::IntegerGlass::Modules::Logo::Durations::ColorDuration);
						switch (colorPhase)
						{
						case 0:
							glassText.SetColors(Rgb::Interpolate(colorProgress,
								Colors::Color1,
								Colors::Color2),
								Rgb::Interpolate(colorProgress,
									Colors::Color2,
									Colors::Color3));
							break;
						case 1:
							glassText.SetColors(Rgb::Interpolate(colorProgress,
								Colors::Color2,
								Colors::Color3),
								Rgb::Interpolate(colorProgress,
									Colors::Color3,
									Colors::Color1));
							break;
						case 2:
						default:
							glassText.SetColors(Rgb::Interpolate(colorProgress,
								Colors::Color3,
								Colors::Color1),
								Rgb::Interpolate(colorProgress,
									Colors::Color1,
									Colors::Color2));
							break;
						}
					}
				};

				template<typename ParentLayout, bool Monochrome,
					uint32_t AnimationDuration = Durations::SplashDuration,
					bool AutoStart = true,
					bool Loop = false>
				class SplashLogo : public Logo<ParentLayout, Monochrome, false>
				{
				private:
					using Base = Logo<ParentLayout, Monochrome, false>;

				public:
					using Base::SetTranslationY;
					using Base::SetAlpha;
					using Base::SetReflectionScroll;

				private:
					enum class AnimationEnum : uint8_t
					{
						AnimationStart,
						SlideIn,
						Hold,
						FadeOut,
						AnimationEnd
					};

					using SplashDurations = ::IntegerGlass::Modules::Logo::Durations::Splash<AnimationDuration>;

				private:

					static constexpr pixel_t VerticalShift = Layout::Glass<ParentLayout>::Height() / Dimensions::SplashVerticalShiftDivisor;

				private:
					uint32_t AnimationStart = 0;

					AnimationEnum State = AutoStart ? AnimationEnum::AnimationStart : AnimationEnum::AnimationEnd;

				public:
					SplashLogo() : Base()
					{
						SetAlpha(0);
					}
					~SplashLogo() = default;

					void Start()
					{
						State = AnimationEnum::AnimationStart;
					}

					void Stop()
					{
						State = AnimationEnum::AnimationEnd;
						SetAlpha(0);
					}

					bool IsFinished() const
					{
						return State == AnimationEnum::AnimationEnd;
					}

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						if (!Base::ViewStep(frameTime, frameCounter))
						{
							return false;
						}

						switch (State)
						{
						case AnimationEnum::AnimationStart:
							SetTranslationY(VerticalShift);
							SetAlpha(0);
							SetReflectionScroll(0);
							State = AnimationEnum::SlideIn;
							AnimationStart = frameTime;
							break;
						case AnimationEnum::SlideIn:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= SplashDurations::SlideIn)
							{
								SetTranslationY(0);
								SetAlpha(UINT8_MAX);
								SetReflectionScroll(UFRACTION16_1X);
								State = AnimationEnum::Hold;
								AnimationStart = frameTime;
							}
							else
							{
								// Linear scroll fraction.
								const ufraction16_t scrollFraction = UFraction16::GetScalar<uint32_t>(elapsed, SplashDurations::SlideIn);
								const ufraction16_t inverseScrollFraction = UFRACTION16_1X - scrollFraction;
								SetTranslationY(Fraction(inverseScrollFraction, VerticalShift));

								SetReflectionScroll(inverseScrollFraction);

								// Use the inverse fraction to decrease the alpha more slowly at the start and faster at the end.
								const uint8_t alpha = UINT8_MAX - Fraction<uint8_t>(inverseScrollFraction, UINT8_MAX);
								SetAlpha(alpha);
							}
						}
						break;
						case AnimationEnum::Hold:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= SplashDurations::Hold)
							{
								State = AnimationEnum::FadeOut;
								SetReflectionScroll(UFRACTION16_1X);
								AnimationStart = frameTime;
							}
							else
							{
								const ufraction16_t scrollFraction = UFraction16::GetScalar<uint32_t>(elapsed, SplashDurations::Hold);
								SetReflectionScroll(UFRACTION16_1X - scrollFraction);
							}
						}
						break;
						case AnimationEnum::FadeOut:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= SplashDurations::FadeOut)
							{
								SetTranslationY(0);
								SetAlpha(0);

								if (Loop)
								{
									State = AnimationEnum::AnimationStart;
									AnimationStart = frameTime;
								}
								else
								{
									State = AnimationEnum::AnimationEnd;
								}
							}
							else
							{
								const ufraction16_t fadeFraction = UFraction16::GetScalar<uint32_t>(elapsed, SplashDurations::FadeOut);
								SetAlpha(UFraction16::Fraction<uint8_t>(UFRACTION16_1X - fadeFraction, UINT8_MAX));
							}
						}
						break;
						case AnimationEnum::AnimationEnd:
						default:
							break;
						}
						return true;
					}
				};
			}
		}
	}
}
#endif