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
				class Splash : public Egfx::Framework::View::DrawablesView<
					Drawable::Screen<ParentLayout, Monochrome>,
					Drawable::LettersEG<ParentLayout, Monochrome>,
					Drawable::LettersFX<ParentLayout, Monochrome>
				>
				{
				private:
					using Base = Egfx::Framework::View::DrawablesView<
						Drawable::Screen<ParentLayout, Monochrome>,
						Drawable::LettersEG<ParentLayout, Monochrome>,
						Drawable::LettersFX<ParentLayout, Monochrome>
					>;

				private:
					static constexpr uint32_t ColorDuration = 1000000;
					static constexpr uint32_t ColorCycle = ColorDuration * 3u;

				public:
					Splash() : Base()
					{
					}

					~Splash() = default;

				public:
					Drawable::Screen<ParentLayout, Monochrome>& GetScreen() { return this->template drawable<0>(); }
					Drawable::LettersEG<ParentLayout, Monochrome>& GetLettersEG() { return this->template drawable<1>(); }
					Drawable::LettersFX<ParentLayout, Monochrome>& GetLetterFX() { return this->template drawable<2>(); }

				protected:
					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						auto& lettersFX = GetLetterFX();
						lettersFX.FontDrawer.ColorSource.FrameCounter = frameCounter;
						lettersFX.FontDrawer.ColorSource.FrameTime = frameTime;

						if (Monochrome)
						{
							return true;
						}

						auto& lettersEG = GetLettersEG();

						const uint32_t cycleTime = frameTime % ColorCycle;

						const ufraction16_t colorProgress = UFraction16::GetScalar<uint32_t>(
							cycleTime % ColorDuration,
							ColorDuration);

						const uint8_t colorPhase = static_cast<uint8_t>(cycleTime / ColorDuration);

						switch (colorPhase)
						{
						case 0:
							lettersEG.FontDrawer.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color1,
								Colors::Color2);
							lettersEG.FontDrawer.ColorSource.Color2 = Rgb::Interpolate(
								colorProgress,
								Colors::Color2,
								Colors::Color3);
							break;
						case 1:
							lettersEG.FontDrawer.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color2,
								Colors::Color3);
							lettersEG.FontDrawer.ColorSource.Color2 = Rgb::Interpolate(
								colorProgress,
								Colors::Color3,
								Colors::Color1);
							break;
						case 2:
						default:
							lettersEG.FontDrawer.ColorSource.Color1 = Rgb::Interpolate(
								colorProgress,
								Colors::Color3,
								Colors::Color1);
							lettersEG.FontDrawer.ColorSource.Color2 = Rgb::Interpolate(
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
					bool AutoStart = true>
				class AnimatedSplash : public Splash<ParentLayout, Monochrome>
				{
				private:
					using Base = Splash<ParentLayout, Monochrome>;

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
					AnimatedSplash() : Base()
					{
					}

					~AnimatedSplash() = default;

					void Start()
					{
						State = AnimationEnum::AnimationStart;
					}

					void Stop()
					{
						State = AnimationEnum::AnimationEnd;
					}

				protected:
					void SetTranslationY(const pixel_t y)
					{
						auto& screen = Base::GetScreen();
						screen.TransformShader.SetTranslation(0, y);
						auto& lettersEG = Base::GetLettersEG();
						auto& lettersFX = Base::GetLetterFX();

						lettersEG.FontDrawer.TransformShader.SetTranslation(0, y);
						lettersFX.FontDrawer.TransformShader.SetTranslation(0, y);
					}

					void SetAlpha(const uint8_t alpha)
					{
						auto& screen = Base::GetScreen();
						auto& lettersEG = Base::GetLettersEG();
						auto& lettersFX = Base::GetLetterFX();
						
						screen.ColorShader.Alpha = alpha;
						lettersEG.FontDrawer.ColorSource.Alpha = alpha;
						lettersFX.FontDrawer.ColorSource.Alpha = alpha;
					}

					bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
					{
						Base::ViewStep(frameTime, frameCounter);

						switch (State)
						{
						case AnimationEnum::AnimationStart:
							SetTranslationY(VerticalShift);
							SetAlpha(0);
							State = AnimationEnum::SlideIn;
							AnimationStart = micros();
							break;
						case AnimationEnum::SlideIn:
						{
							const uint32_t elapsed = micros() - AnimationStart;
							if (elapsed >= Durations::SlideIn)
							{
								SetTranslationY(0);
								SetAlpha(UINT8_MAX);
								State = AnimationEnum::Hold;
								AnimationStart = micros();
							}
							else
							{
								const uint16_t progress = ProgressScaler::GetProgress<Durations::SlideIn>(elapsed);
								const uint8_t rootProgress = IntegerSignal::SquareRoot16(progress);
								const uint8_t curvedProgress = IntegerSignal::Curves::Root2U8<>::Get(rootProgress);
								const pixel_t offset = VerticalShift - ((VerticalShift * curvedProgress) / UINT8_MAX);
								SetTranslationY(offset);

								const auto raw = static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::SlideIn);
								const auto curved = IntegerSignal::Curves::Power2U8<>::Get(raw);
								SetAlpha(Monochrome ? raw : curved);
							}
						}
						break;
						case AnimationEnum::Hold:
						{
							const uint32_t elapsed = micros() - AnimationStart;
							if (elapsed >= Durations::Hold)
							{
								State = AnimationEnum::FadeOut;
								AnimationStart = micros();
							}
						}
						break;
						case AnimationEnum::FadeOut:
						{
							const uint32_t elapsed = micros() - AnimationStart;
							if (elapsed >= Durations::FadeOut)
							{
								SetTranslationY(0);
								SetAlpha(0);
								State = AnimationEnum::AnimationEnd;
								AnimationStart = micros();
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
						return true;
					}
				};
			}
		}
	}
}
#endif