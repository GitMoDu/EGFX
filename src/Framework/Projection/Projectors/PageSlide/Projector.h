#ifndef _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_PROJECTOR_h
#define _EGFX_FRAMEWORK_PROJECTION_PROJECTORS_PAGE_SLIDE_PROJECTOR_h

#include "../../Model.h"
#include "../../Control/Scroll.h"
#include "../../Layout.h"

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
					using scroll_t = Control::Scroll::scroll_t;

					/// <summary>
					/// Stateless page-slide viewport policy. It converts logical page scroll into
					/// clipped child bounds and translation.
					/// </summary>
					template<typename ParentLayout,
						typename Style = PageSlideStyle<>
					>
					class Projector
					{
					public:
						using ItemLayouter = Layout::ListItemLayouter<ParentLayout, Style>;

						using ControllerType = Control::Scroll::PagedScrollController;
						using StateType = Control::Scroll::PagedScrollState;
					private:
						static constexpr uint16_t PageDimension = (Style::Orientation == OrientationEnum::LeftToRight || Style::Orientation == OrientationEnum::RightToLeft)
							? ItemLayouter::PageLayout::Width() : ItemLayouter::PageLayout::Height();
						static constexpr bool IsHorizontal = (Style::Orientation == OrientationEnum::LeftToRight) || (Style::Orientation == OrientationEnum::RightToLeft);

						static constexpr uint16_t ViewDimension = PageDimension;

						static constexpr int8_t ScrollDirection = (Style::Orientation == OrientationEnum::LeftToRight || Style::Orientation == OrientationEnum::TopToBottom) ? 1 : -1;

					public:
						Projector() {}

						~Projector() = default;

						static viewport_t GetViewport(const uint8_t viewIndex, const StateType& state)
						{
							return GetPageState(viewIndex, state.PageCount, state.Scroll);
						}

					public:
						static viewport_t GetPageState(const uint8_t index, const uint8_t pageCount, const scroll_t scroll)
						{
							if (pageCount == 0 || index >= pageCount)
								return viewport_t{ 1, 1, 0, 0, 0, 0 };

							static constexpr scroll_t pageStride = UFraction16::SCALAR_UNIT;
							static constexpr scroll_t overscroll = UFraction16::SCALAR_UNIT / 2;

							const scroll_t maxScroll = static_cast<scroll_t>(pageCount - 1) * pageStride;
							const scroll_t boundedScroll = LimitValue<scroll_t>(scroll, -overscroll, maxScroll + overscroll);
							const scroll_t pagePosition = static_cast<scroll_t>(index) * pageStride;
							const scroll_t relativePosition = pagePosition - boundedScroll;
							const scroll_t distance = AbsValue(relativePosition);
							if (distance > pageStride)
								return viewport_t{ 1, 1, 0, 0, 0, 0 };

							const UFraction16::scalar_t distanceFraction = UFraction16::GetScalar<uint32_t>(
								static_cast<uint32_t>(distance),
								static_cast<uint32_t>(pageStride));
							const uint32_t translationMagnitude = UFraction16::Fraction<uint32_t>(
								distanceFraction,
								static_cast<uint32_t>(ViewDimension));
							const int16_t translation = static_cast<int16_t>(
								(relativePosition < 0 ? -static_cast<int32_t>(translationMagnitude) : static_cast<int32_t>(translationMagnitude)) * ScrollDirection);

							const int16_t translationX = IsHorizontal ? translation : 0;
							const int16_t translationY = IsHorizontal ? 0 : translation;

							return GetViewportInternal(translationX, translationY);
						}

						static viewport_t GetViewportInternal(const int16_t translationX, const int16_t translationY)
						{
							const int16_t viewX = static_cast<int16_t>(ItemLayouter::ViewLayout::X());
							const int16_t viewY = static_cast<int16_t>(ItemLayouter::ViewLayout::Y());
							const int16_t viewWidth = static_cast<int16_t>(ItemLayouter::ViewLayout::Width());
							const int16_t viewHeight = static_cast<int16_t>(ItemLayouter::ViewLayout::Height());
							const int16_t left = ParentLayout::X() - viewX - translationX;
							const int16_t top = ParentLayout::Y() - viewY - translationY;
							const int16_t right = ParentLayout::X() + ParentLayout::Width() - 1 - viewX - translationX;
							const int16_t bottom = ParentLayout::Y() + ParentLayout::Height() - 1 - viewY - translationY;
							const int16_t localRight = viewWidth - 1;
							const int16_t localBottom = viewHeight - 1;
							const int16_t clipLeft = MaxValue<int16_t>(0, left);
							const int16_t clipTop = MaxValue<int16_t>(0, top);
							const int16_t clipRight = MinValue<int16_t>(localRight, right);
							const int16_t clipBottom = MinValue<int16_t>(localBottom, bottom);

							if (clipLeft > clipRight || clipTop > clipBottom)
								return viewport_t{ 1, 1, 0, 0, translationX, translationY };

							return viewport_t{
								static_cast<uint16_t>(clipLeft),
								static_cast<uint16_t>(clipTop),
								static_cast<uint16_t>(clipRight),
								static_cast<uint16_t>(clipBottom),
								translationX,
								translationY
							};
						}
					};
				}
			}
		}
	}
}
#endif