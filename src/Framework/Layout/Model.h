#ifndef _EGFX_FRAMEWORK_LAYOUT_MODEL_h
#define _EGFX_FRAMEWORK_LAYOUT_MODEL_h

#include "../../Platform/Platform.h"
#include "../Support.h"

namespace Egfx
{
	namespace Framework
	{
		/// <summary>
		/// Base layout contract used by EGFX layout templates.
		///
		/// Contract:
		/// - A layout is a pure compile-time "rectangle provider" implemented via static constexpr functions.
		/// - Required members: X(), Y(), Width(), Height().
		/// - Coordinates are pixel units in the same coordinate space as the framebuffer.
		///
		/// Conventions (framework-wide):
		/// - Unless explicitly stated otherwise, layouts are expected to be top-left anchored:
		///   X()/Y() represent the top-left corner of the layout's rectangle.
		/// - Layouts that need alignment/positioning within a parent should be composed with Layout::Align / Layout::Center.
		/// - Width()/Height() represent the size of the rectangle; implementations should avoid returning negative sizes.
		///
		/// Notes:
		/// - This type exists as documentation; implementations are not meant to inherit from it.
		/// </summary>
		struct LayoutContract
		{
			static constexpr pixel_t X() { return 0; }
			static constexpr pixel_t Y() { return 0; }

			static constexpr pixel_t Width() { return 0; }
			static constexpr pixel_t Height() { return 0; }
		};

		/// <summary>
		/// Compile-time layout with fixed coordinates and size.
		///
		/// This is the simplest layout type and is useful for:
		/// - Defining absolute rectangles.
		/// - Adapting/overriding positions and sizes while still satisfying the layout contract.
		/// - Producing "named" sub-layouts from computed values.
		///
		/// Conventions:
		/// - X()/Y() represent the top-left corner of the rectangle.
		/// - Width()/Height() represent the rectangle dimensions.
		/// </summary>
		/// <typeparam name="x">The x-coordinate (top-left) of the layout.</typeparam>
		/// <typeparam name="y">The y-coordinate (top-left) of the layout.</typeparam>
		/// <typeparam name="width">The width of the layout rectangle.</typeparam>
		/// <typeparam name="height">The height of the layout rectangle.</typeparam>
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
}
#endif