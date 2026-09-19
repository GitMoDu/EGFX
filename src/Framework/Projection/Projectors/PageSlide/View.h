#ifndef _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_VIEW_h
#define _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_VIEW_h

#include "Projector.h"
#include "../../View/CompositeListView.h"
#include "../../../Animation/Animator.h"

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

						Framework::Animation::Animator<scroll_t> ScrollAnimator{};
						scroll_t PendingTarget = 0;
						uint32_t PendingDuration = 0;
						bool AnimationPending = false;

					public:
						View() : Base()
						{
							ScrollAnimator.SetDuration(PendingDuration);
						}
						~View() = default;

						void SetScroll(const scroll_t scroll)
						{
							ScrollAnimator.Cancel();
							Base::ControllerState().SetScroll(scroll);
						}

						void SetAnimationCurve(const Framework::Animation::Interpolator::IInterpolator& interpolator)
						{
							ScrollAnimator.SetInterpolator(interpolator);
						}

						void SetAnimationCurve(const Framework::Animation::Interpolator::IInterpolator* interpolator)
						{
							ScrollAnimator.SetInterpolator(interpolator);
						}

						void AnimateToPage(const uint8_t index, const uint32_t duration)
						{
							const scroll_t target = static_cast<scroll_t>(index) * UFraction16::SCALAR_UNIT;
							PendingTarget = target;
							PendingDuration = duration;
							AnimationPending = true;
						}

						void AnimateToScroll(const scroll_t target, const uint32_t duration)
						{
							PendingTarget = target;
							PendingDuration = duration;
							AnimationPending = true;
						}

						bool IsScrollAnimating() const
						{
							return AnimationPending || ScrollAnimator.IsAnimating();
						}

						bool IsVisible() const
						{
							return IsScrollAnimating() || Base::IsVisible();
						}

						void SetPagePosition(const uint8_t index, const scalar_t offset)
						{
							ScrollAnimator.Cancel();
							AnimationPending = false;
							Base::ControllerState().SetPagePosition(index, offset);
						}

						void SetPageStartOverscroll(const scalar_t offset)
						{
							ScrollAnimator.Cancel();
							AnimationPending = false;
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
							if (AnimationPending)
							{
								ScrollAnimator.SetDuration(PendingDuration);
								ScrollAnimator.Start(Base::ControllerState().GetScroll(), PendingTarget);
								AnimationPending = false;
							}

							if (ScrollAnimator.IsAnimating())
							{
								const auto state = ScrollAnimator.Step(frameTime);
								Base::ControllerState().SetScroll(state.Value);
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