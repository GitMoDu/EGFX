#ifndef _TITLE_h
#define _TITLE_h

#include <IntegerGlassModules.h>
#include "Constants.h"

namespace Showcase
{
	namespace Title
	{
		using namespace IntegerGlass;

		using FontType = Modules::Kit::Font::Vector::Epoxy::FontType8x8;

		using GlyphStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		using PaletteType = Framework::Shader::Image::Vector::ColorPalette<1>;

		template<typename ParentLayout>
		using TitleLayout = ParentLayout;

		template<typename ParentLayout>
		using dimension_t = typename Framework::AutoDimension::ByLayout<TitleLayout<ParentLayout>>::dimension_t;


		template<typename ParentLayout>
		static constexpr int16_t TextKerning()
		{
			return Constants::Showcase::TitleHeight<ParentLayout>() / 5;
		}

		template<typename ParentLayout>
		static constexpr int16_t TextWidth()
		{
			return static_cast<int16_t>((static_cast<uint16_t>(FontType::Width) * Constants::Showcase::TitleHeight<ParentLayout>()) / FontType::Height);
		}

		template<typename ParentLayout, bool Monochrome>
		using TextDrawable = Framework::Text::Vector::Drawable::Text<
			dimension_t<TitleLayout<ParentLayout>>,
			TitleLayout<ParentLayout>,
			FontType, GlyphStyle, PaletteType,
			typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
			Modules::Logo::Shader::Color::ScreenDitherAlpha<dimension_t<TitleLayout<ParentLayout>>>,
			Framework::Shader::Color::NoShader<dimension_t<TitleLayout<ParentLayout>>>,
			Monochrome>::type,
			Framework::Shader::Transform::NoTransform<dimension_t<TitleLayout<ParentLayout>>>,
			Framework::Shader::Pixel::BlendModeEnum::Add
		>;

		template<typename ParentLayout, bool Monochrome>
		class View : public Framework::View::DrawablesView<
			TitleLayout<ParentLayout>,
			TextDrawable<ParentLayout, Monochrome>
		>
		{
		private:
			using Base = Framework::View::DrawablesView<
				TitleLayout<ParentLayout>,
				TextDrawable<ParentLayout, Monochrome>
			>;

		private:
			Framework::Animation::Animator<UFraction16::scalar_t> FadeInAnimator{};
			Framework::Animation::Animator<UFraction16::scalar_t> FadeOutAnimator{};
			Framework::Animation::Interpolator::InterpolatorAdapter<Modules::Kit::Animation::Interpolator::EaseIn> FadeInInterpolator{};
			Framework::Animation::Interpolator::InterpolatorAdapter<Modules::Kit::Animation::Interpolator::EaseOut> FadeOutInterpolator{};

			const char* PendingTitle = nullptr;

			enum class StateEnum
			{
				Idle,
				FadeIn,
				FadingIn,
				Holding,
				FadeOut,
				FadingOut
			};

			StateEnum State = StateEnum::Idle;
			uint32_t HoldStart = 0;
			using ColorDispatchTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
				IntegerSignal::TypeTraits::TypeDispatch::TrueType,
				IntegerSignal::TypeTraits::TypeDispatch::FalseType,
				Monochrome>::type;

		public:
			View() : Base()
			{
				Label().SetFontSize(TextWidth<ParentLayout>(), Constants::Showcase::TitleHeight<ParentLayout>(), false);
				Label().SetKerningWidth(TextKerning<ParentLayout>());
				Label().GetPalette().SetColor(0, RGB_COLOR_BLACK);
				FadeInAnimator.SetInterpolator(FadeInInterpolator);
				FadeOutAnimator.SetInterpolator(FadeOutInterpolator);
			}
			~View() = default;

			void SetTitle(const char* name, const uint32_t fadeDuration)
			{
				if (fadeDuration != 0)
				{
					PendingTitle = name;
					const uint32_t phaseDuration = fadeDuration / 3;
					FadeOutAnimator.SetDuration(phaseDuration);
					FadeInAnimator.SetDuration(phaseDuration);
					HoldDuration = phaseDuration;
					State = StateEnum::FadeIn;
				}
				else
				{
					State = StateEnum::Idle;
					PendingTitle = nullptr;
					Label().SetText(name);
					Label().ColorSource.Color = RGB_COLOR_BLACK;
					SetAlpha(UFraction16::SCALAR_UNIT);
				}
			}

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				switch (State)
				{
				case StateEnum::FadeOut:
					SetPaletteColor(Constants::Decorator::TextColor, ColorDispatchTag{});
					FadeOutAnimator.Start(UFraction16::SCALAR_UNIT, 0);
					State = StateEnum::FadingOut;
					break;
				case StateEnum::FadingOut:
					if (FadeOutAnimator.IsAnimating())
					{
						const auto animationState = FadeOutAnimator.Step(frameTime);
						SetAlpha(animationState.Value);
						SetPaletteColor(Rgb::Interpolate<UFraction16::scalar_t>(animationState.Value, RGB_COLOR_BLACK, Constants::Decorator::TextColor), ColorDispatchTag{});
					}
					else
					{
						State = StateEnum::Idle;
						return false; // Skip draw, nothing to show.
					}
					break;
				case StateEnum::FadeIn:
					SetPaletteColor(RGB_COLOR_BLACK, ColorDispatchTag{});
					FadeInAnimator.Start(0, UFraction16::SCALAR_UNIT);
					Label().SetText(PendingTitle);
					State = StateEnum::FadingIn;
					return false; // Skip draw, nothing to show.
					break;
				case StateEnum::FadingIn:
					if (FadeInAnimator.IsAnimating())
					{
						const auto animationState = FadeInAnimator.Step(frameTime);
						SetAlpha(animationState.Value);
						SetPaletteColor(Rgb::Interpolate<UFraction16::scalar_t>(animationState.Value, RGB_COLOR_BLACK, Constants::Decorator::TextColor), ColorDispatchTag{});
					}
					else
					{
						SetPaletteColor(Constants::Decorator::TextColor, ColorDispatchTag{});
						SetAlpha(UFraction16::SCALAR_UNIT);
						HoldStart = frameTime;
						State = StateEnum::Holding;
					}

					break;
				case StateEnum::Holding:
					if (frameTime - HoldStart >= HoldDuration)
					{
						State = StateEnum::FadeOut;
					}
					break;
				case StateEnum::Idle:
				default:
					break;
				}

				return true;
			}

		private:
			void SetPaletteColor(const rgb_color_t color, IntegerSignal::TypeTraits::TypeDispatch::TrueType) {}

			void SetPaletteColor(const rgb_color_t color, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
			{
				Label().GetPalette().SetColor(0, color);
			}

			void SetAlpha(const UFraction16::scalar_t alpha)
			{
				SetAlpha(alpha, ColorDispatchTag{});
			}

			void SetAlpha(const UFraction16::scalar_t alpha, IntegerSignal::TypeTraits::TypeDispatch::TrueType)
			{
				Label().ColorShader.Alpha = UFraction16::Fraction<uint8_t>(alpha, UINT8_MAX);
			}

			void SetAlpha(const UFraction16::scalar_t, IntegerSignal::TypeTraits::TypeDispatch::FalseType) {}

			uint32_t HoldDuration = 0;

			TextDrawable<ParentLayout, Monochrome>& Label()
			{
				return Base::template drawable<0>();
			}
		};
	}
}

#endif

