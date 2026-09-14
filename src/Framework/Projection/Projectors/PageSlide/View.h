#ifndef _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_VIEW_h
#define _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_VIEW_h

#include "Projector.h"
#include "../../View/CompositeListView.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Projection
		{
			namespace Projectors
			{
				namespace PageSlide
				{
					/// <summary>
					/// Page-slide semantic view that owns page control and optional linear scroll animation
					/// while the base projection host performs viewport forwarding.
					/// </summary>
					/// <typeparam name="ParentLayout">The parent layout within which the pages are arranged.</typeparam>
					/// <typeparam name="Style">The style configuration for the page slide. Must be a super-set of PageSlideStyle.</typeparam>
					/// <typeparam name="...ViewTypes">The types of the child views managed by this view.</typeparam>
					template<typename ParentLayout,
						typename Style = PageSlideStyle<>,
						typename... ViewTypes>
					class View : public Projection::View::CompositeListView<ParentLayout,
						Projectors::PageSlide::Projector<ParentLayout, Style>, ViewTypes...>
					{
					public:
						using ViewLayout = ParentLayout;
						using scroll_t = Control::Scroll::scroll_t;
						using scalar_t = UFraction16::scalar_t;

					private:
						using Base = Projection::View::CompositeListView<ParentLayout,
							Projectors::PageSlide::Projector<ParentLayout, Style>, ViewTypes...>;

						scroll_t AnimationStart = 0;
						scroll_t AnimationTarget = 0;
						uint32_t AnimationStartTime = 0;
						uint32_t AnimationDuration = 0;
						bool AnimationActive = false;
						bool AnimationStarted = false;

					public:
						View() : Base() {}
						~View() = default;

						void SetScroll(const scroll_t scroll)
						{
							AnimationActive = false;
							Base::ControllerState().SetScroll(scroll);
						}

						void AnimateScroll(const scroll_t target, const uint32_t duration)
						{
							AnimationStart = Base::ControllerState().GetScroll();
							AnimationTarget = target;
							AnimationDuration = duration;
							AnimationActive = duration != 0;
							AnimationStarted = false;

							if (duration == 0)
							{
								Base::ControllerState().SetScroll(target);
							}
						}

						bool IsScrollAnimating() const
						{
							return AnimationActive;
						}

						void SetPagePosition(const uint8_t index, const scalar_t offset)
						{
							AnimationActive = false;
							Base::ControllerState().SetPagePosition(index, offset);
						}

						void SetPageStartOverscroll(const scalar_t offset)
						{
							AnimationActive = false;
							Base::ControllerState().SetPageStartOverscroll(offset);
						}

						scroll_t GetScroll() const
						{
							return Base::ControllerState().GetScroll();
						}

						uint8_t GetPageCount() const
						{
							return Base::ControllerState().GetPageCount();
						}

					protected:
						bool ViewStep(const uint32_t frameTime, const uint16_t frameCounter) override
						{
							if (AnimationActive)
							{
								if (!AnimationStarted)
								{
									AnimationStartTime = frameTime;
									AnimationStarted = true;
								}

								const uint32_t elapsed = frameTime - AnimationStartTime;
								if (elapsed >= AnimationDuration)
								{
									this->ControllerState().SetScroll(AnimationTarget);
									AnimationActive = false;
									AnimationStarted = false;
								}
								else
								{
									const int64_t delta = static_cast<int64_t>(AnimationTarget) - AnimationStart;
									const scroll_t scroll = static_cast<scroll_t>(
										static_cast<int64_t>(AnimationStart) +
										(delta * elapsed) / AnimationDuration);
									Base::ControllerState().SetScroll(scroll);
								}
							}

							return Base::ViewStep(frameTime, frameCounter);
						}
					};
				}
			}
		}
	}
}
#endif