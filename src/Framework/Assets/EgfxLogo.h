#ifndef _EGFX_ASSETS_EGFX_LOGO_h
#define _EGFX_ASSETS_EGFX_LOGO_h

#include <EgfxDrawer.h>

namespace Egfx
{
	namespace Framework
	{
		namespace Assets
		{
			namespace EgfxLogo
			{
				// Fixed dimension references for logo rendering.
				namespace Dimensions
				{
					static constexpr uint8_t Separator() { return 1; }
					static constexpr uint8_t LineWidth() { return 2; }
					static constexpr uint8_t VerticalCount() { return 10; }
					static constexpr uint8_t HorizontalCount() { return 6; }
					static constexpr uint8_t LetterCount() { return 4; }
					static constexpr uint8_t LogoMinHeight() { return 32; }
				}

				namespace Colors
				{
					static constexpr rgb_color_t Background = RGB_COLOR_BLACK;
					static constexpr rgb_color_t Foreground = RGB_COLOR_WHITE;

					// Palette colors
					static constexpr rgb_color_t Color1 = Rgb::Color(uint32_t(0xA80F21));
					static constexpr rgb_color_t Color2 = Rgb::Color(uint32_t(0xCFD60E));
					static constexpr rgb_color_t Color3 = Rgb::Color(uint32_t(0x006CE0));
				}

				// Automated layout calculations for logo rendering.
				template<typename ParentLayout>
				struct Layout
				{
					static constexpr pixel_t MinDimension() { return MinValue(ParentLayout::Width(), ParentLayout::Height()); }
					static constexpr pixel_t MaxDimension() { return MaxValue(ParentLayout::Width(), ParentLayout::Height()); }

					struct MainLayout
					{
						static constexpr pixel_t TargetMargin()
						{
							return MaxDimension() / 8;
						}

						static constexpr pixel_t OuterMargin()
						{
							return ParentLayout::Height() <= Dimensions::LogoMinHeight() ? 0 : TargetMargin();
						}

						static constexpr pixel_t X() { return ParentLayout::X() + OuterMargin(); }
						static constexpr pixel_t Y() { return ParentLayout::Y() + OuterMargin(); }
						static constexpr pixel_t Width() { return ParentLayout::Width() - (OuterMargin() * 2); }
						static constexpr pixel_t Height() { return ParentLayout::Height() - (OuterMargin() * 2); }
						static constexpr pixel_t InnerMargin() { return 4 + ParentLayout::Height() / 64; }

						static constexpr pixel_t InnerX() { return X() + InnerMargin(); }
						static constexpr pixel_t InnerY() { return Y() + InnerMargin(); }
						static constexpr pixel_t InnerWidth() { return Width() - (InnerMargin() * 2); }
						static constexpr pixel_t InnerHeight() { return Height() - (InnerMargin() * 2); }
					};

					// Shared screen matrix properties
					struct LetterMatrixCalculator
					{
						static constexpr pixel_t MaxLetterHeight() { return MainLayout::InnerHeight(); }

						static constexpr pixel_t MaxLetterWidth()
						{
							return (MaxLetterHeight() * Dimensions::HorizontalCount()) / Dimensions::VerticalCount();
						}

						static constexpr pixel_t DivisionLetterWidth()
						{
							return (MainLayout::InnerWidth() - ((Dimensions::LetterCount() - 1) * 2)) / Dimensions::LetterCount();
						}

						static constexpr pixel_t LetterWidth()
						{
							return MinValue(MaxLetterWidth(), DivisionLetterWidth());
						}

						static constexpr pixel_t LetterHeight()
						{
							return (LetterWidth() * Dimensions::VerticalCount()) / Dimensions::HorizontalCount();
						}

						static constexpr pixel_t SquareDimension()
						{
							return (LetterWidth() - ((Dimensions::HorizontalCount() - 1) * Dimensions::Separator())) / Dimensions::HorizontalCount();
						}

						static constexpr pixel_t ActualLetterWidth()
						{
							return (SquareDimension() * Dimensions::HorizontalCount()) + ((Dimensions::HorizontalCount() - 1) * Dimensions::Separator());
						}

						static constexpr pixel_t ActualLetterHeight()
						{
							return (SquareDimension() * Dimensions::VerticalCount()) + ((Dimensions::VerticalCount() - 1) * Dimensions::Separator());
						}

						static constexpr pixel_t UsedWidth()
						{
							return (ActualLetterWidth() * Dimensions::LetterCount()) + (Kerning() * (Dimensions::LetterCount() - 1));
						}

						static constexpr pixel_t RemainingWidth()
						{
							return UsedWidth() > MainLayout::InnerWidth() ? 0 : MainLayout::InnerWidth() - UsedWidth();
						}

						static constexpr pixel_t RemainingHeight()
						{
							return ActualLetterHeight() > MainLayout::InnerHeight() ? 0 : MainLayout::InnerHeight() - ActualLetterHeight();
						}

						static constexpr pixel_t Kerning()
						{
							return SquareDimension() * 2 + 1;
						}

						static constexpr pixel_t SquareX(const uint8_t xIndex)
						{
							return (xIndex * (SquareDimension() + Dimensions::Separator()));
						}

						static constexpr pixel_t SquareY(const uint8_t yIndex)
						{
							return (yIndex * (SquareDimension() + Dimensions::Separator()));
						}
					};

					struct LetterE
					{
						static constexpr pixel_t X()
						{
							return MainLayout::X() + ((MainLayout::Width() - LetterMatrixCalculator::UsedWidth()) / 2) + 1;
						}

						static constexpr pixel_t Y()
						{
							return MainLayout::Y() + ((MainLayout::Height() - LetterMatrixCalculator::ActualLetterHeight()) / 2) + 1;
						}

						static constexpr pixel_t SquareX(const uint8_t xIndex)
						{
							return X() + LetterMatrixCalculator::SquareX(xIndex);
						}

						static constexpr pixel_t SquareY(const uint8_t yIndex)
						{
							return Y() + LetterMatrixCalculator::SquareY(yIndex);
						}

						static constexpr pixel_t SquareDimension()
						{
							return LetterMatrixCalculator::SquareDimension();
						}
					};

					struct LetterG
					{
						static constexpr pixel_t X() { return LetterE::X() + LetterMatrixCalculator::ActualLetterWidth() + LetterMatrixCalculator::Kerning(); }
						static constexpr pixel_t Y() { return LetterE::Y(); }
						static constexpr pixel_t SquareX(const uint8_t xIndex)
						{
							return X() + LetterMatrixCalculator::SquareX(xIndex);
						}
						static constexpr pixel_t SquareY(const uint8_t yIndex)
						{
							return Y() + LetterMatrixCalculator::SquareY(yIndex);
						}
						static constexpr pixel_t SquareDimension()
						{
							return LetterMatrixCalculator::SquareDimension();
						}
					};

					struct LetterF
					{
						static constexpr pixel_t X() { return LetterG::X() + LetterMatrixCalculator::ActualLetterWidth() + LetterMatrixCalculator::Kerning(); }
						static constexpr pixel_t Y() { return LetterE::Y(); }
						static constexpr pixel_t SquareX(const uint8_t xIndex)
						{
							return X() + LetterMatrixCalculator::SquareX(xIndex);
						}
						static constexpr pixel_t SquareY(const uint8_t yIndex)
						{
							return Y() + LetterMatrixCalculator::SquareY(yIndex);
						}
						static constexpr pixel_t SquareDimension()
						{
							return LetterMatrixCalculator::SquareDimension();
						}
					};

					struct LetterX
					{
						static constexpr pixel_t X() { return LetterF::X() + LetterMatrixCalculator::ActualLetterWidth() + LetterMatrixCalculator::Kerning(); }
						static constexpr pixel_t Y() { return LetterE::Y(); }
						static constexpr pixel_t SquareX(const uint8_t xIndex)
						{
							return X() + LetterMatrixCalculator::SquareX(xIndex);
						}
						static constexpr pixel_t SquareY(const uint8_t yIndex)
						{
							return Y() + LetterMatrixCalculator::SquareY(yIndex);
						}
						static constexpr pixel_t SquareDimension()
						{
							return LetterMatrixCalculator::SquareDimension();
						}
					};

					struct ScreenBorder
					{
						static constexpr pixel_t X() { return LetterE::X() - MainLayout::InnerMargin(); }
						static constexpr pixel_t Y() { return LetterE::Y() - MainLayout::InnerMargin(); }
						static constexpr pixel_t Width() { return LetterMatrixCalculator::UsedWidth() + MainLayout::InnerMargin() * 2; }
						static constexpr pixel_t Height() { return LetterMatrixCalculator::ActualLetterHeight() + MainLayout::InnerMargin() * 2; }
					};
				};

				namespace Shaders
				{
					// Shared interface for letter shaders
					struct ILetterShader
					{
						virtual rgb_color_t Shade(const uint8_t x, const uint8_t y) = 0;
					};

					// Simple RNG-based ditherer, controlled via chance (0-255)
					class RngDitherer
					{
					private:
						uint16_t Rng = 64;

					public:
						bool Dither(const uint8_t chance)
						{
							Rng ^= Rng << 7;
							Rng ^= Rng >> 9;
							Rng ^= Rng << 8;

							return chance == 0 ? false : chance >= static_cast<uint8_t>(Rng);
						}
					};

					// Two-color horizontal gradient shader. Optionally controllable dithering.
					template<bool Monochrome,
						rgb_color_t foreground1,
						rgb_color_t foreground2,
						rgb_color_t background>
					class GradientShader : public ILetterShader
					{
					private:
						RngDitherer Ditherer{};

					public:
						uint8_t Chance = 250;

					public:
						GradientShader() : ILetterShader() {}

						rgb_color_t Shade(const uint8_t x, const uint8_t /*y*/) override
						{
							return Ditherer.Dither(Chance) ? GetColor(x) : background;
						}

						rgb_color_t GetColor(const uint8_t x) const
						{
							if (Monochrome)
							{
								return Colors::Foreground;
							}
							else
							{
								return Rgb::Color(
									(uint16_t(Rgb::R(foreground1)) * (Dimensions::HorizontalCount() - x) / Dimensions::HorizontalCount()) + (uint16_t(Rgb::R(foreground2)) * x / Dimensions::HorizontalCount()),
									(uint16_t(Rgb::G(foreground1)) * (Dimensions::HorizontalCount() - x) / Dimensions::HorizontalCount()) + (uint16_t(Rgb::G(foreground2)) * x / Dimensions::HorizontalCount()),
									(uint16_t(Rgb::B(foreground1)) * (Dimensions::HorizontalCount() - x) / Dimensions::HorizontalCount()) + (uint16_t(Rgb::B(foreground2)) * x / Dimensions::HorizontalCount()));
							}
						}
					};

					// RGB cycling shader with glitch effect. Optionally controllable dithering.
					template<bool Monochrome>
					class RgbShader : public ILetterShader
					{
					private:
						static constexpr uint8_t Glitch = 3;

					private:
						RngDitherer Ditherer{};

					public:
						uint8_t Chance = 250;
						uint16_t FrameCounter = 0;

					public:
						RgbShader() : ILetterShader() {}

						rgb_color_t Shade(const uint8_t x, const uint8_t y) override
						{
							return Ditherer.Dither(Chance) ? GetColor(x, y) : Colors::Background;
						}

					private:
						rgb_color_t GetColor(const uint8_t x, const uint8_t y) const
						{
							if (Monochrome)
							{
								if ((FrameCounter & 0b1) && (((x + y) + (FrameCounter >> 1)) % Glitch) != 0)
								{
									return Colors::Background;
								}
								else
								{
									return Colors::Foreground;
								}
							}
							else
							{
								const uint8_t index = ((x + y * 2) + (FrameCounter >> 1)) % 3;
								switch (index)
								{
								case 0:
									return Colors::Color1;
								case 1:
									return Colors::Color2;
								case 2:
								default:
									return Colors::Color3;
								}
							}
						}
					};
				}

				namespace Drawables
				{
					/// <summary>
					/// Draws dot matrix letters using provided layout and color shader.
					/// </summary>
					template<typename LetterLayout>
					struct LettersRasterizer
					{
						static void HorizontalLine(IFrameBuffer* frame,
							Shaders::ILetterShader* shader,
							const uint8_t width,
							const uint8_t xOffset, const uint8_t yOffset,
							const pixel_point_t translation)
						{
							for (uint8_t x = 0; x < width; x++)
							{
								const rgb_color_t color = shader->Shade(x + xOffset, yOffset);

								if (LetterLayout::SquareDimension() > 1)
								{
									frame->RectangleFill(color,
										LetterLayout::SquareX(x + xOffset) + translation.x, LetterLayout::SquareY(yOffset) + translation.y,
										LetterLayout::SquareX(x + xOffset) + LetterLayout::SquareDimension() - 1 + translation.x,
										LetterLayout::SquareY(yOffset) + LetterLayout::SquareDimension() - 1 + translation.y);
								}
								else
								{
									frame->Pixel(color,
										LetterLayout::SquareX(x + xOffset) + translation.x, LetterLayout::SquareY(yOffset) + translation.y);
								}
							}
						}
					};

					/// <summary>
					/// Draws screen border rectangle.
					/// </summary>
					template<typename Layout, bool Monochrome>
					struct Screen
					{
						pixel_point_t Translation{ 0, 0 };
						rgb_color_t Color = Colors::Foreground;

						void Draw(IFrameBuffer* frame)
						{
							frame->Rectangle(Color,
								Layout::X() + Translation.x, Layout::Y() + Translation.y,
								Layout::X() + Layout::Width() + Translation.x,
								Layout::Y() + Layout::Height() + Translation.y);
						}
					};

					/// <summary>
					/// Passive lines drawable - renders animated lines based on current state.
					/// </summary>
					template<typename Layout>
					struct LetterE
					{
						using Rasterizer = LettersRasterizer<Layout>;
						Shaders::ILetterShader* Shader = nullptr;
						pixel_point_t Translation{ 0, 0 };

						void Draw(IFrameBuffer* frame)
						{
							if (Shader != nullptr)
							{
								uint8_t yIndex = 0;
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - 1, 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - 1, 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
								Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
							}
						}
					};

					template<typename Layout>
					struct LetterG
					{
						using Rasterizer = LettersRasterizer<Layout>;
						Shaders::ILetterShader* Shader = nullptr;
						pixel_point_t Translation{ 0, 0 };

						void Draw(IFrameBuffer* frame)
						{
							uint8_t yIndex = 0;
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - 1 * 2, 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth() * 2 - 1, Dimensions::HorizontalCount() - Dimensions::LineWidth() * 2 + 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth() * 2 - 1, Dimensions::HorizontalCount() - Dimensions::LineWidth() * 2 + 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - 1 * 2, 1, yIndex++, Translation);
						}
					};

					template<typename Layout>
					struct LetterF
					{
						using Rasterizer = LettersRasterizer<Layout>;
						Shaders::ILetterShader* Shader = nullptr;
						pixel_point_t Translation{ 0, 0 };

						void Draw(IFrameBuffer* frame)
						{
							uint8_t yIndex = 0;
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::HorizontalCount() - Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex++, Translation);
						}
					};

					template<typename Layout>
					struct LetterX
					{
						using Rasterizer = LettersRasterizer<Layout>;
						Shaders::ILetterShader* Shader = nullptr;
						pixel_point_t Translation{ 0, 0 };

						void Draw(IFrameBuffer* frame)
						{
							uint8_t yIndex = 0;
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 1, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth() - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 1, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth() - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() / 2 - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() / 2 - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 1, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth() - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 1, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth() - 1, yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), 0, yIndex, Translation);
							Rasterizer::HorizontalLine(frame, Shader, Dimensions::LineWidth(), Dimensions::HorizontalCount() - Dimensions::LineWidth(), yIndex++, Translation);
						}
					};
				}

				namespace Views
				{
					// Basic static splash view rendering the logo.
					template<typename ParentLayout, bool Monochrome>
					class Splash : public Egfx::Framework::View::DrawablesView<
						Drawables::Screen<typename Layout<ParentLayout>::ScreenBorder, Monochrome>,
						Drawables::LetterE<typename Layout<ParentLayout>::LetterE>,
						Drawables::LetterG<typename Layout<ParentLayout>::LetterG>,
						Drawables::LetterF<typename Layout<ParentLayout>::LetterF>,
						Drawables::LetterX<typename Layout<ParentLayout>::LetterX>>
					{
					private:
						using Base = Egfx::Framework::View::DrawablesView<
							Drawables::Screen<typename Layout<ParentLayout>::ScreenBorder, Monochrome>,
							Drawables::LetterE<typename Layout<ParentLayout>::LetterE>,
							Drawables::LetterG<typename Layout<ParentLayout>::LetterG>,
							Drawables::LetterF<typename Layout<ParentLayout>::LetterF>,
							Drawables::LetterX<typename Layout<ParentLayout>::LetterX>>;

					protected:
						Shaders::GradientShader<Monochrome,
							Colors::Color1, Colors::Color2, Colors::Background> EShader{};
						Shaders::GradientShader<Monochrome,
							Colors::Color2, Colors::Color3, Colors::Background> GShader{};
						Shaders::RgbShader<Monochrome> FXShader{};

					public:
						Splash() : Base()
						{
							GetScreen().Color = Colors::Foreground;
							GetLetterE().Shader = &EShader;
							GetLetterG().Shader = &GShader;
							GetLetterF().Shader = &FXShader;
							GetLetterX().Shader = &FXShader;

							EShader.Chance = UINT8_MAX;
							GShader.Chance = UINT8_MAX;
							FXShader.Chance = UINT8_MAX;
						}

						~Splash() = default;

					public:
						Drawables::Screen<typename Layout<ParentLayout>::ScreenBorder, Monochrome>& GetScreen() { return this->template drawable<0>(); }
						Drawables::LetterE<typename Layout<ParentLayout>::LetterE>& GetLetterE() { return this->template drawable<1>(); }
						Drawables::LetterG<typename Layout<ParentLayout>::LetterG>& GetLetterG() { return this->template drawable<2>(); }
						Drawables::LetterF<typename Layout<ParentLayout>::LetterF>& GetLetterF() { return this->template drawable<3>(); }
						Drawables::LetterX<typename Layout<ParentLayout>::LetterX>& GetLetterX() { return this->template drawable<4>(); }

					protected:
						void ViewStep(const uint32_t /*frameTime*/, const uint16_t frameCounter) override
						{
							FXShader.FrameCounter = frameCounter;
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
						using Base::EShader;
						using Base::GShader;
						using Base::FXShader;

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
							static constexpr uint32_t Margin = AnimationDuration / 25;
							static constexpr uint32_t UsableTime = AnimationDuration - Margin;
							static constexpr uint32_t Hold = UsableTime / 3;

							static constexpr uint32_t SlideIn = ((UsableTime - Hold) / 2);
							static constexpr uint32_t FadeOut = UsableTime - Hold - SlideIn - 1;
						};

						static constexpr uint16_t VerticalShift = ParentLayout::Height() / 2;// -Layout<ParentLayout>::ScreenBorder::Height();

						Shaders::RngDitherer Ditherer{};
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
							auto& letterE = Base::GetLetterE();
							auto& letterG = Base::GetLetterG();
							auto& letterF = Base::GetLetterF();
							auto& letterX = Base::GetLetterX();

							switch (State)
							{
							case AnimationEnum::AnimationStart:
								EShader.Chance = 0;
								GShader.Chance = 0;
								FXShader.Chance = 0;
								screen.Color = GetBorderColor(0);
								screen.Translation.y = VerticalShift;
								letterE.Translation.y = VerticalShift;
								letterG.Translation.y = VerticalShift;
								letterF.Translation.y = VerticalShift;
								letterX.Translation.y = VerticalShift;
								State = AnimationEnum::SlideIn;
								AnimationStart = micros();
								break;
							case AnimationEnum::SlideIn:
							{
								const uint32_t elapsed = micros() - AnimationStart;
								if (elapsed >= Durations::SlideIn)
								{
									screen.Translation.y = 0;
									letterE.Translation.y = 0;
									letterG.Translation.y = 0;
									letterF.Translation.y = 0;
									letterX.Translation.y = 0;
									screen.Color = GetBorderColor(UINT8_MAX);
									EShader.Chance = UINT8_MAX;
									GShader.Chance = UINT8_MAX;
									FXShader.Chance = UINT8_MAX;
									State = AnimationEnum::Hold;
									AnimationStart = micros();
								}
								else
								{
									const uint16_t progress = ProgressScaler::GetProgress<Durations::SlideIn>(elapsed);
									const uint8_t curvedProgress = IntegerSignal::Curves::Root2U8<>::Get(progress >> 8);
									const pixel_t offset = VerticalShift - ((VerticalShift * curvedProgress) / UINT8_MAX);
									screen.Translation.y = offset;
									letterE.Translation.y = offset;
									letterG.Translation.y = offset;
									letterF.Translation.y = offset;
									letterX.Translation.y = offset;

									const auto raw = static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::SlideIn);
									const auto curved = IntegerSignal::Curves::Power3U8<>::Get(raw);
									EShader.Chance = curved;
									GShader.Chance = curved;
									FXShader.Chance = curved;
									screen.Color = GetBorderColor(Monochrome ? raw : curved);
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
									EShader.Chance = 0;
									GShader.Chance = 0;
									FXShader.Chance = 0;
									letterE.Translation.y = 0;
									letterG.Translation.y = 0;
									letterF.Translation.y = 0;
									letterX.Translation.y = 0;
									screen.Color = GetBorderColor(0);
									State = AnimationEnum::AnimationEnd;
									AnimationStart = micros();
								}
								else
								{
									const auto raw = static_cast<uint8_t>((elapsed * UINT8_MAX) / Durations::SlideIn);
									const auto curved = UINT8_MAX - IntegerSignal::Curves::Power3U8<>::Get(raw);
									EShader.Chance = curved;
									GShader.Chance = curved;
									FXShader.Chance = curved;
									screen.Color = GetBorderColor(curved);
								}
							}
							break;
							case AnimationEnum::AnimationEnd:
								break;
							default:
								break;
							}
						}

					private:
						rgb_color_t GetBorderColor(const uint8_t progress)
						{
							if (Monochrome)
							{
								if (progress > 0)
								{
									return Ditherer.Dither(progress) ? Colors::Foreground : Colors::Background;
								}
								else
								{
									return Colors::Background;
								}
							}
							else
							{
								return Rgb::Color(progress, progress, progress);
							}
						}
					};
				}

				template<typename ParentLayout, bool Monochrome>
				using SplashFrame = Egfx::Framework::View::FrameAdapter<Views::Splash<ParentLayout, Monochrome>>;

				template<typename ParentLayout, bool Monochrome,
					uint32_t AnimationDuration = 4000000,
					bool AutoStart = true>
				struct AnimatedViewFrame : Egfx::Framework::View::FrameAdapter<
					Views::AnimatedSplash<ParentLayout, Monochrome, AnimationDuration, AutoStart>>
				{
#if defined(SERIAL_LOG)
					void PrintDescription() const
					{
						Serial.println();
						Serial.print(F("Splash screen"));
					}
#endif
				};
			}
		}
	}
}
#endif