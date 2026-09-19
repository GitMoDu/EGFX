#ifndef _EGFX_MODULES_EGFX_LOGO_VIEW_h
#define _EGFX_MODULES_EGFX_LOGO_VIEW_h

#include "Drawable.h"
#include "Layout.h"

namespace Egfx
{
	namespace Modules
	{
		namespace EgfxLogo
		{
			namespace View
			{
				template<typename ParentLayout, bool Monochrome>
				class Logo : public Egfx::Framework::View::DrawablesView<ParentLayout,
					Drawable::Screen<ParentLayout, Monochrome>,
					Drawable::LettersEG<ParentLayout, Monochrome>,
					Drawable::LettersFX<ParentLayout, Monochrome>
				>
				{
				private:
					using Base = Egfx::Framework::View::DrawablesView<ParentLayout,
						Drawable::Screen<ParentLayout, Monochrome>,
						Drawable::LettersEG<ParentLayout, Monochrome>,
						Drawable::LettersFX<ParentLayout, Monochrome>
					>;

				private:
					static constexpr uint32_t ColorDuration = 1000000;
					static constexpr uint32_t ColorCycle = ColorDuration * 3u;

					bool logged = false;

				public:
					Logo() : Base() {}

					~Logo() = default;

				public:
					Drawable::Screen<ParentLayout, Monochrome>& GetScreen() { return this->template drawable<0>(); }
					Drawable::LettersEG<ParentLayout, Monochrome>& GetLettersEG() { return this->template drawable<1>(); }
					Drawable::LettersFX<ParentLayout, Monochrome>& GetLetterFX() { return this->template drawable<2>(); }

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						auto& lettersFX = GetLetterFX();
						lettersFX.ColorSource.FrameCounter = frameCounter;
						lettersFX.ColorSource.FrameTime = frameTime;

						if (Monochrome)
						{
							return true;
						}


						const uint32_t cycleTime = frameTime % ColorCycle;

						const ufraction16_t colorProgress = UFraction16::GetScalar<uint32_t>(
							cycleTime % ColorDuration,
							ColorDuration);

						const uint8_t colorPhase = static_cast<uint8_t>(cycleTime / ColorDuration);

						auto& lettersEG = GetLettersEG();
						switch (colorPhase)
						{
						case 0:
							lettersEG.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color1,
								Colors::Color2);
							lettersEG.ColorSource.Color2 = Rgb::Interpolate(
								colorProgress,
								Colors::Color2,
								Colors::Color3);
							break;
						case 1:
							lettersEG.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color2,
								Colors::Color3);
							lettersEG.ColorSource.Color2 = Rgb::Interpolate(
								colorProgress,
								Colors::Color3,
								Colors::Color1);
							break;
						case 2:
						default:
							lettersEG.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color3,
								Colors::Color1);
							lettersEG.ColorSource.Color2 = Rgb::Interpolate(
								colorProgress,
								Colors::Color1,
								Colors::Color2);
							break;
						}
						return true;
					}
				};

				/// <summary>
				/// Animated splash screen class that provides slide-in, hold, and fade-out animation effects for a splash display.
				/// </summary>
				/// <typeparam name="ParentLayout">Parent layout providing X/Y/Width/Height.</typeparam>
				/// <typeparam name="Monochrome">Boolean flag indicating whether the animation should use monochrome (dithered) or color rendering.</typeparam>
				/// <typeparam name="AnimationDuration">The total duration of the animation sequence in microseconds. Defaults to 4000000 (4 seconds).</typeparam>
				/// <typeparam name="AutoStart">Boolean flag indicating whether the animation should start automatically. Defaults to true.</typeparam>
				template<typename ParentLayout, bool Monochrome,
					uint32_t AnimationDuration = 4000000,
					bool AutoStart = true,
					bool Loop = false>
				class SplashLogo : public Logo<ParentLayout, Monochrome>
				{
				private:
					using Base = Logo<ParentLayout, Monochrome>;

				private:
					enum class AnimationEnum : uint8_t
					{
						AnimationStart,
						SlideIn,
						Hold,
						FadeOut,
						AnimationEnd
					};

					struct Durations
					{
						static constexpr uint32_t Margin = AnimationDuration / 28;
						static constexpr uint32_t UsableTime = AnimationDuration - Margin;
						static constexpr uint32_t Hold = UsableTime / 3;

						static constexpr uint32_t FadeOut = ((UsableTime - Hold) / 3) - Margin;
						static constexpr uint32_t SlideIn = UsableTime - Hold - FadeOut;
					};

					static constexpr pixel_t VerticalShift = ParentLayout::Height() / 2;

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
					}

					bool IsFinished() const
					{
						return State == AnimationEnum::AnimationEnd;
					}

				protected:
					void SetTranslationY(const pixel_t y)
					{
						auto& screen = Base::GetScreen();
						screen.SetTranslation(0, y);
						auto& lettersEG = Base::GetLettersEG();
						auto& lettersFX = Base::GetLetterFX();

						lettersEG.SetTranslation(0, y);
						lettersFX.SetTranslation(0, y);
					}

					void SetAlpha(const uint8_t alpha)
					{
						auto& screen = Base::GetScreen();
						auto& lettersEG = Base::GetLettersEG();
						auto& lettersFX = Base::GetLetterFX();

						screen.ColorShader.Alpha = alpha;
						lettersEG.ColorSource.Alpha = alpha;
						lettersFX.ColorSource.Alpha = alpha;
					}

					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						

						switch (State)
						{
						case AnimationEnum::AnimationStart:
							SetTranslationY(VerticalShift);
							SetAlpha(0);
							State = AnimationEnum::SlideIn;
							AnimationStart = frameTime;
							break;
						case AnimationEnum::SlideIn:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= Durations::SlideIn)
							{
								SetTranslationY(0);
								SetAlpha(UINT8_MAX);
								State = AnimationEnum::Hold;
								AnimationStart = frameTime;
							}
							else
							{
								const uint16_t progress = ProgressScaler::GetProgress<Durations::SlideIn>(elapsed);
								const uint8_t rootProgress = IntegerSignal::SquareRoot16(progress);
								const pixel_t offset = VerticalShift - ((VerticalShift * rootProgress) / UINT8_MAX);
								SetTranslationY(offset);

								const auto raw = static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::SlideIn);
								const auto curved = IntegerSignal::Curves::Power2U8<>::Get(raw);
								SetAlpha(Monochrome ? raw : curved);
							}
						}
						break;
						case AnimationEnum::Hold:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= Durations::Hold)
							{
								State = AnimationEnum::FadeOut;
								AnimationStart = frameTime;
							}
						}
						break;
						case AnimationEnum::FadeOut:
						{
							const uint32_t elapsed = frameTime - AnimationStart;
							if (elapsed >= Durations::FadeOut)
							{
								SetTranslationY(0);
								SetAlpha(0);
								if (Loop)
								{
									State = AnimationEnum::AnimationStart;
								}
								else
								{
									State = AnimationEnum::AnimationEnd;
								}
								AnimationStart = frameTime;
							}
							else
							{
								SetAlpha(UINT8_MAX - static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::FadeOut));
							}
						}
						break;
						case AnimationEnum::AnimationEnd:
							break;
						default:
							break;
						}
						
						return Base::ViewStep(frameTime, frameCounter);;
					}
				};
			}
		}
	}
}
#endif