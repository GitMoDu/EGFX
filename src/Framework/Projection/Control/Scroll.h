#ifndef _EGFX_FRAMEWORK_PROJECTION_CONTROL_SCROLL_h
#define _EGFX_FRAMEWORK_PROJECTION_CONTROL_SCROLL_h

#include "../Model.h"

namespace Egfx
{
	namespace Framework
	{
		namespace Projection
		{
			namespace Control
			{
				namespace Scroll
				{
					/// <summary>
					/// Signed logical scroll units. Each whole UFraction16 scalar unit represents one
					/// controller-defined item step; the remainder represents fractional movement.
					/// </summary>
					using scroll_t = int32_t;

					/// <summary>
					/// Immutable snapshot consumed by a projector during one projection pass.
					/// </summary>
					struct PagedScrollState
					{
						scroll_t Scroll;
						uint8_t PageCount;
						uint8_t Version;
					};

					/// <summary>
					/// Stores logical scroll position and a version used to detect projection changes.
					/// The owning semantic view exposes domain-specific control methods to callers.
					/// </summary>
					class ScrollController
					{
					protected:
						scroll_t Scroll = 0;
						uint8_t Version = 0;

					public:
						ScrollController()
						{}

						~ScrollController() = default;

						void SetScroll(const scroll_t scroll)
						{
							if (scroll != Scroll)
							{
								Scroll = scroll;
								Version++;
							}
						}

						scroll_t GetScroll() const
						{
							return Scroll;
						}

						uint8_t GetVersion() const
						{
							return Version;
						}
					};

					/// <summary>
					/// Adds page-count and page-position operations to the logical scroll controller.
					/// </summary>
					class PagedScrollController : public ScrollController
					{
					private:
						uint8_t PageCount = 0;

					public:
						PagedScrollController(const uint8_t pageCount)
							: ScrollController()
							, PageCount(pageCount)
						{}

						~PagedScrollController() = default;


						void SetPageCount(const uint8_t pageCount)
						{
							PageCount = pageCount;
							Version++;
						}

						uint8_t GetPageCount() const
						{
							return PageCount;
						}

						void SetPagePosition(const uint8_t index, const UFraction16::scalar_t offset)
						{
							SetScroll(static_cast<scroll_t>(index) * UFraction16::SCALAR_UNIT + offset);
						}

						void SetPageStartOverscroll(const UFraction16::scalar_t offset)
						{
							const UFraction16::scalar_t limitedOffset = MinValue<UFraction16::scalar_t>(
								offset, static_cast<UFraction16::scalar_t>(UFraction16::SCALAR_UNIT / 2));
							SetScroll(-static_cast<scroll_t>(limitedOffset));
						}

						PagedScrollState GetState() const
						{
							return PagedScrollState{ GetScroll(), PageCount, GetVersion() };
						}
					};
				}
			}
		}
	}
}
#endif