#ifndef _EGFX_LAYOUT_h
#define _EGFX_LAYOUT_h

#include "RgbColor.h"

namespace Egfx
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
}
#endif