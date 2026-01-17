#ifndef _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_VIEWS_h
#define _EGFX_FRAMEWORK_ASSETS_EGFX_LOGO_VIEWS_h

#include <EgfxDrawer.h>
#include "Drawables.h"
#include "Layout.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				namespace Views
				{
					template<typename ParentLayout, bool Monochrome>
					class Splash : public Egfx::Framework::View::DrawablesView<
						Drawables::Screen<typename Layout<ParentLayout>::ScreenMargin, Monochrome>,
						Drawables::LetterE<typename Layout<ParentLayout>::LetterE, Monochrome>,
						Drawables::LetterG<typename Layout<ParentLayout>::LetterG, Monochrome>,
						Drawables::LetterF<typename Layout<ParentLayout>::LetterF, Monochrome>,
						Drawables::LetterX<typename Layout<ParentLayout>::LetterX, Monochrome>
					>
					{
					private:
						using Base = Egfx::Framework::View::DrawablesView<
							Drawables::Screen<typename Layout<ParentLayout>::ScreenMargin, Monochrome>,
							Drawables::LetterE<typename Layout<ParentLayout>::LetterE, Monochrome>,
							Drawables::LetterG<typename Layout<ParentLayout>::LetterG, Monochrome>,
							Drawables::LetterF<typename Layout<ParentLayout>::LetterF, Monochrome>,
							Drawables::LetterX<typename Layout<ParentLayout>::LetterX, Monochrome>
						>;

					private:
						static constexpr uint32_t ColorDuration = 1000000;
						static constexpr uint32_t ColorCycle = ColorDuration * 3u;

					protected:
						using dimension_t = CodeFonts::LogoFontFamily::Dimensions::font_dimension_t;

						using Logo = typename Layout<ParentLayout>::Logo;

						static constexpr auto FontWidth = Logo::FontWidth();
						static constexpr auto FontHeight = Logo::FontHeight();

						using RetroLinesPrimitiveShaderType = Drawables::RetroLinesPrimitiveShaderType<FontWidth, FontHeight, Monochrome>;
						using RetroLinesFontDrawerType = CodeFonts::LogoFontFamily::TemplateFont<FontWidth, FontHeight, RetroLinesPrimitiveShaderType>;

						using DotMatrixPrimitiveShaderType = Drawables::DotMatrixPrimitiveShaderType<FontWidth, FontHeight, Monochrome>;
						using DotMatrixFontDrawerType = CodeFonts::LogoFontFamily::TemplateFont<FontWidth, FontHeight, DotMatrixPrimitiveShaderType>;

						RetroLinesFontDrawerType RetroLinesFont{};
						DotMatrixFontDrawerType DotMatrixFont{};

						pixel_point_t Translation{ 0, 0 };

					public:
						Splash() : Base()
						{
							GetLetterE().FontDrawer = &RetroLinesFont;
							GetLetterG().FontDrawer = &RetroLinesFont;
							GetLetterF().FontDrawer = &DotMatrixFont;
							GetLetterX().FontDrawer = &DotMatrixFont;

							GetScreen().Translation = &Translation;
							GetLetterE().Translation = &Translation;
							GetLetterG().Translation = &Translation;
							GetLetterF().Translation = &Translation;
							GetLetterX().Translation = &Translation;
						}

						~Splash() = default;

					public:
						Drawables::Screen<typename Layout<ParentLayout>::ScreenMargin, Monochrome>& GetScreen() { return this->template drawable<0>(); }
						Drawables::LetterE<typename Layout<ParentLayout>::LetterE, Monochrome>& GetLetterE() { return this->template drawable<1>(); }
						Drawables::LetterG<typename Layout<ParentLayout>::LetterG, Monochrome>& GetLetterG() { return this->template drawable<2>(); }
						Drawables::LetterF<typename Layout<ParentLayout>::LetterF, Monochrome>& GetLetterF() { return this->template drawable<3>(); }
						Drawables::LetterX<typename Layout<ParentLayout>::LetterX, Monochrome>& GetLetterX() { return this->template drawable<4>(); }

					protected:
						void ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
						{
							DotMatrixFont.PrimitiveShader.ColorShader.FrameTime = frameTime;
							DotMatrixFont.PrimitiveShader.ColorShader.FrameCounter = frameCounter;

							const uint32_t cycleTime = frameTime % ColorCycle;

							const ufraction16_t colorProgress = UFraction16::GetScalar<uint32_t>(
								cycleTime % ColorDuration,
								ColorDuration);

							const uint8_t colorPhase = static_cast<uint8_t>(cycleTime / ColorDuration);

							switch (colorPhase)
							{
							case 0:
								RetroLinesFont.PrimitiveShader.ColorShader.Color1 = Rgb::Interpolate(
									colorProgress,
									Colors::Color1,
									Colors::Color2);
								RetroLinesFont.PrimitiveShader.ColorShader.Color2 = Rgb::Interpolate(
									colorProgress,
									Colors::Color2,
									Colors::Color3);
								break;
							case 1:
								RetroLinesFont.PrimitiveShader.ColorShader.Color1 = Rgb::Interpolate(
									colorProgress,
									Colors::Color2,
									Colors::Color3);
								RetroLinesFont.PrimitiveShader.ColorShader.Color2 = Rgb::Interpolate(
									colorProgress,
									Colors::Color3,
									Colors::Color1);
								break;
							case 2:
							default:
								RetroLinesFont.PrimitiveShader.ColorShader.Color1 = Rgb::Interpolate(
									colorProgress,
									Colors::Color3,
									Colors::Color1);
								RetroLinesFont.PrimitiveShader.ColorShader.Color2 = Rgb::Interpolate(
									colorProgress,
									Colors::Color1,
									Colors::Color2);
								break;
							}
						}
					};

					/// <summary>
					/// Animated splash screen class that provides slide-in, hold, and fade-out animation effects for a splash display.
					/// </summary>
					/// <typeparam name="ParentLayout">The parent layout class that defines the screen dimensions and layout properties.</typeparam>
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

					protected:
						using Base::RetroLinesFont;
						using Base::DotMatrixFont;
						using Base::Translation;

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

						static constexpr pixel_t VerticalShift = Layout<ParentLayout>::LogoBorder::Height();

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
						void ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
						{
							Base::ViewStep(frameTime, frameCounter);

							auto& screen = Base::GetScreen();

							switch (State)
							{
							case AnimationEnum::AnimationStart:
								Translation.y = VerticalShift;
								screen.Shader.Alpha = 0;
								RetroLinesFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
								DotMatrixFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
								State = AnimationEnum::SlideIn;
								AnimationStart = micros();
								break;
							case AnimationEnum::SlideIn:
							{
								const uint32_t elapsed = micros() - AnimationStart;
								if (elapsed >= Durations::SlideIn)
								{
									Translation.y = 0;
									screen.Shader.Alpha = UINT8_MAX;
									RetroLinesFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									DotMatrixFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									State = AnimationEnum::Hold;
									AnimationStart = micros();
								}
								else
								{
									const uint16_t progress = ProgressScaler::GetProgress<Durations::SlideIn>(elapsed);
									const uint8_t rootProgress = IntegerSignal::SquareRoot16(progress);
									const uint8_t curvedProgress = IntegerSignal::Curves::Root2U8<>::Get(rootProgress);
									const pixel_t offset = VerticalShift - ((VerticalShift * curvedProgress) / UINT8_MAX);
									Translation.y = offset;

									const auto raw = static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::SlideIn);
									const auto curved = IntegerSignal::Curves::Power2U8<>::Get(raw);
									screen.Shader.Alpha = Monochrome ? raw : curved;
									RetroLinesFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									DotMatrixFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
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
									Translation.y = 0;
									screen.Shader.Alpha = 0;
									RetroLinesFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									DotMatrixFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									State = AnimationEnum::AnimationEnd;
									AnimationStart = micros();
								}
								else
								{
									screen.Shader.Alpha = UINT8_MAX - static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::FadeOut);
									RetroLinesFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
									DotMatrixFont.PrimitiveShader.ColorShader.Alpha = screen.Shader.Alpha;
								}
							}
							break;
							case AnimationEnum::AnimationEnd:
								break;
							default:
								break;
							}
						}
					};
				}
			}
		}
	}
}
#endif