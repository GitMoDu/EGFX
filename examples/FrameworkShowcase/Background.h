#ifndef _BACKGROUND_h
#define _BACKGROUND_h

#include <IntegerGlassModules.h>

#include "Constants.h"

namespace Showcase
{
	using namespace IntegerGlass;

	template<typename Drawable, bool Monochrome>
	struct BackgroundColorSetter;

	template<typename Drawable>
	struct BackgroundColorSetter<Drawable, true>
	{
		static void Set(Drawable&, const angle_t, const uint8_t) {}
	};

	template<typename Drawable>
	struct BackgroundColorSetter<Drawable, false>
	{
		static void Set(Drawable& drawable, const angle_t hue, const uint8_t value)
		{
			drawable.ColorSource.Color = Rgb::ColorFromHSV(hue, Constants::Background::Saturation, value);
		}
	};

	template<typename ParentLayout>
	using ColorBackgroundDrawable = Modules::Kit::Drawable::Shape::RectangleFill<
		typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
		ParentLayout,
		Framework::Shader::Pixel::TemplateShader<
		typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
		Framework::Shader::Source::SingleColor<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t, Constants::Background::Color>
		>
	>;

	template<typename ParentLayout>
	using MonochromeBackgroundDrawable = Framework::Drawable::MockDrawable<
		typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
		Framework::Shader::Pixel::TemplateShader<
		typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t,
		Framework::Shader::Source::StaticColor<typename Framework::AutoDimension::ByLayout<ParentLayout>::dimension_t>>>;

	template<typename ParentLayout, bool Monochrome>
	using BackgroundDrawable = typename IntegerSignal::TypeTraits::TypeConditional::conditional_type<
		MonochromeBackgroundDrawable<ParentLayout>,
		ColorBackgroundDrawable<ParentLayout>,
		Monochrome>::type;

	template<typename ParentLayout, bool Monochrome>
	class BackgroundView : public Framework::View::DrawablesView<
		ParentLayout,
		BackgroundDrawable<ParentLayout, Monochrome>>
	{
	private:
		using Base = Framework::View::DrawablesView<
			ParentLayout,
			BackgroundDrawable<ParentLayout, Monochrome>>;

	private:
		Framework::Animation::Animator<angle_t> ColorAnimator{};
		Framework::Animation::Animator<uint8_t> ValueAnimator{};

		enum class StateEnum
		{
			Invisible,
			FadingIn,
			Holding,
			FadingOut
		};

		StateEnum State = StateEnum::Invisible;
		uint8_t CurrentValue = 0;

	public:
		BackgroundView() : Base()
		{
			ColorAnimator.SetLoop(true);
			ColorAnimator.SetDuration(Constants::Background::AnimationDuration);

			// Continuous color animation.
			ColorAnimator.Start(0, ANGLE_RANGE);
		}
		~BackgroundView() = default;

		bool IsVisible() const
		{
			return State != StateEnum::Invisible || Base::IsVisible();
		}

		void FadeIn()
		{
			ValueAnimator.SetDuration(Constants::Background::FadeInDuration);
			ValueAnimator.Start(CurrentValue, Constants::Background::Value);
			State = StateEnum::FadingIn;
		}

		void FadeOut()
		{
			ValueAnimator.SetDuration(Constants::Background::FadeOutDuration);
			ValueAnimator.Start(CurrentValue, 0);
			State = StateEnum::FadingOut;
		}

	protected:
		bool ViewStep(const uint32_t frameTime, const uint16_t /*frameCounter*/) override
		{
			const auto colorState = ColorAnimator.Step(frameTime + Constants::Background::ColorTimeOffset);

			switch (State)
			{
			case StateEnum::FadingIn:
				if (!ValueAnimator.IsAnimating())
				{
					State = StateEnum::Holding;
				}
				else
				{
					const auto valueState = ValueAnimator.Step(frameTime);
					CurrentValue = valueState.Value;
				}
				break;
			case StateEnum::Holding:
				CurrentValue = Constants::Background::Value;
				break;
			case StateEnum::FadingOut:
				if (!ValueAnimator.IsAnimating())
				{
					State = StateEnum::Invisible;
					CurrentValue = 0;
				}
				else
				{
					const auto valueState = ValueAnimator.Step(frameTime);
					CurrentValue = valueState.Value;
				}
				break;
			case StateEnum::Invisible:
			default:
				CurrentValue = 0;
				break;
			}

			const uint8_t value = IntegerSignal::Curves::Root2U8<>::Get(CurrentValue);
			BackgroundColorSetter<BackgroundDrawable<ParentLayout, Monochrome>, Monochrome>::Set(
				Base::template drawable<0>(), colorState.Value, value);

			return true;
		}

	};
}

#endif