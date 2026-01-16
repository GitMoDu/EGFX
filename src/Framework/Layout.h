#ifndef _EGFX_FRAMEWORK_LAYOUT_h
#define _EGFX_FRAMEWORK_LAYOUT_h

#include "../Platform/Platform.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Defines a contract for layout components, specifying position and size properties.
		/// </summary>
		struct LayoutContract
		{
			static constexpr pixel_t X() { return 0; }
			static constexpr pixel_t Y() { return 0; }

			static constexpr pixel_t Width() { return 0; }
			static constexpr pixel_t Height() { return 0; }
		};

		/// <summary>
		/// Template layout definition specifying position and size as compile-time constants,
		/// following the LayoutContract structure.
		/// </summary>
		/// <typeparam name="x">The x-coordinate of the layout's position.</typeparam>
		/// <typeparam name="y">The y-coordinate of the layout's position.</typeparam>
		/// <typeparam name="width">The width of the layout region.</typeparam>
		/// <typeparam name="height">The height of the layout region.</typeparam>
		template<const pixel_t x, const pixel_t y,
			const pixel_t width, const pixel_t height>
		struct TemplateLayout
		{
			static constexpr pixel_t X() { return x; }
			static constexpr pixel_t Y() { return y; }

			static constexpr pixel_t Width() { return width; }
			static constexpr pixel_t Height() { return height; }
		};

		/// <summary>
		/// Constrains a child layout to a fixed aspect ratio inside a parent layout and optional margin.
		/// This behaves like a constexpr version of a “letterbox” scaler: the parent space is reduced by the margin,
		/// then the aspect ratio is respected by choosing either the available width or height.
		/// </summary>
		/// <typeparam name="ParentLayout">Layout that provides the parent rectangle.</typeparam>
		/// <typeparam name="AspectWidth">Horizontal component of the target aspect ratio.</typeparam>
		/// <typeparam name="AspectHeight">Vertical component of the target aspect ratio.</typeparam>
		/// <typeparam name="Margin">Symmetric padding applied on all sides before aspect constraining.</typeparam>
		template<typename ParentLayout,
			pixel_t AspectWidth,
			pixel_t AspectHeight,
			pixel_t Margin = 0
		>
		struct ConstrainedRatioLayout
		{
			static_assert(AspectHeight > 0 && AspectWidth > 0, "Aspect ratio values must be greater than zero.");

		private:
			// Pull the parent’s dimensions once so that nested calculations remain constexpr-friendly.
			static constexpr pixel_t ParentWidth() { return ParentLayout::Width(); }
			static constexpr pixel_t ParentHeight() { return ParentLayout::Height(); }

			// Horizontal/vertical margins are halved if the caller asked for more padding than the parent can provide.
			// This avoids negative “available” sizes when the parent area is extremely small.
			static constexpr pixel_t HorizontalMargin()
			{
				return (Margin * 2) >= ParentWidth() ? (ParentWidth() / 2) : Margin;
			}

			static constexpr pixel_t VerticalMargin()
			{
				return (Margin * 2) >= ParentHeight() ? (ParentHeight() / 2) : Margin;
			}

			// Remaining drawable span after subtracting margins. When the margins completely consume the parent,
			// we clamp to zero to keep downstream math well-defined.
			static constexpr pixel_t AvailableWidth()
			{
				return ParentWidth() >= (HorizontalMargin() * 2) ? (ParentWidth() - (HorizontalMargin() * 2)) : 0;
			}

			static constexpr pixel_t AvailableHeight()
			{
				return ParentHeight() >= (VerticalMargin() * 2) ? (ParentHeight() - (VerticalMargin() * 2)) : 0;
			}

			// Flag indicating which dimension becomes the limiter when enforcing the aspect ratio.
			// If the available width can fit the aspect ratio, we keep the full width; otherwise we shrink
			// horizontally and derive the width from the available height.
			static constexpr bool WidthLimited()
			{
				return (uint32_t(AvailableWidth()) * AspectHeight) <= (uint32_t(AvailableHeight()) * AspectWidth);
			}

		public:
			static constexpr pixel_t Width()
			{
				return WidthLimited()
					? AvailableWidth()
					: static_cast<pixel_t>((uint32_t(AvailableHeight()) * AspectWidth) / AspectHeight);
			}

			static constexpr pixel_t Height()
			{
				return WidthLimited()
					? static_cast<pixel_t>((uint32_t(AvailableWidth()) * AspectHeight) / AspectWidth)
					: AvailableHeight();
			}

			static constexpr pixel_t X()
			{
				// Start at parent X + margin, then center the constrained aspect rect inside the available area.
				// Centering after clamping keeps the child balanced even when one dimension is reduced.
				return ParentLayout::X()
					+ HorizontalMargin()
					+ ((AvailableWidth() - Width()) / 2);
			}

			static constexpr pixel_t Y()
			{
				// Start at parent Y + margin, then center the constrained aspect rect inside the available area.
				return ParentLayout::Y()
					+ VerticalMargin()
					+ ((AvailableHeight() - Height()) / 2);
			}
		};
	}
}
#endif