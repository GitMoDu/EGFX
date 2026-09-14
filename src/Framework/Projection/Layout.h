#ifndef _EGFX_FRAMEWORK_PROJECTION_LAYOUT_h
#define _EGFX_FRAMEWORK_PROJECTION_LAYOUT_h

#include "Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Projection
		{
			namespace Layout
			{
				/// <summary>
				/// Compile-time layout calculator for projected list items within a parent layout.
				/// </summary>
				/// <typeparam name="ParentLayout">The parent layout within which the list items are arranged.</typeparam>
				/// <typeparam name="Style">The list style containing orientation, visibility, and margin settings.</typeparam>
				template<typename ParentLayout, typename Style = ListStyle<>>
				class ListItemLayouter
				{
				private:
					static constexpr bool IsHorizontal = (Style::Orientation == OrientationEnum::LeftToRight) || (Style::Orientation == OrientationEnum::RightToLeft);
					static constexpr bool IsVertical = (Style::Orientation == OrientationEnum::TopToBottom) || (Style::Orientation == OrientationEnum::BottomToTop);

					static constexpr uint16_t ItemWidth = IsHorizontal
						? (ParentLayout::Width() / Style::VisibleCount) : ParentLayout::Width();
					static constexpr uint16_t ItemHeight = IsHorizontal
						? ParentLayout::Height() : (ParentLayout::Height() / Style::VisibleCount);

					static constexpr int8_t ScrollDirection = (Style::Orientation == OrientationEnum::LeftToRight || Style::Orientation == OrientationEnum::TopToBottom) ? -1 : 1;

					static constexpr Framework::Layout::AlignmentEnum ItemAlignment = IsHorizontal ?
						(Style::Orientation == OrientationEnum::LeftToRight ? Framework::Layout::AlignmentEnum::MiddleLeft : Framework::Layout::AlignmentEnum::MiddleRight)
						: (Style::Orientation == OrientationEnum::TopToBottom ? Framework::Layout::AlignmentEnum::TopCenter : Framework::Layout::AlignmentEnum::BottomCenter)
						;

					struct RawItemLayout
					{
						static constexpr int16_t Width() { return ItemWidth; }
						static constexpr int16_t Height() { return ItemHeight; }

						static constexpr int16_t X() { return ParentLayout::X(); }
						static constexpr int16_t Y() { return ParentLayout::Y(); }
					};

					using CenteredLayout = Framework::Layout::Align<ParentLayout, RawItemLayout, Framework::Layout::AlignmentEnum::MiddleCenter>;

				public:
				// Center the logical item slot before applying its projection translation.
					using PageLayout = CenteredLayout;

				// Child views use this layout so margins and intrinsic dimensions remain compile-time known.
					using ViewLayout = Framework::Layout::Margin<PageLayout,
						Style::ItemMarginLeft, Style::ItemMarginTop,
						Style::ItemMarginRight, Style::ItemMarginBottom>;
				};
			}
		}
	}
}
#endif