#ifndef _SHOWCASE_SLIDER_h
#define _SHOWCASE_SLIDER_h

#include <IntegerGlassModules.h>

#include "IDemoPage.h"
#include "Constants.h"

#include "Decorator.h"


namespace Showcase
{
	template<typename ParentLayout,
		typename PageSliderView,
		bool Monochrome = false>
	class SliderView : public Framework::View::CompositeView<
		ParentLayout,
		PageSliderView,
		Decorator::View<ParentLayout, Monochrome>>
	{
	private:
		using Base = Framework::View::CompositeView<
			ParentLayout,
			PageSliderView,
			Decorator::View<ParentLayout, Monochrome>>;

		static constexpr uint8_t PageCount = PageSliderView::ViewCount;
		enum class StateEnum : uint8_t
		{
			Ready,
			Starting,
			Showing,
			Returning
		};

		uint32_t StartTimestamp = 0;
		StateEnum State = StateEnum::Ready;
		uint8_t CurrentPage = 0;

		Framework::Animation::Interpolator::InterpolatorAdapter<Constants::Showcase::AnimationCurveType> AnimationCurve{};

	public:
		using ViewLayout = ParentLayout;

		SliderView() : Base()
		{
			auto& slider = Base::template view<0>();
			slider.SetAnimationCurve(AnimationCurve);
			slider.SetPageStartOverscroll(UFraction16::SCALAR_UNIT);
		}
		~SliderView() = default;

		void Start()
		{
			if (State != StateEnum::Ready)
			{
				return;
			}

			State = StateEnum::Starting;
		}

		bool IsReady() const
		{
			return State == StateEnum::Ready;
		}

	protected:
		bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
		{
			if (State == StateEnum::Ready)
			{
				return true;
			}

			auto& slider = Base::template view<0>();
			if (State == StateEnum::Starting)
			{
				SetPage(0);
				StartTimestamp = frameTime;
				State = StateEnum::Showing;
			}

			if (!slider.IsScrollAnimating())
			{
				switch (State)
				{
				case StateEnum::Returning:
					State = StateEnum::Ready;
					break;
				case StateEnum::Showing:
					if (frameTime - StartTimestamp >= Constants::Showcase::PageDemoDuration)
					{
						if (CurrentPage + 1u >= PageCount)
						{
							Base::template view<1>().Hide(Constants::Showcase::PageSlideDuration);
							slider.AnimateToScroll(-static_cast<typename PageSliderView::scroll_t>(UFraction16::SCALAR_UNIT), Constants::Showcase::PageSlideDuration);
							State = StateEnum::Returning;
						}
						else
						{
							const uint8_t nextPage = static_cast<uint8_t>(CurrentPage + 1u);
							SetPage(nextPage);
						}

						StartTimestamp = frameTime;
					}
					break;
				case StateEnum::Ready:
				default:
					break;
				}
			}

			return Base::ViewStep(frameTime, frameCounter);
		}

	private:
			void SetPage(const uint8_t page)
		{
			auto& slider = Base::template view<0>();
			auto& decorator = Base::template view<1>();

			if (!slider.template VisitView<IDemoPage>(page, [&](IDemoPage& pageView)
				{
					decorator.SetPageName(pageView.GetPageName(), Constants::Showcase::PageSlideDuration);
				}))
			{
				decorator.SetPageName("?", Constants::Showcase::PageSlideDuration);
			}
			slider.AnimateToPage(static_cast<typename PageSliderView::scroll_t>(page), Constants::Showcase::PageSlideDuration);

			CurrentPage = page;
		}
	};
}
#endif