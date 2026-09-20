#ifndef _DECORATOR_h
#define _DECORATOR_h

#include <IntegerGlassFramework.h>
#include <Modules/Kit/Include.h>
#include <IntegerGlassModules.h>

#include "Constants.h"

namespace Showcase
{
	namespace Decorator
	{
		using namespace IntegerGlass;

		using FontType = Constants::Showcase::DecoratorFontType;

		using GlyphStyle = Framework::Image::TemplateImageStyle<
			Framework::Layout::AlignmentEnum::MiddleCenter>;

		using PaletteType = Framework::Shader::Image::Vector::ColorPalette<1>;

		template<typename ParentLayout>
		using DecoratorLayout = Framework::TemplateLayout<
			ParentLayout::X(),
			ParentLayout::Y() + ParentLayout::Height() - Constants::Showcase::StripHeight<ParentLayout>(),
			ParentLayout::Width(),
			Constants::Showcase::StripHeight<ParentLayout>()
		>;

		template<typename ParentLayout>
		using TextLayout = Framework::Layout::Translate<
			DecoratorLayout<ParentLayout>,
			0,
			Constants::Showcase::StripHeight<ParentLayout>() / 6>;

		template<typename ParentLayout>
		using dimension_t = typename Framework::AutoDimension::ByLayout<DecoratorLayout<ParentLayout>>::dimension_t;

		template<typename ParentLayout>
		static constexpr int16_t TextHeight()
		{
			return (Constants::Showcase::StripHeight<ParentLayout>() * 6) / 10;
		}

		template<typename ParentLayout>
		static constexpr int16_t TextWidth()
		{
			return static_cast<int16_t>((static_cast<uint16_t>(FontType::Width) * TextHeight<ParentLayout>()) / FontType::Height);
		}

		template<typename ParentLayout, bool Monochrome>
		using TextDrawable = Framework::Text::Vector::Drawable::Text<
			dimension_t<TextLayout<ParentLayout>>,
			TextLayout<ParentLayout>,
			FontType, GlyphStyle, PaletteType,
			typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
			Modules::Logo::Shader::Color::ScreenDitherAlpha<dimension_t<TextLayout<ParentLayout>>>,
			Framework::Shader::Color::NoShader<dimension_t<TextLayout<ParentLayout>>>,
			Monochrome>::type,
			Framework::Shader::Transform::NoTransform<dimension_t<TextLayout<ParentLayout>>>,
			Framework::Shader::Pixel::BlendModeEnum::Add
		>;

		template<typename ParentLayout>
		using ColorShadowDrawable = Modules::Kit::Drawable::Shadow::CardinalGradient<
			dimension_t<ParentLayout>,
			DecoratorLayout<ParentLayout>,
			Modules::Kit::Drawable::Shadow::CardinalOrientationEnum::BottomToTop,
			Framework::Shader::Source::SingleColor<
			dimension_t<ParentLayout>,
			Constants::Decorator::ShadowColor>>;

		template<typename ParentLayout>
		using MonochromeShadowDrawable = Framework::Drawable::MockDrawable<
			dimension_t<ParentLayout>,
			Framework::Shader::Pixel::TemplateShader<dimension_t<ParentLayout>>>;

		template<typename ParentLayout, bool Monochrome>
		using ShadowDrawable = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
			MonochromeShadowDrawable<ParentLayout>,
			ColorShadowDrawable<ParentLayout>,
			Monochrome>::type;

		template<typename ParentLayout, bool Monochrome>
		class View : public Framework::View::DrawablesView<
			DecoratorLayout<ParentLayout>,
			ShadowDrawable<ParentLayout, Monochrome>,
			TextDrawable<ParentLayout, Monochrome>
		>
		{
		private:
			using Base = Framework::View::DrawablesView<
				DecoratorLayout<ParentLayout>,
				ShadowDrawable<ParentLayout, Monochrome>,
				TextDrawable<ParentLayout, Monochrome>
			>;

		private:
			Framework::Animation::Animator<UFraction16::scalar_t> FadeInAnimator{};
			Framework::Animation::Animator<UFraction16::scalar_t> FadeOutAnimator{};
			Framework::Animation::Interpolator::InterpolatorAdapter<Modules::Kit::Animation::Interpolator::EaseIn> FadeInInterpolator{};
			Framework::Animation::Interpolator::InterpolatorAdapter<Modules::Kit::Animation::Interpolator::EaseOut> FadeOutInterpolator{};

			const char* PendingPageName = nullptr;
			UFraction16::scalar_t Visibility = 0;

			enum class StateEnum
			{
				Hidden,
				Idle,
				FadingOut,
				FadeIn,
				FadingIn,
				Hiding
			};

			StateEnum State = StateEnum::Idle;
			using ColorDispatchTag = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
				IntegerSignal::TypeTraits::TypeDispatch::TrueType,
				IntegerSignal::TypeTraits::TypeDispatch::FalseType,
				Monochrome>::type;

		public:
			using ViewLayout = DecoratorLayout<ParentLayout>;

			View() : Base()
			{
				Label().SetFontSize(TextWidth<ParentLayout>(), TextHeight<ParentLayout>(), false);
				Label().SetSpaceWidth(TextWidth<ParentLayout>() / 3);
				SetVisibility(0);
				FadeInAnimator.SetInterpolator(FadeInInterpolator);
				FadeOutAnimator.SetInterpolator(FadeOutInterpolator);
			}
			~View() = default;

			void Hide(const uint32_t fadeDuration)
			{
				PendingPageName = nullptr;
				FadeOutAnimator.SetDuration(fadeDuration);
				if (fadeDuration == 0 || Visibility == 0)
				{
					SetVisibility(0);
					State = StateEnum::Hidden;
				}
				else
				{
					FadeOutAnimator.Start(Visibility, 0);
					State = StateEnum::Hiding;
				}
			}

			void SetPageName(const char* name, const uint32_t fadeDuration)
			{
				if (fadeDuration != 0)
				{
					PendingPageName = name;
					FadeOutAnimator.SetDuration((fadeDuration * 2) / 3);
					FadeInAnimator.SetDuration(fadeDuration / 4);
					if (State == StateEnum::Hidden)
					{
						State = StateEnum::FadeIn;
					}
					else
					{
						FadeOutAnimator.Start(Visibility, 0);
						State = StateEnum::FadingOut;
					}
				}
				else
				{
					State = StateEnum::Idle;
					PendingPageName = nullptr;
					Label().SetText(name);
					SetVisibility(0);
				}
			}

		protected:
			bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
			{
				switch (State)
				{
				case StateEnum::Hiding:
					if (FadeOutAnimator.IsAnimating())
					{
						const auto animationState = FadeOutAnimator.Step(frameTime);
						SetVisibility(animationState.Value);
					}
					else
					{
						SetPaletteColor(RGB_COLOR_BLACK, ColorDispatchTag{});
						SetShadowOpacity(0);
						State = StateEnum::Hidden;
					}
					break;
				case StateEnum::FadingOut:
					if (FadeOutAnimator.IsAnimating())
					{
						const auto animationState = FadeOutAnimator.Step(frameTime);
						SetVisibility(animationState.Value);
					}
					else
					{
						State = StateEnum::FadeIn;
						return false; // Skip draw, nothing to show.
					}
					break;
				case StateEnum::FadeIn:
					SetVisibility(Visibility);
					FadeInAnimator.Start(Visibility, UFraction16::SCALAR_UNIT);
					Label().SetText(PendingPageName);
					State = StateEnum::FadingIn;
					return false; // Skip draw, nothing to show.
					break;
				case StateEnum::FadingIn:
					if (FadeInAnimator.IsAnimating())
					{
						const auto animationState = FadeInAnimator.Step(frameTime);
						SetVisibility(animationState.Value);
					}
					else
					{
						SetVisibility(UFraction16::SCALAR_UNIT);
						State = StateEnum::Idle;
					}
					break;
				case StateEnum::Hidden:
				case StateEnum::Idle:
				default:
					break;
				}

				return true;
			}

		private:
			void SetVisibility(const UFraction16::scalar_t visibility)
			{
				Visibility = visibility;
				SetShadowOpacity(visibility);
				SetPaletteColor(Rgb::Interpolate<UFraction16::scalar_t>(visibility, RGB_COLOR_BLACK, Constants::Decorator::TextColor), ColorDispatchTag{});
				SetAlpha(visibility);
			}

			void SetShadowOpacity(const UFraction16::scalar_t opacity)
			{
				SetShadowOpacity(opacity, ColorDispatchTag{});
			}

			void SetShadowOpacity(const UFraction16::scalar_t opacity, IntegerSignal::TypeTraits::TypeDispatch::TrueType) {}

			void SetShadowOpacity(const UFraction16::scalar_t opacity, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
			{
				Shadow().SetOpacity(opacity);
			}

			void SetPaletteColor(const rgb_color_t color, IntegerSignal::TypeTraits::TypeDispatch::TrueType) {}

			void SetPaletteColor(const rgb_color_t color, IntegerSignal::TypeTraits::TypeDispatch::FalseType)
			{
				Label().GetPalette().SetColor(0, color);
			}

			TextDrawable<ParentLayout, Monochrome>& Label()
			{
				return Base::template drawable<1>();
			}

			ShadowDrawable<ParentLayout, Monochrome>& Shadow()
			{
				return Base::template drawable<0>();
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
		};
	}
}

#endif

